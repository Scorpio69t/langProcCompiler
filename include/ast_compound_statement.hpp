#ifndef AST_COMPOUND_STATEMENT_HPP
#define AST_COMPOUND_STATEMENT_HPP

#include "ast_node.hpp"

class CompoundStatement: public StatementRoot
{
private:
    List_Ptr statement_list;
    List_Ptr declaration_list;

public:
    CompoundStatement(List_Ptr _statement_list, List_Ptr _declaration_list):statement_list(_statement_list), declaration_list(_declaration_list) {}

    virtual ~CompoundStatement();

    virtual int getSize() const override;
    virtual void EmitRISC(std::ostream& stream, Context& context, int destReg) const override;
    virtual void Print(std::ostream& stream) const override;
};

#endif
