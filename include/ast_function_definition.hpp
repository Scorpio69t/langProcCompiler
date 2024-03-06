#ifndef AST_FUNCTION_DEFINITION_HPP
#define AST_FUNCTION_DEFINITION_HPP

#include "ast_node.hpp"

class FunctionDefinition : public Node
{
private:
    Specifier type_;
    DeclarationRoot* declaratorRoot_;
    StatementRoot* statementRoot_;

public:
    FunctionDefinition(Specifier _type, DeclarationRoot* _declaratorRoot, StatementRoot* _statementRoot);
    virtual ~FunctionDefinition();

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

class FunctionDeclarator : public DeclarationRoot
{
private:
    DeclarationRoot* declarator;
    List_Ptr parameters;

public:
    FunctionDeclarator(DeclarationRoot* _declarator, List_Ptr _parameters);
    virtual ~FunctionDeclarator();

    virtual const std::string& getIdentifier() const override;
    virtual int getSize() const override;

    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

#endif
