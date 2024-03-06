#ifndef AST_TYPE_SPECIFIER
#define AST_TYPE_SPECIFIER

#include "ast_node.hpp"

class TypeSpecifier : public Node
{
private:
    std::string type_;

public:
    TypeSpecifier(const std::string& type) : type_(type){};
    virtual ~TypeSpecifier(){};
    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    virtual void Print(std::ostream &stream) const override;
};

#endif
