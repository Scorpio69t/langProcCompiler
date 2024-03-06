#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include "ast_node.hpp"

#include "ast_node.hpp"

class Declaration
    : public DeclarationRoot
{
private:
    Specifier type;
    DeclarationRoot* init_declarator ;
public:
    Declaration(Specifier _type, DeclarationRoot* _declarator);

    ~Declaration();

    int getSize() const override;
    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

class ParameterDeclaration
    : public DeclarationRoot
{
private:
    Specifier type;
    DeclarationRoot* declarator;

public:
    ParameterDeclaration(Specifier _type, DeclarationRoot* _declarator);
    ~ParameterDeclaration();

    int getSize() const override;
    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

#endif
