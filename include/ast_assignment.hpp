#ifndef AST_ASSIGNMENT_HPP
#define AST_ASSIGNMENT_HPP

#include "ast_node.hpp"

class Assignment : public ExpressionRoot
{
private:
    ExpressionRoot* identifier;
    ExpressionRoot* expression;

public:
    Assignment(ExpressionRoot* _identifier, ExpressionRoot* _expression);
    ~Assignment();

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;

    Specifier getType(Context& ctx) const override;
};

#endif
