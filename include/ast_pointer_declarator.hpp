#ifndef AST_POINTER_DECLARATOR_HPP
#define AST_POINTER_DECLARATOR_HPP

#include "ast_node.hpp"

class PointerDeclarator: public DeclarationRoot{
private:
    DeclarationRoot* identifier;
public:
    PointerDeclarator(DeclarationRoot* _identifier);
    virtual ~PointerDeclarator();

    virtual bool isPointer() const override;
    virtual const std::string& getIdentifier() const override;
    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

#endif
