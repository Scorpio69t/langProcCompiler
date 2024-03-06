#ifndef AST_SWITCH_HPP
#define AST_SWITCH_HPP

#include "ast_node.hpp"

class SwitchStatement : public StatementRoot
{
private:
    ExpressionRoot* condition;
    StatementRoot* body;

public:
    SwitchStatement(ExpressionRoot* _condition, StatementRoot* _body);
    ~SwitchStatement();

    int getSize() const override;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

#endif
