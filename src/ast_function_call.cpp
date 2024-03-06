#include "ast_function_call.hpp"

FunctionCall::FunctionCall(ExpressionRoot* _identifier, List_Ptr _arguments)
    : identifier(_identifier), arguments(_arguments) {}

FunctionCall::~FunctionCall()
{
    delete identifier;
    for (auto& arg : *arguments)
    {
        delete arg;
    }
    delete arguments;
}

bool FunctionCall::isFuncCall() const
{
    return true;
}

Specifier FunctionCall::getType(Context& ctx) const
{
    auto it = ctx.functionReturnTypes.find(identifier->getIdentifier());
    // default to int if func call id not found - as defined by c90
    if (it == ctx.functionReturnTypes.end()) return Specifier::_int;
    return it->second;
}

void FunctionCall::EmitRISC(std::ostream& stream, Context& context, int destReg) const
{
    std::string func_name = identifier->getIdentifier();
    if (arguments == nullptr){
        std::cerr << "No arguments for function call" << std::endl;
    }
    else{
        int int_reg = 0;
        int float_reg = 0;

        for (auto arg : *arguments)
        {
            Specifier arg_type = dynamic_cast<const ExpressionRoot*>(arg)->getType(context);

            if (arg_type == Specifier::_int || arg_type == Specifier::_char)
            {
                arg->EmitRISC(stream, context, 10 + int_reg);
                int_reg++;
            }
            else if (arg_type == Specifier::_float || arg_type == Specifier::_double)
            {
                arg->EmitRISC(stream, context, 42 + float_reg);
                float_reg++;
            }
        }
    }

    stream << "jal " << func_name << std::endl;
    Specifier return_type = getType(context);

    if (return_type == Specifier::_int || return_type == Specifier::_char)
    {
        stream << "move " << context.getMnemonic(destReg) << ", a0" << std::endl;
    }
    else if (return_type == Specifier::_float || return_type == Specifier::_double)
    {
        stream << "mov.s " << context.getMnemonic(destReg) << ", fa0" << std::endl;
    }
    else{
        std::cerr << "Invalid return type" << std::endl;
        exit(1);
    }
}

void FunctionCall::Print(std::ostream& stream) const
{
    identifier->Print(stream);
    stream << "(";
    if (arguments != nullptr)
    {
        for (auto arg : *arguments)
        {
            arg->Print(stream);
            stream << ", ";
        }
        stream << "\b\b";
    }
    stream << ")";
}
