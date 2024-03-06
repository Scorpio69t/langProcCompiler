#include "ast_type_specifier.hpp"

void TypeSpecifier::EmitRISC(std::ostream &stream, Context &context, int destReg) const {}

void TypeSpecifier::Print(std::ostream &stream) const
{
    stream << type_;
}
