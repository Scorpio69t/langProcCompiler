#ifndef AST_UNARY_HPP
#define AST_UNARY_HPP

#include "ast_node.hpp"

class SizeOfExpression : public ExpressionRoot {
private:
    ExpressionRoot* expr;

public:
    SizeOfExpression(ExpressionRoot* _expr) : expr(_expr) {}

    ~SizeOfExpression() {
        delete expr;
    }

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    virtual void Print(std::ostream& stream) const override;
};


class SizeOfType : public ExpressionRoot {
private:
    Specifier type;

public:
    SizeOfType(Specifier _type) : type(_type) {}

    ~SizeOfType() {}

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    virtual void Print(std::ostream& stream) const override;
};

enum class UnaryOperator {
    Plus,
    Minus,
    BitwiseNot,
    LogicalNot,
    Deref,
    AddressOf
};

class UnaryOp : public ExpressionRoot
{
public:
    UnaryOp(UnaryOperator _op, ExpressionRoot* _expr);
    ~UnaryOp();

    bool isDerefPointer() const override;
    const std::string& getIdentifier() const override;
    Specifier getType(Context& context) const override;
    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& dst) const override;

private:
    UnaryOperator op;
    ExpressionRoot* expr;
};


class Increment
    : public ExpressionRoot
{
public:
    Increment(ExpressionRoot* _expr);
    ~Increment();

    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& dst) const override;

private:
    ExpressionRoot* expr;
};


class Decrement
    : public ExpressionRoot
{
public:
    Decrement(ExpressionRoot* _expr);
    ~Decrement();

    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& dst) const override;

private:
    ExpressionRoot* expr;
};



#endif

//today is 2.29
//once every 4 years
//and i am spending it on making this compiler
