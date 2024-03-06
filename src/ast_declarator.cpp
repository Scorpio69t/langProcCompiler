#include "ast_declarator.hpp"

Declarator::Declarator(const std::string& _identifier) : identifier(_identifier) {}

void Declarator::EmitRISC(std::ostream& os, Context& context, int destReg) const { }

const std::string& Declarator::getIdentifier() const {
    return identifier;
}

void Declarator::Print(std::ostream& stream) const {
    stream << "Declarator: " << identifier;
}

InitDeclarator::InitDeclarator(DeclarationRoot* _declarator, ExpressionRoot* _initialiser)
    : declarator(_declarator), initialiser(_initialiser) {}

InitDeclarator::~InitDeclarator() {
    delete declarator;
    delete initialiser;
}

bool InitDeclarator::isPointer() const {
    return declarator->isPointer();
}

int InitDeclarator::getArraySize() const {
    return declarator->getArraySize();
}

const std::string& InitDeclarator::getIdentifier() const {
    return declarator->getIdentifier();
}

void InitDeclarator::EmitRISC(std::ostream& os, Context& context, int destReg) const {

    if (getArraySize() != -1) {
        // array initialisation
        std::string array_name = declarator->getIdentifier();
        int array_address_reg = context.allocateReg(Specifier::_int);
        context.useReg(array_address_reg);

        // load the memory address of element 0 into the reg
        int base_offset = context.getVarOffset(array_name);
        os << "addi " << context.getMnemonic(array_address_reg) << ", " << "s0, " << base_offset << std::endl;
        initialiser->EmitRISC(os, context, array_address_reg);

        context.freeReg(array_address_reg);

    } else {
        // variable initialisation

        std::string var_name = declarator->getIdentifier();
        Specifier type = context.getVarType(var_name);

        // Pointers themselves use standard int regs + operations
        if(isPointer()) type = Specifier::_int;

        int init_reg = context.allocateReg(type);
        context.useReg(init_reg);
        // evaluate initialiser expression e.g. (3+4) = 7
        initialiser->EmitRISC(os, context, init_reg);

        int var_offset = context.getVarOffset(var_name);

        context.storeInstruction(os, type, init_reg, var_offset);

        context.freeReg(init_reg);
    }

}

void InitDeclarator::Print(std::ostream& stream) const {
    declarator->Print(stream);
    stream << " = ";
    initialiser->Print(stream);
}
