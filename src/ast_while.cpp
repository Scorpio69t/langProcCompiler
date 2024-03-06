#include "ast_while.hpp"

WhileStatement::WhileStatement(ExpressionRoot* condition, StatementRoot* body)
    : condition(condition), body(body) {}

WhileStatement::~WhileStatement()
{
    delete condition;
    delete body;
}

int WhileStatement::getSize() const
{
    return body->getSize();
}

void WhileStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    std::string loopStartLabel = context.makeLabel("loop_start");
    std::string loopEndLabel = context.makeLabel("loop_end");

    // Jump to loop condition check
    stream << "jal x0, " << loopStartLabel << std::endl;
    stream << loopStartLabel << ":" << std::endl;

    // Evaluate the loop condition
    condition->EmitRISC(stream, context, destReg);

    // If condition is false, exit loop
    stream << "beqz " << context.getMnemonic(destReg) << ", " << loopEndLabel << std::endl;

    // Loop body
    body->EmitRISC(stream, context, destReg);
    stream << "j " << loopStartLabel << std::endl; // Jump back to the start of the loop to re-evaluate the condition
    stream << loopEndLabel << ":" << std::endl;
}


void WhileStatement::Print(std::ostream& stream) const
{
    stream << "while (";
    condition->Print(stream);
    stream << ") ";
    body->Print(stream);
}
