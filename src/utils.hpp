#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

struct Token;
class ASTNode;
class SymbolTable;

namespace FileUtils {
    bool fileExists(const std::string& path);
    std::string readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);
    std::string getFileExtension(const std::string& path);
    std::string getBaseName(const std::string& path);
    std::string getDirectory(const std::string& path);
    std::string resolvePath(const std::string& basePath, const std::string& relativePath);
}

namespace StringUtils {
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    std::string replace(const std::string& str, const std::string& from, const std::string& to);
}

namespace DebugUtils {
    void printAST(class ASTNode* node, int indent = 0);
    void printTokens(const std::vector<Token>& tokens);
    void printSymbolTable(const SymbolTable& table);
}

namespace CompilerUtils {
    std::string generateUniqueId(const std::string& prefix = "");
    bool isValidIdentifier(const std::string& name);
    bool isKeyword(const std::string& word);
    std::string escapeString(const std::string& str);
    std::string unescapeString(const std::string& str);
}