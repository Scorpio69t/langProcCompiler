#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP

#include "ast_node.hpp"

class Identifier : public ExpressionRoot
{
private:
    std::string identifier_;

public:
    Identifier(const std::string identifier) : identifier_(identifier){};

    void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    void Print(std::ostream &stream) const override;

    Specifier getType(Context &ctx) const override;
    const std::string &getIdentifier() const override;
};

#endif
