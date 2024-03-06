#include "ast_assignment.hpp"
#include "ast_array.hpp"

Assignment::Assignment(ExpressionRoot* _identifier, ExpressionRoot* _expression) : identifier(_identifier), expression(_expression) {}

Assignment::~Assignment()
{
    delete identifier;
    delete expression;
}

void Assignment::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    std::string var_name = identifier->getIdentifier();
    Variable var = context.getVar(var_name);

    Specifier var_type = var.type;
    int var_size = typeSizes.at(var_type);

    int right_reg = context.allocateReg(var_type);

    context.useReg(right_reg);
    expression->EmitRISC(stream, context, right_reg);

    if(identifier->isDerefPointer()) {
        int address_reg = context.allocateReg(Specifier::_int);
        context.useReg(address_reg);

        context.loadInstruction(stream, Specifier::_int, address_reg, var.offset);
        context.storeInstruction(stream, var_type, right_reg, 0, address_reg);

        context.freeReg(address_reg);

    } else if (var.is_pointer) {
        context.storeInstruction(stream, Specifier::_int, right_reg, var.offset);

    } else if (!identifier->isArray()) {
        context.storeInstruction(stream, var_type, right_reg, var.offset);

    } else {

        int index_reg = context.allocateReg(Specifier::_int);
        context.useReg(index_reg);
        const ArrayIndex* index_expression = dynamic_cast<const ArrayIndex*>(identifier);
        index_expression->getIndex()->EmitRISC(stream, context, index_reg);

        stream << "slli " << context.getMnemonic(index_reg) << ", " << context.getMnemonic(index_reg) << ", " << log2(var_size) << std::endl;
        stream << "sub " << context.getMnemonic(index_reg) << ", s0, " << context.getMnemonic(index_reg) << std::endl;
        stream << "addi " << context.getMnemonic(index_reg) << ", " << context.getMnemonic(index_reg) << ", " << var.offset << std::endl;

        context.storeInstruction(stream, var_type, right_reg, 0, index_reg);

        context.freeReg(index_reg);
    }

    context.freeReg(right_reg);

}


void Assignment::Print(std::ostream& stream) const {
    stream << "(";
    identifier->Print(stream);
    stream << " = ";
    expression->Print(stream);
    stream << ")";
}

Specifier Assignment::getType(Context& ctx) const {
    return identifier->getType(ctx);
}
