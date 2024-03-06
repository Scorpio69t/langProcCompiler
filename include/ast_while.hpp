#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include "ast_node.hpp"

class WhileStatement : public StatementRoot
{
private:
    ExpressionRoot* condition;
    StatementRoot* body;

public:
    WhileStatement(ExpressionRoot* _condition, StatementRoot* _body);
    ~WhileStatement();

    int getSize() const override;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

#endif
