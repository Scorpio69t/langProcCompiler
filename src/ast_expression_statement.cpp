#include "ast_expression_statement.hpp"
// Constructor
ExpressionStatement::ExpressionStatement(ExpressionRoot* _expression) : expression(_expression) {}
// Destructor
ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

void ExpressionStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    expression->EmitRISC(stream, context, destReg);
}

void ExpressionStatement::Print(std::ostream& stream) const {
    stream << "ExpressionStatement :";
    if (expression != nullptr) {
        expression->Print(stream);
    }
    stream << std::endl;
}
