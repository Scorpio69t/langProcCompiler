#ifndef AST_JUMP_STATEMENT_HPP
#define AST_JUMP_STATEMENT_HPP

#include "ast_node.hpp"

class ReturnStatement : public StatementRoot
{
private:
    ExpressionRoot* expression;
public:
    ReturnStatement(ExpressionRoot* _expression = nullptr);
    virtual ~ReturnStatement();

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

class ContinueStatement : public StatementRoot
{
public:
    ContinueStatement();
    virtual ~ContinueStatement();

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

class BreakStatement : public StatementRoot
{
public:
    BreakStatement();
    virtual ~BreakStatement();

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

#endif
