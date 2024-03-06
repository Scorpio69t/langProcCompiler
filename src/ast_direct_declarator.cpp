#include "ast_direct_declarator.hpp"

void DirectDeclarator::EmitRISC(std::ostream &stream, Context &context, int destReg) const
{
    identifier_->EmitRISC(stream, context, destReg);
    stream << ":" << std::endl;
}

void DirectDeclarator::Print(std::ostream &stream) const
{
    identifier_->Print(stream);
}
