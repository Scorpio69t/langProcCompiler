#include "ast_constant.hpp"

void IntConstant::EmitRISC(std::ostream &stream, Context &context, int destReg) const
{
    stream << "li " << context.getMnemonic(destReg) << "," << value_ << std::endl;;
}

void IntConstant::Print(std::ostream &stream) const
{
    stream << value_;
}

void FloatConstant::EmitRISC(std::ostream &stream, Context &context, int destReg) const {
    uint32_t intRepresentation = context.FloatToInt(value_);

    int tempReg = context.allocateReg(Specifier::_int);

    stream << "li " << context.getMnemonic(tempReg) << ", " << intRepresentation << std::endl;
    stream << "sw " << context.getMnemonic(tempReg) << ", 0(sp)" << std::endl;

    // Load the value from the stack into the destination floating-point register.
    stream << "flw " << context.getMnemonic(destReg) << ", 0(sp)" << std::endl;

    context.freeReg(tempReg);
}


void FloatConstant::Print(std::ostream &stream) const
{
    stream << value_;
}

void CharConstant::EmitRISC(std::ostream &stream, Context &context, int destReg) const {
    stream << "li " << context.getMnemonic(destReg) << ", " << static_cast<int>(value_[0]) << std::endl;
}


void CharConstant::Print(std::ostream &stream) const
{
    stream << value_;
}

void StringConstant::EmitRISC(std::ostream &stream, Context &context, int destReg) const {
    std::string string_label = context.makeLabel("string");

    // store the string in the data segment
    stream << ".data" << std::endl;
    stream << string_label << ":" << std::endl;
    stream<<".string"<<" "<<value_<<'"'<<std::endl;
    stream << ".text" << std::endl;
    stream << "lui x" << destReg << ", %hi(" << string_label << ")" << std::endl;
    stream << "addi x" << destReg << ", x" << destReg << ", %lo(" << string_label << ")" << std::endl;
}


void StringConstant::Print(std::ostream &stream) const
{
    stream << value_;
}
