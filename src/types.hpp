#pragma once

#include <string>
#include <memory>

enum class PrimitiveType {
    INT,
    FLOAT, 
    STRING,
    BOOL,
    ANY,
    VOID
};

class Type {
public:
    virtual ~Type() = default;
    virtual bool isCompatibleWith(const Type& other) const = 0;
    virtual std::string toString() const = 0;
    virtual bool isInferred() const { return false; }
    virtual PrimitiveType getPrimitiveType() const = 0;
};

class PrimitiveTypeImpl : public Type {
private:
    PrimitiveType type;
    bool inferred;
    
public:
    explicit PrimitiveTypeImpl(PrimitiveType t, bool isInferred = false) 
        : type(t), inferred(isInferred) {}
    
    bool isCompatibleWith(const Type& other) const override;
    std::string toString() const override;
    bool isInferred() const override { return inferred; }
    PrimitiveType getPrimitiveType() const override { return type; }
    
    static std::unique_ptr<Type> createInt() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::INT); 
    }
    static std::unique_ptr<Type> createFloat() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::FLOAT); 
    }
    static std::unique_ptr<Type> createString() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::STRING); 
    }
    static std::unique_ptr<Type> createBool() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::BOOL); 
    }
    static std::unique_ptr<Type> createAny() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::ANY); 
    }
    static std::unique_ptr<Type> createVoid() { 
        return std::make_unique<PrimitiveTypeImpl>(PrimitiveType::VOID); 
    }
};

namespace TypeUtils {
    std::string primitiveTypeToString(PrimitiveType type);
    PrimitiveType stringToPrimitiveType(const std::string& typeStr);
    bool isNumericType(PrimitiveType type);
    bool canImplicitlyConvert(PrimitiveType from, PrimitiveType to);
}