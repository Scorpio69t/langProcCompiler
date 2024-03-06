#ifndef AST_FOR_STATEMENT_HPP
#define AST_FOR_STATEMENT_HPP

#include "ast_node.hpp"

class ForStatement: public StatementRoot{
private:
    StatementRoot* initializer;
    StatementRoot* condition;
    ExpressionRoot* increment;
    StatementRoot* statement;

public:
    ForStatement(StatementRoot* _init, StatementRoot* _cond, ExpressionRoot* _incr, StatementRoot* _stat);
    ForStatement(StatementRoot* _init, StatementRoot* _cond, StatementRoot* _stat);

    virtual ~ForStatement() override;

    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    virtual void Print(std::ostream &stream) const override;
    virtual int getSize() const override;
};

#endif
