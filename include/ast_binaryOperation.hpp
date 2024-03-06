#ifndef ast_binaryOperation_hpp
#define ast_binaryOperation_hpp

#include "ast_node.hpp"

#include <cmath>
#include <string>

/*
class BinaryOperation

    : public ExpressionRoot
{
private:

protected:
    Node_Ptr left;
    Node_Ptr right;
    BinaryOperation(Node_Ptr _left,
        Node_Ptr _right)
        :left(_left),
        right(_right)
    {}
public:

    virtual Specifier getType(Context& context) const = 0;
    virtual ~BinaryOperation()
    {delete left;
    delete right;}

    virtual const std::string getOpcode() const =0;

    virtual void Print(std::ostream &stream) const override
    {
        stream<<"(";
        left->Print(stream);
        stream<<" "<<getOpcode()<<" ";
        right->Print(stream);
        stream<<")";
    }
    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg)const override{}
    virtual const std::string& getIdentifier() const override{
        std::cerr<<"getting Id of binary expression";
        exit(1);
    };

    int allocateRegBasedOnType(Context& context, Specifier type, bool isFunctionCall) const {

        int reg = context.allocateReg(type);
        if (auto rightExprPtr = dynamic_cast<const ExpressionRoot*>(right)) {
            if (rightExprPtr->isFuncCall()) {
                reg = (type == Specifier::_int) ? 9 : 41;
            }
        }

        context.useReg(reg);
        return reg;
    }

    int prepLeft(std::ostream &stream, Context &context, int destReg) const{
        Specifier type;  // Assume a default or invalid value exists.
        if (auto leftExprPtr = dynamic_cast<const ExpressionRoot*>(left)) {
            type = leftExprPtr->getType(context);
        }

        bool isFunctionCall = false;
        if (auto rightExprPtr = dynamic_cast<const ExpressionRoot*>(right)) {
            isFunctionCall = rightExprPtr->isFuncCall();
        }

        int leftReg = allocateRegBasedOnType(context, type, isFunctionCall);
        left->EmitRISC(stream, context, leftReg);
        return leftReg;
    }

    int prepRight(std::ostream &stream, Context &context, int destReg) const{
        Specifier type;  // Again, assuming a default or invalid value.
        if (auto rightExprPtr = dynamic_cast<const ExpressionRoot*>(right)) {
            type = rightExprPtr->getType(context);
        }

        int var_size = typeSizes.at(type);
        bool is_pointer_var = false;

        if (auto leftExprPtr = dynamic_cast<const ExpressionRoot*>(left)) {
            is_pointer_var = !leftExprPtr->isDerefPointer() && context.getIsPointer(leftExprPtr->getIdentifier());
        }

        int rightReg = allocateRegBasedOnType(context, type, false);
        right->EmitRISC(stream, context, rightReg);

        if (is_pointer_var) {
            stream << "slli " << context.getMnemonic(rightReg) << ", " << context.getMnemonic(rightReg) << ", " << log2(var_size) << std::endl;
        }

        return rightReg;
    }

};
*/

class BinaryOperation
    : public ExpressionRoot
{
public:
    BinaryOperation(ExpressionRoot* _left, ExpressionRoot* _right, bool _owns_operands = true);
    ~BinaryOperation();

    int allocateRegBasedOnType(Context& context, Specifier type, bool isFunctionCall) const;
    int prepLeft(std::ostream& stream, Context& context, int destReg) const;
    int prepRight(std::ostream& stream, Context& context, int destReg) const;

    Specifier getType(Context& context) const override;
    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

protected:
    ExpressionRoot* left;
    ExpressionRoot* right;
    bool owns_operands;
};


class Equal
    : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;

};


class NotEqual
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class LessThan
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class GreaterThan
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class LessThanEqual
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class GreaterThanEqual
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class BitwiseAnd
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class BitwiseXor
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class BitwiseOr
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class LogicalAnd
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};


class LogicalOr
    : public BinaryOperation
{
    using BinaryOperation::BinaryOperation;

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;

    void Print(std::ostream& stream) const override;
};

#endif
