#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>

#include "ast_context.hpp"

class Node;

typedef const Node* Node_Ptr;
typedef std::vector<Node_Ptr> List;
typedef List* List_Ptr;

class Node
{
protected:
    std::vector<Node *> branches_;

public:
    Node(){};
    virtual ~Node();
    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg) const = 0;
    virtual void Print(std::ostream &stream) const = 0;


};

// Represents a list of nodes.
class RootNode : public Node
{
private:
    List_Ptr list_;
public:
    RootNode(List_Ptr list) : list_(list) {}
    ~RootNode();

    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    virtual void Print(std::ostream &stream) const override;
};

List_Ptr initList(Node_Ptr NodePtr);

List_Ptr concatList(List_Ptr ListPtr, Node_Ptr NodePtr);


class ExpressionRoot
    : public Node
{
public:
    virtual ~ExpressionRoot() = default;

    virtual int getValue() const;
    virtual const std::string& getIdentifier() const;
    virtual Specifier getType(Context& context) const;
    virtual void print(std::ostream& stream) const;

    virtual bool isArray() const;
    virtual bool isDerefPointer() const;
    virtual bool isFuncCall() const;
};


class DeclarationRoot
    : public Node
{
public:
    virtual ~DeclarationRoot() = default;

    virtual int getSize() const;
    virtual const std::string& getIdentifier() const;
    virtual Specifier getType(Context& context) const;
    virtual void print(std::ostream& stream) const;

    virtual bool isPointer() const;
    virtual int getArraySize() const;

};

class StatementRoot
    : public Node
{
public:
    virtual ~StatementRoot() = default;
    virtual int getSize() const;
    virtual void print(std::ostream& stream) const;
};

#endif
