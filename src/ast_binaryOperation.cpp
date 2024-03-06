#include "ast_binaryOperation.hpp"

BinaryOperation::BinaryOperation(ExpressionRoot* _left, ExpressionRoot* _right, bool _owns_operands)
        : left(_left), right(_right), owns_operands(_owns_operands) {}

BinaryOperation::~BinaryOperation() {
    if (owns_operands) delete left;
    delete right;
};

Specifier BinaryOperation::getType(Context& context) const {
    return right->getType(context);
}

int BinaryOperation::allocateRegBasedOnType(Context& context, Specifier type, bool isFunctionCall) const {

    int reg = context.allocateReg(type);

    if (isFunctionCall) {
        reg = (type == Specifier::_int) ? 9 : 41; // s1 or fs1 callee saved reg
    }

    context.useReg(reg);
    return reg;
}

int BinaryOperation::prepLeft(std::ostream& stream, Context& context, int destReg) const {
    Specifier type = left->getType(context);

    // only need to check if left is a function call - as left recursive ((1+2)+3)..
    int leftReg = allocateRegBasedOnType(context, type, right->isFuncCall());

    left->EmitRISC(stream, context, leftReg);
    return leftReg;
}

int BinaryOperation::prepRight(std::ostream& stream, Context& context, int destReg) const {
    Specifier type = right->getType(context);
    int var_size = typeSizes.at(type);
    // if left was a pointer var, then right operand should be an integer - for pointer arithemetic
    // int *p = &a, p = p + 2 (p + size(pointed by p) * 2)
    bool is_pointer_var = !left->isDerefPointer() && context.getIsPointer(left->getIdentifier());

    int rightReg = allocateRegBasedOnType(context, type, false);
    right->EmitRISC(stream, context, rightReg);

    if (is_pointer_var) {
        stream << "slli " << context.getMnemonic(rightReg) << ", " << context.getMnemonic(rightReg) << ", " << log2(var_size) << std::endl;
    }

    return rightReg;
}

void BinaryOperation::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    std::cerr << "Error: EmitRISC() should not be called on a BinaryOperation object" << std::endl;
};



void Equal::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sub " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "seqz " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void Equal::Print(std::ostream& stream) const {
    stream << "Equal Operation";
}


void NotEqual::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sub " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "snez " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void NotEqual::Print(std::ostream& stream) const {
    stream << "NotEqual Operation";
}


void LessThan::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "slt " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void LessThan::Print(std::ostream& stream) const {
    stream << "LessThan Operation";
}


void GreaterThan::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sgt " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void GreaterThan::Print(std::ostream& stream) const {
    stream << "GreaterThan Operation";

}


void LessThanEqual::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "sgt " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "xori " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", " << "1" << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void LessThanEqual::Print(std::ostream& stream) const {
    stream << "LessThanEqual Operation";

}


void GreaterThanEqual::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "slt " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;
    stream << "xori " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", " << "1" << std::endl;
    stream << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void GreaterThanEqual::Print(std::ostream& stream) const {
    stream << "GreaterThanEqual Operation";
}


void BitwiseAnd::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "and " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void BitwiseAnd::Print(std::ostream& stream) const {
    stream << "BitwiseAnd Operation";
}


void BitwiseXor::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "xor " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void BitwiseXor::Print(std::ostream& stream) const {
    stream << "BitwiseXor Operation";
}


void BitwiseOr::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int left_reg = prepLeft(stream, context, destReg);
    int right_reg = prepRight(stream, context, destReg);

    stream << "or " << context.getMnemonic(destReg) << ", " << context.getMnemonic(left_reg) << ", " << context.getMnemonic(right_reg) << std::endl;

    context.freeReg(left_reg);
    context.freeReg(right_reg);

}

void BitwiseOr::Print(std::ostream& stream) const {
    stream << "BitwiseOr Operation";
}


void LogicalAnd::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    std::string false_label = context.makeLabel(".AND_FALSE");
    std::string end_label = context.makeLabel(".AND_END");

    int left_reg = context.allocateReg(Specifier::_int);
    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);
    if (rightExpr && rightExpr->isFuncCall()) {
        left_reg = 9;
    }
    context.useReg(left_reg);

    left->EmitRISC(stream, context, left_reg);
    stream << "beq " << context.getMnemonic(left_reg) << ", zero, " << false_label << std::endl;

    // would short circuit, so can use same reg
    right->EmitRISC(stream, context, left_reg);
    stream << "beq " << context.getMnemonic(left_reg) << ", zero, " << false_label << std::endl;

    // If both are true, set destination register to 1 (true)
    stream << "li " << context.getMnemonic(destReg) << ", 1" << std::endl;
    stream << "j " << end_label << std::endl;

    // If either expression is false, set destReg to 0
    stream << false_label << ":" << std::endl;
    stream << "li " << context.getMnemonic(destReg) << ", 0" << std::endl;
    stream << end_label << ":" << std::endl;

    context.freeReg(left_reg);
}

void LogicalAnd::Print(std::ostream& stream) const {
    stream << "LogicalAnd Operation";
}


void LogicalOr::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    std::string true_label = context.makeLabel(".OR_TRUE");
    std::string end_label = context.makeLabel(".OR_END");

    int left_reg = context.allocateReg(Specifier::_int);

    const ExpressionRoot* rightExpr = dynamic_cast<const ExpressionRoot*>(right);
    if (rightExpr && rightExpr->isFuncCall()) {
        left_reg = 9;
    }
    context.useReg(left_reg);

    left->EmitRISC(stream, context, left_reg);
    stream << "bne " << context.getMnemonic(left_reg) << ", zero, " << true_label << std::endl;

    right->EmitRISC(stream, context, left_reg);
    stream << "bne " << context.getMnemonic(left_reg) << ", zero, " << true_label << std::endl;

    // If both are false, set destination register to 0 (false)
    stream << "li " << context.getMnemonic(destReg) << ", 0" << std::endl;
    stream << "j " << end_label << std::endl;

    // If either expression is true, set destReg to 1 (true)
    stream << true_label << ":" << std::endl;
    stream << "li " << context.getMnemonic(destReg) << ", 1" << std::endl;

    // End label
    stream << end_label << ":" << std::endl;
    context.freeReg(left_reg);
}

void LogicalOr::Print(std::ostream& stream) const {
    stream << "LogicalOr Operation";
}
