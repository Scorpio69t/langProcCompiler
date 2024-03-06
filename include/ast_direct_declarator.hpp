#ifndef AST_DIRECT_DECLARATOR_HPP
#define AST_DIRECT_DECLARATOR_HPP

#include "ast_node.hpp"

class DirectDeclarator : public Node{
private:
    Node *identifier_;

public:
    DirectDeclarator(Node *identifier) : identifier_(identifier){};
    virtual ~DirectDeclarator(){
        delete identifier_;
    };
    virtual void EmitRISC(std::ostream &stream, Context &context, int destReg) const override;
    virtual void Print(std::ostream &stream) const override;
};

#endif
