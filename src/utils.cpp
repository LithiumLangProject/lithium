#include "utils.hpp"
#include "lexar.hpp"
#include "semantic.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace FileUtils {
    bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    
    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + path);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    bool writeFile(const std::string& path, const std::string& content) {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return true;
    }
    
    std::string getFileExtension(const std::string& path) {
        std::filesystem::path p(path);
        return p.extension().string();
    }
    
    std::string getBaseName(const std::string& path) {
        std::filesystem::path p(path);
        return p.stem().string();
    }
    
    std::string getDirectory(const std::string& path) {
        std::filesystem::path p(path);
        return p.parent_path().string();
    }
    
    std::string resolvePath(const std::string& basePath, const std::string& relativePath) {
        std::filesystem::path base(basePath);
        std::filesystem::path relative(relativePath);
        return (base / relative).string();
    }
}

namespace DebugUtils {
    void printAST(ASTNode* node, int indent) {
        // TODO: Implement actual AST printing
        if (node) {
            std::cout << std::string(indent, ' ') << "AST Node (stub)\n";
        }
    }
    
    void printTokens(const std::vector<Token>& tokens) {
        for (const auto& token : tokens) {
            std::cout << "Token: " << static_cast<int>(token.type) 
                     << " Value: '" << token.value << "' "
                     << " Position: " << token.position.filename 
                     << ":" << token.position.line 
                     << ":" << token.position.column << std::endl;
        }
    }
    
    void printSymbolTable(const SymbolTable& table) {
        // TODO: Implement actual symbol table printing
        std::cout << "Symbol Table (stub)\n";
    }
}