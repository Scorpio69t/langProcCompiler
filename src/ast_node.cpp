#include "ast_node.hpp"

Node::~Node()
{
    for (auto branch : branches_)
    {
        delete branch;
    }
}

void RootNode::EmitRISC(std::ostream &stream, Context &context, int destReg) const
{
    stream << ".text" << std::endl;
    for (auto node : *list_)
    {
        node->EmitRISC(stream, context, destReg);
    }
}

RootNode::~RootNode()
{
    if (list_)
    {
        for (auto node : *list_)
        {
            delete node;
        }
        delete list_;
    }
}

void RootNode::Print(std::ostream &stream) const
{
    if (list_)
    {
        for (auto node : *list_)
        {
            if (node == nullptr)
            {
                continue;
            }
            node->Print(stream);
        }
    }
}

List_Ptr initList(Node_Ptr NodePtr) {
    List_Ptr ListPtr =  new List();
    ListPtr->push_back(NodePtr);
    return ListPtr;
}

List_Ptr concatList(List_Ptr ListPtr, Node_Ptr NodePtr) {
    ListPtr->push_back(NodePtr);
    return ListPtr;
}

int ExpressionRoot::getValue() const
{
    return 0;
}

const std::string &ExpressionRoot::getIdentifier() const
{
    static const std::string empty_string = "";
    return empty_string;
}

Specifier ExpressionRoot::getType(Context &context) const
{
    //only consider int now
    return Specifier::_int;
}

void ExpressionRoot::print(std::ostream &stream) const
{
    stream << "ExpressionRoot: ";
}

bool ExpressionRoot::isArray() const
{
    return false;
}

bool ExpressionRoot::isDerefPointer() const
{
    return false;
}

bool ExpressionRoot::isFuncCall() const
{
    return false;
}


int DeclarationRoot::getSize() const
{
    return 0;
}

const std::string &DeclarationRoot::getIdentifier() const
{
    static const std::string empty_string = "";
    return empty_string;
}

Specifier DeclarationRoot::getType(Context &context) const
{
    //only consider int now
    return Specifier::_int;
}

void DeclarationRoot::print(std::ostream &stream) const
{
    stream << "DeclarationRoot: ";
}

bool DeclarationRoot::isPointer() const
{
    return false;
}

int DeclarationRoot::getArraySize() const
{
    return 0;
}


int StatementRoot::getSize() const
{
    return 0;
}

void StatementRoot::print(std::ostream &stream) const
{
    stream << "StatementRoot: ";
}

