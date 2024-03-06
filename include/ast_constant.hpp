#ifndef AST_CONSTANT_HPP
#define AST_CONSTANT_HPP

#include "ast_node.hpp"

class IntConstant : public ExpressionRoot
{
private:
    int value_;

public:
    IntConstant(int value) : value_(value) {}
    ~IntConstant() {}

    void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    void Print(std::ostream &stream) const override;
};

class FloatConstant : public ExpressionRoot
{
private:
    float value_;

public:
    FloatConstant(float value) : value_(value) {}

    void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    void Print(std::ostream &stream) const override;
};

class CharConstant : public ExpressionRoot
{
private:
    std::string value_;

public:
    CharConstant(std::string value) : value_(value) {
        value_.erase(0, 1);
        value_.pop_back();
    }

    void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    void Print(std::ostream &stream) const override;
};

class StringConstant : public ExpressionRoot
{
private:
    std::string value_;

public:
    StringConstant(const std::string &value) : value_(value) {
        value_.erase(0, 1);
        value_.pop_back();
    }

    void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    void Print(std::ostream &stream) const override;
};


#endif
