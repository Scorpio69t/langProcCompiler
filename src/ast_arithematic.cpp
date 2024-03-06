#include "ast_arithematic.hpp"


void Addition::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    const ExpressionRoot* leftExpr = dynamic_cast<const ExpressionRoot*>(left);
    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);

    if (!leftExpr || !rightExpr) {
        std::cerr << "Addition: Either left or right node is not an ExpressionRoot." << std::endl;
        exit(1);
    }

    Specifier left_type = leftExpr->getType(context);
    Specifier right_type = rightExpr->getType(context);

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    // if left contains a pointer, left_type will be type of the object the pointer is pointing to
    // e.g. for example if left_type is a float pointer -> it will return float
    // however pointers require int operation
    // this is why we switch on right_type which will always be the correct type for pointer arithmetic
    switch (right_type) {
        case Specifier::_int:
            stream << "add " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_double:
            stream << "fadd.d " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_float:
            stream << "fadd.s " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        default:
            std::cerr << "Addition: Invalid op type" << std::endl;
            exit(1);
    }

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void Addition::Print(std::ostream& stream) const {
    stream << "Addition Operation";
}


void Subtraction::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    const ExpressionRoot* leftExpr = dynamic_cast<const ExpressionRoot*>(left);
    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);

    if (!leftExpr || !rightExpr) {
        std::cerr << "Subtraction: Either left or right node is not an ExpressionRoot." << std::endl;
        exit(1);
    }

    Specifier left_type = leftExpr->getType(context);
    Specifier right_type = rightExpr->getType(context);

    switch (right_type) {
        case Specifier::_int:
            stream << "sub " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_double:
            stream << "fsub.d " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_float:
            stream << "fsub.s " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        default:
            std::cerr << "Subtraction: Invalid op type" << std::endl;
            exit(1);
    }

    context.freeReg(left_reg);
    context.freeReg(right_reg);
}

void Subtraction::Print(std::ostream& stream) const {
    stream << "Subtraction Operation";
}


void Multiplication::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    const ExpressionRoot* leftExpr = dynamic_cast<const ExpressionRoot*>(left);
    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);

    if (!leftExpr || !rightExpr) {
        std::cerr << "Multiplication: Either left or right node is not an ExpressionRoot." << std::endl;
        exit(1);
    }

    Specifier left_type = leftExpr->getType(context);
    Specifier right_type = rightExpr->getType(context);

    switch (right_type) {
        case Specifier::_int:
            stream << "mul " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_double:
            stream << "fmul.d " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_float:
            stream << "fmul.s " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        default:
            std::cerr << "Multiplication: Invalid op type" << std::endl;
            exit(1);
    }

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void Multiplication::Print(std::ostream& stream) const {
    stream << "Multiplication Operation";
}


void Division::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    const ExpressionRoot* leftExpr = dynamic_cast<const ExpressionRoot*>(left);
    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);

    if (!leftExpr || !rightExpr) {
        std::cerr << "Division: Either left or right node is not an ExpressionRoot." << std::endl;
        exit(1);
    }

    Specifier left_type = leftExpr->getType(context);
    Specifier right_type = rightExpr->getType(context);

    switch (right_type) {
        case Specifier::_int:
            stream << "div " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_double:
            stream << "fdiv.d " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        case Specifier::_float:
            stream << "fdiv.s " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
            break;
        default:
            std::cerr << "Division: Invalid op type" << std::endl;
            exit(1);
    }

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void Division::Print(std::ostream& stream) const {
    stream << "Division Operation";
}


void Modulus::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "rem " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void Modulus::Print(std::ostream& stream) const {
    stream << "Modulus Operation";
}


void LeftShift::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sll " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void LeftShift::Print(std::ostream& stream) const {
    stream << "LeftShift Operation";
}


void RightShift::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sra " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void RightShift::Print(std::ostream& stream) const {
    stream << "RightShift Operation";
}
