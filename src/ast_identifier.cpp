#include "ast_identifier.hpp"

void Identifier::EmitRISC(std::ostream &stream, Context &context, int destReg) const
{
    Variable var = context.getVar(identifier_);
    context.loadInstruction(stream, var.type, destReg, var.offset);
}

void Identifier::Print(std::ostream &stream) const
{
    std::cout << "Identifier: " << identifier_ << std::endl;
    stream << identifier_;
};

Specifier Identifier::getType(Context &ctx) const
{
    Specifier type = ctx.getVarType(identifier_);

    if (type == Specifier::INVALID_TYPE) {
        std::cerr << "Invalid Data Type" << std::endl;
    }

    return ctx.getVarType(identifier_);
}

const std::string &Identifier::getIdentifier() const
{
    return identifier_;
}
