#include "ast_compound_statement.hpp"

CompoundStatement::~CompoundStatement() {
    if (statement_list) {
        for (auto node: *statement_list) {
            delete node;
        }
        delete statement_list;
        statement_list = nullptr;
    }
    if (declaration_list) {
        for (auto node: *declaration_list) {
            delete node;
        }
        delete declaration_list;
        declaration_list = nullptr;
    }
}

// calculate size of CompoundStatement
int CompoundStatement::getSize() const {
    int total_size = 0;

    if (declaration_list) {
        for (auto decl : *declaration_list) {
            if (decl == nullptr) {
                std::cerr << "Null pointer in statement list.\n";
            }else{
                total_size += dynamic_cast<const DeclarationRoot*>(decl)->getSize();
            }
        }
    }

    if (statement_list) {
        for (auto statement : *statement_list) {
            if (statement == nullptr) {
                std::cerr << "Null pointer in statement list.\n";
            } else {
                total_size += dynamic_cast<const StatementRoot*>(statement)->getSize();
            }
        }
    }

    return total_size;
}

// generate RISC
void CompoundStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    if (context.is_function_def) {
        context.is_function_def = 0;
    } else {
        context.newScope();
    }

    if (declaration_list) {
        for (auto node: *declaration_list) {
            node->EmitRISC(stream, context, destReg);
        }
    }

    if (statement_list) {
        for (auto node: *statement_list) {
            node->EmitRISC(stream, context, destReg);
        }
    }

    //context.debugScope();

    context.popScope();
}

// print CompoundStatement
void CompoundStatement::Print(std::ostream& stream) const {
    stream << "Compound Statement:\n";

    if (declaration_list) {
        stream << "Declarations:\n";
        for (auto node : *declaration_list) {
            node->Print(stream);
        }
    } else {
        std::cerr << "Declarations list is empty.\n";
    }

    if (statement_list) {
        stream << "Statements:\n";
        for (auto node : *statement_list) {
            node->Print(stream);
        }
    } else {
        std::cerr << "Statements list is empty.\n";
    }
}
