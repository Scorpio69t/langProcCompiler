#include "ast_unary.hpp"

void SizeOfExpression::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    std::string var_name = expr->getIdentifier();
    Specifier var_type = context.getVarType(var_name);
    int size = typeSizes.at(var_type);
    stream << "li " << context.getMnemonic(destReg) << ", " << size << std::endl;
}

void SizeOfExpression::Print(std::ostream& stream) const {
    stream << "sizeof(";
    expr->Print(stream);
    stream << ")";
}

void SizeOfType::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    int size = typeSizes.at(type);
    stream << "li " << context.getMnemonic(destReg) << ", " << size << std::endl;
}

void SizeOfType::Print(std::ostream& stream) const {
    Context ctx;
    stream << "sizeof" << ctx.specifierToString(type);
}

UnaryOp::UnaryOp(UnaryOperator _op, ExpressionRoot* _expr)
    : op(_op), expr(_expr) {}

UnaryOp::~UnaryOp() {
    delete expr;
}

bool UnaryOp::isDerefPointer() const {
    return op==UnaryOperator::Deref ? true : false;
}

const std::string& UnaryOp::getIdentifier() const {
    return expr->getIdentifier();
}

 Specifier UnaryOp::getType(Context& context) const {
    return expr->getType(context);
 }

void UnaryOp::EmitRISC(std::ostream& os, Context& context, int destReg) const {

    switch(op)
    {
        case UnaryOperator::Minus:
            expr->EmitRISC(os, context, destReg);
            os << "neg " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << std::endl;
            break;
        case UnaryOperator::Plus:
            expr->EmitRISC(os, context, destReg);
            break;
        case UnaryOperator::LogicalNot:
            expr->EmitRISC(os, context, destReg);
            os << "seqz " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << std::endl;
            os << "andi " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << ", 0xff" << std::endl;
            break;
        case UnaryOperator::BitwiseNot:
            expr->EmitRISC(os, context, destReg);
            os << "not " << context.getMnemonic(destReg) << ", " << context.getMnemonic(destReg) << std::endl;
            break;
        case UnaryOperator::Deref:
        {
            // puts value that is being pointed to into destReg
            std::string var_name = expr->getIdentifier();
            Variable var = context.getVar(var_name);

            int address_reg = context.allocateReg(Specifier::_int);
            context.useReg(address_reg);
            os << "lw " << context.getMnemonic(address_reg) << ", " << var.offset << "(s0)" << std::endl;

            context.loadInstruction(os, var.type, destReg, 0, address_reg);

            context.freeReg(address_reg);
            break;
        }
        case UnaryOperator::AddressOf:
        {
            std::string var_name = expr->getIdentifier();
            int offset = context.getVarOffset(var_name);
            os << "addi " << context.getMnemonic(destReg) << ", s0, " << offset << std::endl;
            break;
        }
    }

};


void UnaryOp::Print(std::ostream& stream) const {
    stream << "LogicalOr Operation";
}



Increment::Increment(ExpressionRoot* _expr) : expr(_expr) {};

Increment::~Increment() {
    delete expr;
}

void Increment::EmitRISC(std::ostream& os, Context& context, int destReg) const {

    std::string var_name = expr->getIdentifier();
    int reg = context.allocateReg(Specifier::_int);

    int var_offset = context.getVarOffset(var_name);
    context.useReg(reg);

    expr->EmitRISC(os, context, reg);

    os << "addi " << context.getMnemonic(reg) << ", " << context.getMnemonic(reg) << ", 1" << std::endl;
    os << "sw " << context.getMnemonic(reg) << ", " << var_offset << "(s0)" << std::endl;

    context.freeReg(reg);

}

void Increment::Print(std::ostream& stream) const {
    stream << "Increment Operation";
}


Decrement::Decrement(ExpressionRoot* _expr) : expr(_expr) {};

Decrement::~Decrement() {
    delete expr;
}

void Decrement::EmitRISC(std::ostream& os, Context& context, int destReg) const {

    std::string var_name = expr->getIdentifier();
    int reg = context.allocateReg(Specifier::_int);

    int var_offset = context.getVarOffset(var_name);
    context.useReg(reg);

    expr->EmitRISC(os, context, reg);

    os << "addi " << context.getMnemonic(reg) << ", " << context.getMnemonic(reg) << ", -1" << std::endl;
    os << "sw " << context.getMnemonic(reg) << ", " << var_offset << "(s0)" << std::endl;

    context.freeReg(reg);

}

void Decrement::Print(std::ostream& stream) const {
    stream << "Decrement Operation";
}
