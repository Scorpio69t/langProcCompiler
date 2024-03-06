#ifndef AST_FUNCTION_CALL_HPP
#define AST_FUNCTION_CALL_HPP

#include "ast_node.hpp"

class FunctionCall : public ExpressionRoot
{
private:
    ExpressionRoot* identifier;
    List_Ptr arguments;

public:
    FunctionCall(ExpressionRoot* _identifier, List_Ptr _arguments);
    ~FunctionCall();

    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;

    bool isFuncCall() const override;
    Specifier getType(Context& ctx) const override;
};

#endif
