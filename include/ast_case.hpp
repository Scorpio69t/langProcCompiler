#ifndef AST_CASE_HPP
#define AST_CASE_HPP

#include "ast_node.hpp"

class Case
    : public StatementRoot
{
private:
    ExpressionRoot* expression;
    StatementRoot* statements;

public:
    Case(ExpressionRoot* _expression, StatementRoot* _statements);
    Case(StatementRoot* _statements);
    ~Case();

    int getSize() const override;
    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

#endif
