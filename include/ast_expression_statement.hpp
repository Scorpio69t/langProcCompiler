#ifndef AST_EXPRESSION_STATEMENT_HPP
#define AST_EXPRESSION_STATEMENT_HPP

#include "ast_node.hpp"

class ExpressionStatement : public StatementRoot
{
private:
    ExpressionRoot* expression;
public:
    ExpressionStatement(ExpressionRoot* _expression);
    virtual ~ExpressionStatement();

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

#endif
