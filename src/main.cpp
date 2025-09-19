#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "lexar.hpp"
#include "parser.hpp"
#include "semantic.hpp"
#include "codegen.hpp"
#include "error.hpp"
#include "utils.hpp"

struct CompilerOptions {
    std::string inputFile;
    std::string outputFile;
    bool verbose = false;
    bool debugLexer = false;
    bool debugParser = false;
    bool debugSemantic = false;
    TargetType targetType = TargetType::EXECUTABLE;
};

void printUsage(const char* programName);
bool parseArguments(int argc, char* argv[], CompilerOptions& options);
std::string readSourceFile(const std::string& filename);
int compileFile(const CompilerOptions& options);

int main(int argc, char* argv[]) {
    CompilerOptions options;
    
    if (!parseArguments(argc, argv, options)) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    
    return compileFile(options);
}

void printUsage(const char* programName) {
    std::cout << "Lithium Compiler v1.0\n";
    std::cout << "Usage: " << programName << " [options] <input-file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o <file>     Specify output file\n";
    std::cout << "  -v, --verbose Enable verbose output\n";
    std::cout << "  --debug-lexer Enable lexer debugging\n";
    std::cout << "  --debug-parser Enable parser debugging\n";
    std::cout << "  --debug-semantic Enable semantic analysis debugging\n";
    std::cout << "  -t <type>     Target type (exe, asm, ir)\n";
    std::cout << "  -h, --help    Show this help message\n";
}

bool parseArguments(int argc, char* argv[], CompilerOptions& options) {
    if (argc < 2) {
        return false;
    }
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            return false;
        } else if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "--debug-lexer") {
            options.debugLexer = true;
        } else if (arg == "--debug-parser") {
            options.debugParser = true;
        } else if (arg == "--debug-semantic") {
            options.debugSemantic = true;
        } else if (arg == "-o" && i + 1 < argc) {
            options.outputFile = argv[++i];
        } else if (arg == "-t" && i + 1 < argc) {
            std::string target = argv[++i];
            if (target == "exe") {
                options.targetType = TargetType::EXECUTABLE;
            } else if (target == "asm") {
                options.targetType = TargetType::ASSEMBLY;
            } else if (target == "ir") {
                options.targetType = TargetType::INTERMEDIATE;
            } else {
                std::cerr << "Error: Unknown target type '" << target << "'\n";
                return false;
            }
        } else if (arg[0] != '-') {
            if (options.inputFile.empty()) {
                options.inputFile = arg;
            } else {
                std::cerr << "Error: Multiple input files specified\n";
                return false;
            }
        } else {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            return false;
        }
    }
    
    if (options.inputFile.empty()) {
        std::cerr << "Error: No input file specified\n";
        return false;
    }
    
    if (options.outputFile.empty()) {
        std::filesystem::path inputPath(options.inputFile);
        std::string baseName = inputPath.stem().string();
        
        switch (options.targetType) {
            case TargetType::EXECUTABLE:
                options.outputFile = baseName;
                break;
            case TargetType::ASSEMBLY:
                options.outputFile = baseName + ".s";
                break;
            case TargetType::INTERMEDIATE:
                options.outputFile = baseName + ".ir";
                break;
        }
    }
    
    return true;
}

std::string readSourceFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int compileFile(const CompilerOptions& options) {
    try {
        std::string sourceCode = readSourceFile(options.inputFile);
        
        if (options.verbose) {
            std::cout << "Compiling " << options.inputFile << "...\n";
        }
        
        ErrorReporter errorReporter;
        
        Lexer lexer(sourceCode, options.inputFile);
        std::vector<Token> tokens = lexer.tokenize();
        
        if (options.debugLexer) {
            std::cout << "=== TOKENS ===\n";
            DebugUtils::printTokens(tokens);
        }
        
        if (errorReporter.hasAnyErrors()) {
            errorReporter.printErrors();
            return EXIT_FAILURE;
        }
        
        Parser parser(std::move(tokens), errorReporter);
        auto program = parser.parseProgram();
        
        if (options.debugParser) {
            std::cout << "=== AST ===\n";
            DebugUtils::printAST(program.get());
        }
        
        if (errorReporter.hasAnyErrors()) {
            errorReporter.printErrors();
            return EXIT_FAILURE;
        }
        
        SemanticAnalyzer semanticAnalyzer(errorReporter);
        bool semanticSuccess = semanticAnalyzer.analyze(program.get());
        
        if (options.debugSemantic) {
            std::cout << "=== SEMANTIC ANALYSIS ===\n";
            std::cout << "Analysis " << (semanticSuccess ? "passed" : "failed") << "\n";
        }
        
        if (errorReporter.hasAnyErrors()) {
            errorReporter.printErrors();
            return EXIT_FAILURE;
        }
        
        Target target(options.targetType, options.outputFile);
        CodeGenerator codeGenerator(target, errorReporter);
        bool codeGenSuccess = codeGenerator.generate(program.get(), options.outputFile);
        
        if (errorReporter.hasAnyErrors()) {
            errorReporter.printErrors();
            return EXIT_FAILURE;
        }
        
        if (!codeGenSuccess) {
            std::cerr << "Error: Code generation failed\n";
            return EXIT_FAILURE;
        }
        
        if (options.verbose) {
            std::cout << "Compilation successful. Output: " << options.outputFile << "\n";
        }
        
        return EXIT_SUCCESS;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
