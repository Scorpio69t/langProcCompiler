#ifndef ast_arithematic_hpp
#define ast_arithematic_hpp

#include "ast_node.hpp"
#include "ast_binaryOperation.hpp"

// using BinaryOperation::BinaryOperation; lets us recycle constructors + destructors from BinaryOperation
// Left and Right in BinaryOp are also protected - so can still be accessed if needed


class Addition
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};

class Subtraction
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};

class Multiplication
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};

class Division
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};


class Modulus
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};


class LeftShift
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};


class RightShift
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};

#endif
