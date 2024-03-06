#ifndef AST_DECLARATOR_HPP
#define AST_DECLARATOR_HPP

#include "ast_node.hpp"

class Declarator: public DeclarationRoot
{
private:
    std::string identifier;
public:
    Declarator(const std::string& _identifier);

    const std::string& getIdentifier() const override;
    void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};

class InitDeclarator
    : public DeclarationRoot
{
private:
    DeclarationRoot* declarator;
    ExpressionRoot* initialiser;
public:
    InitDeclarator(DeclarationRoot* _declarator, ExpressionRoot* _initialiser);

    ~InitDeclarator();

    bool isPointer() const override;
    int getArraySize() const override;
    const std::string& getIdentifier() const override;
    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
};



#endif
