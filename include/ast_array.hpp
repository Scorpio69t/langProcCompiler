#ifndef AST_ARRAY_HPP
#define AST_ARRAY_HPP

#include "ast_node.hpp"

class ArrayDeclarator
    : public DeclarationRoot
{
public:
    ArrayDeclarator(DeclarationRoot* _declarator, ExpressionRoot* _array_size);
    ~ArrayDeclarator();

    int getArraySize() const override;
    const std::string& getIdentifier() const override;
    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;

private:
    DeclarationRoot* declarator;
    ExpressionRoot* array_size;
};

class ArrayIndex
    : public ExpressionRoot
{
public:
    ArrayIndex(ExpressionRoot* _array_id, ExpressionRoot* _index);
    ~ArrayIndex();

    bool isArray() const override;
    const std::string& getIdentifier() const override;
    const ExpressionRoot* getIndex() const;
    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;

private:
    ExpressionRoot* array_id;
    ExpressionRoot* index;
};

class ArrayInitialiser
    : public ExpressionRoot
{
public:
    ArrayInitialiser(List_Ptr _initialiser_list);
    ~ArrayInitialiser();

    void EmitRISC(std::ostream& os, Context& context, int destReg) const override;
    void Print(std::ostream& stream) const override;
private:
    List_Ptr initialiser_list;
};

#endif
