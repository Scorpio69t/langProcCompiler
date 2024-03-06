#include "ast_if_else.hpp"
#include <algorithm>

IfElseStatement::IfElseStatement(ExpressionRoot* _condition, StatementRoot* _ifBody, StatementRoot* _elseBody)
    : condition(_condition), ifBody(_ifBody), elseBody(_elseBody) {}

IfElseStatement::~IfElseStatement()
{
    delete condition;
    delete ifBody;
    if (elseBody != nullptr) {
        delete elseBody;
    }
}

int IfElseStatement::getSize() const
{
    int size = ifBody->getSize();

    // If there is an 'else' body, add its size.
    if (elseBody != nullptr) {
        size += elseBody->getSize();
    }

    return size;
}


void IfElseStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const
{
    // Create unique labels for the true and false branches of the if-else statement
    std::string trueLabel = context.makeLabel("if_true");
    std::string falseLabel = context.makeLabel("if_false");
    std::string endLabel = context.makeLabel("if_end");

    // Evaluate the condition and jump to the true or false branch
    int cond_reg = context.allocateReg(Specifier::_int);
    condition->EmitRISC(stream, context, cond_reg);
    stream << "bnez " << context.getMnemonic(cond_reg) << ", " << trueLabel << std::endl;

    // If condition is false, jump to the falseLabel
    stream << "j " << falseLabel << std::endl;

    // True branch statements
    stream << trueLabel << ":" << std::endl;
    ifBody->EmitRISC(stream, context, destReg);
    // After executing the true branch, jump to the endLabel
    stream << "j " << endLabel << std::endl;

    // False branch statements
    stream << falseLabel << ":" << std::endl;
    if (elseBody != nullptr) {
        elseBody->EmitRISC(stream, context, destReg);
    }
    // Label for the end of the if-else statement
    stream << endLabel << ":" << std::endl;

    // Free the register used for the condition
    context.freeReg(cond_reg);
}

void IfElseStatement::Print(std::ostream& stream) const
{
    stream << "IfElseStatement(";
    condition->Print(stream);
    stream << ", ";
    ifBody->Print(stream);
    stream << ", ";
    if (elseBody != nullptr) {
        elseBody->Print(stream);
    }
    else {
        stream << "nullptr";
    }
}

