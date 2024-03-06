#include "ast_declaration.hpp"

// two constructors - with + without initialisation
Declaration::Declaration(Specifier _type, DeclarationRoot* _declarator)
    : type(_type), init_declarator(_declarator) {}

Declaration::~Declaration() {
    delete init_declarator;
}

int Declaration::getSize() const {

    int array_size = init_declarator->getArraySize();
    if (array_size == -1) {
        return typeSizes.at(type);
    }

    return typeSizes.at(type) * array_size;
}

void Declaration::EmitRISC(std::ostream& os, Context& context, int destReg) const {

    std::string var_name = init_declarator->getIdentifier();
    if (init_declarator->getArraySize() != -1) {
        int array_size = init_declarator->getArraySize();
        context.addArray(var_name, type, array_size);

    } else if (init_declarator->isPointer()) {
        context.addVar(var_name, type, true);
    }

    else {
        context.addVar(var_name, type);
    }

    init_declarator->EmitRISC(os, context, destReg);
};

void Declaration::Print(std::ostream& stream) const {
    stream << "Declaration(";
    stream << ", ";
    if (init_declarator != nullptr) {
        init_declarator->Print(stream);
    }
    stream << ")";
}

ParameterDeclaration::ParameterDeclaration(Specifier _type, DeclarationRoot* _declarator)
    : type(_type), declarator(_declarator) {}

ParameterDeclaration::~ParameterDeclaration() {
    delete declarator;
}

int ParameterDeclaration::getSize() const {
    return typeSizes.at(type);
}

void ParameterDeclaration::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    std::string param_type = declarator->getIdentifier();
    bool is_pointer = declarator->isPointer();

    int param_offset = context.addParam(param_type, type, destReg, is_pointer);

    if (param_offset == 1) return;
    std::cerr << "param_offset: " << param_offset << std::endl;

    if (is_pointer || type == Specifier::_int) {
        context.storeInstruction(stream, Specifier::_int, 10 + destReg, param_offset);
    } else if ( type == Specifier::_float || type == Specifier::_double) {
        context.storeInstruction(stream, type, 42 + destReg, param_offset);
    } else {
        std::cerr << "ParamDeclaration: Invalid param type" << std::endl;
        exit(1);
    }
};

// void ParameterDeclaration::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
//     std::string param_name = declarator->getIdentifier();
//     bool is_pointer = declarator->isPointer();
//     std::string param_type = declarator->getIdentifier();

//     // Calculate the size and the stack offset for the parameter
//     int param_size = is_pointer ? 4 : typeSizes.at(type);
//     int param_offset = context.addParam(param_type, type, destReg, is_pointer);

//     // Register allocation should be handled by the context, and it should know which register
//     // corresponds to which parameter index. For simplicity, I'll assume a0-a7 and fa0-fa7.
//     std::string reg_prefix = (type == Specifier::_float || type == Specifier::_double) ? "fa" : "a";
//     std::string register_name = reg_prefix + std::to_string(destReg);

//     // Store the parameter from the register to its stack location.
//     // The register names and the exact instructions depend on the ABI.
//     if (is_pointer || type == Specifier::_int) {
//         stream << "sw " << register_name << ", " << param_offset << "(s0)" << std::endl;
//     } else if (type == Specifier::_float || type == Specifier::_double) {
//         stream << (type == Specifier::_float ? "fsw " : "fsd ") << register_name << ", " << param_offset << "(s0)" << std::endl;
//     } else {
//         std::cerr << "ParameterDeclaration::EmitRISC: Invalid parameter type for " << param_name << std::endl;
//         exit(1);
//     }

//     // Output debug information
//     std::cerr << "Parameter " << param_name << " of type " << context.specifierToString(type)
//               << (is_pointer ? " (pointer)" : "") << " allocated to register " << register_name
//               << " with stack offset " << param_offset << std::endl;
// }


void ParameterDeclaration::Print(std::ostream& stream) const {
    stream << "ParameterDeclaration(";
    stream << ", ";
    if (declarator != nullptr) {
        declarator->Print(stream);
    }
    stream << ")";
}
