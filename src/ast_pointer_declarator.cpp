#include "ast_pointer_declarator.hpp"


PointerDeclarator::PointerDeclarator(DeclarationRoot* _identifier)
    : identifier(_identifier) {}

PointerDeclarator::~PointerDeclarator() {
    delete identifier;
}

const std::string& PointerDeclarator::getIdentifier() const {
    return identifier->getIdentifier();
}

void PointerDeclarator::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

}

bool PointerDeclarator::isPointer() const {
    return true;
}

void PointerDeclarator::Print(std::ostream& stream) const {
    stream << "PointerDeclarator: ";
    identifier->Print(stream);
}
