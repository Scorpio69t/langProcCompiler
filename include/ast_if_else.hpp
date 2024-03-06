#ifndef AST_IF_ELSE_HPP
#define AST_IF_ELSE_HPP

#include "ast_node.hpp"

class IfElseStatement : public StatementRoot
{
private:
    ExpressionRoot* condition;
    StatementRoot* ifBody;
    StatementRoot* elseBody;

public:
    IfElseStatement(ExpressionRoot* _condition, StatementRoot* _ifBody, StatementRoot* _elseBody);
    ~IfElseStatement();

    int getSize() const override;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

#endif
