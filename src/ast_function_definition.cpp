#include "ast_function_definition.hpp"

FunctionDefinition::FunctionDefinition(Specifier _type, DeclarationRoot* _declaratorRoot, StatementRoot* _statementRoot)
    : type_(_type), declaratorRoot_(_declaratorRoot), statementRoot_(_statementRoot) {}

FunctionDefinition::~FunctionDefinition() {
    if (declaratorRoot_ != nullptr) {
        delete declaratorRoot_;
    }
    if (statementRoot_ != nullptr) {
        delete statementRoot_;
    }
}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    std::string functionName = declaratorRoot_->getIdentifier();

    context.saveFuncReturnType(functionName, type_);
    if (type_ == Specifier::_float || type_ == Specifier::_double) {
        destReg = 42;
    }

    stream << ".globl " << functionName << std::endl;
    stream << functionName << ":" << std::endl;

    context.ret_label = context.makeLabel(".FUNC_RETURN");
    context.resetOffsets();

    int paramListSize = declaratorRoot_->getSize();
    int statementSize = statementRoot_->getSize();

    std::cerr << "Param list size: " << paramListSize << std::endl;
    std::cerr << "Statement list size: " << statementSize << std::endl;

    int stackSize = context.calculateStackSize(statementSize, paramListSize);

    // allocate stack frame and save registers
    stream << "addi sp,sp," << -1 * stackSize << std::endl;
    stream << "sw ra, "<< stackSize - 4 << "(sp)" << std::endl;
    stream << "sw s0, " << stackSize - 8 << "(sp)" << std::endl;
    stream << "sw s1, " << stackSize - 12 << "(sp)" << std::endl;
    stream << "addi s0,sp," << stackSize << std::endl;

    declaratorRoot_->EmitRISC(stream, context, destReg);
    statementRoot_->EmitRISC(stream, context, destReg);

    stream << context.ret_label << ":" << std::endl;
    if( type_ == Specifier::_int || type_ == Specifier::_char) {
        stream << "mv a0, a5" << std::endl;
    }else if(type_ == Specifier::_double || type_ ==Specifier::_float){
        stream << "fmv.d fa0, fa5" << std::endl;
    }else{
        std::cerr << "invalid function" << std::endl;
        exit(1);
    }

    // restore saved registers
    stream << "lw ra, "<< stackSize - 4 << "(sp)" << std::endl;
    stream << "lw s0, " << stackSize - 8 << "(sp)" << std::endl;
    stream << "lw s1, " << stackSize - 12 << "(sp)" << std::endl;
    stream << "addi sp,sp, " << stackSize << std::endl;
    stream << "jr ra \n" << std::endl;
}

// implements printing of the function definition for debugging
void FunctionDefinition::Print(std::ostream &stream) const {
    stream << "FunctionDefinition: ";
    stream << " ";

    declaratorRoot_->Print(stream);
    stream << "() {" << std::endl;

    if (statementRoot_ != nullptr) {
        statementRoot_->Print(stream);
    }
    stream << "}" << std::endl;
}


FunctionDeclarator::FunctionDeclarator(DeclarationRoot* _declarator, List_Ptr _parameters)
    : declarator(_declarator), parameters(_parameters) {}

FunctionDeclarator::~FunctionDeclarator() {
    delete declarator;
    if (parameters != nullptr){
        for (auto param : *parameters) {
            delete param;
        }
        delete parameters;
    }
}

const std::string& FunctionDeclarator::getIdentifier() const {
    return declarator->getIdentifier();
}

int FunctionDeclarator::getSize() const {
    if (parameters == nullptr){
        return 0;
    }
    else {
        int size = 0;
        int count = 0;
        for (auto param : *parameters) {
            // if (count >= 8) {
            //     break; // Exit the loop if we have processed 8 parameters.
            // }
            size += dynamic_cast<const DeclarationRoot*>(param)->getSize();
            count++;
        }
        return size;
    }
}


void FunctionDeclarator::EmitRISC(std::ostream& stream, Context& context, int destReg) const{
    context.newScope();
    context.is_function_def = true;
    std::cerr << "scope is created for the function" << std::endl;


    if (parameters != nullptr){
        int int_param_count = 0;
        int float_param_count = 0;
        int double_param_count = 0;
        int char_param_count = 0;

        for (auto param : *parameters) {
            // if (int_param_count >= 8 && float_param_count >= 8 && char_param_count >= 8) {
            //     break; // Exit the loop if we have processed 8 parameters.
            // }

            Specifier param_type = dynamic_cast<const DeclarationRoot*>(param)->getType(context);

            if (dynamic_cast<const DeclarationRoot*>(param)->isPointer()) {
                param_type = Specifier::_int;
            }

            switch (param_type) {
                case Specifier::_int:
                    param->EmitRISC(stream, context, int_param_count);
                    int_param_count++;
                    break;
                case Specifier::_float:
                    param->EmitRISC(stream, context, float_param_count);
                    float_param_count++;
                    break;
                case Specifier::_double:
                    param->EmitRISC(stream, context, double_param_count);
                    double_param_count++;
                    break;
                case Specifier::_char:
                    param->EmitRISC(stream, context, char_param_count);
                    char_param_count++;
                    break;
                default:
                    std::cerr << "FunctionDeclarator: Invalid parameter type" << std::endl;
                    exit(1);
            }
        }
    }
    else {
        std::cerr << "FunctionDeclarator: No parameters" << std::endl;
    }
}

void FunctionDeclarator::Print(std::ostream& stream) const {
    stream << "FunctionDeclarator: ";
    declarator->Print(stream);
    stream << "(";
    if (parameters != nullptr) {
        for (auto param : *parameters) {
            param->Print(stream);
            stream << ", ";
        }
    }
    stream << ")";
}
