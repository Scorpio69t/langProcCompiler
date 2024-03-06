#include "ast_for_statement.hpp"

ForStatement::ForStatement(StatementRoot* _init, StatementRoot* _cond, ExpressionRoot* _incr, StatementRoot* _stat)
    : initializer(_init), condition(_cond), increment(_incr), statement(_stat) {}

// Destructor
ForStatement::~ForStatement()
{
    delete initializer;
    delete statement;
    delete increment;
}

// Returns the size of the for loop statement
int ForStatement::getSize() const
{
    return statement->getSize();
}

void ForStatement::EmitRISC(std::ostream &stream, Context &context, int destReg) const {
    std::string start_label = context.makeLabel("for_start");
    std::string end_label = context.makeLabel("for_end");
    std::string continue_label = context.makeLabel("for_continue");

    initializer->EmitRISC(stream, context, destReg);

    stream << start_label << ":" << std::endl;

    int cond_reg = context.allocateReg(Specifier::_int);
    condition->EmitRISC(stream, context, cond_reg);
    stream << "beqz " << context.getMnemonic(cond_reg) << ", " << end_label << std::endl;

    statement->EmitRISC(stream, context, destReg);

    stream << continue_label << ": " << std::endl;
    if (increment != nullptr) {
        increment->EmitRISC(stream, context, destReg);
    }

    stream << "j " << start_label << std::endl;

    stream << end_label << ": " << std::endl;

    context.freeReg(cond_reg);
}

// Print out loop statement
void ForStatement::Print(std::ostream &stream) const{
    stream << "for (";
    if (initializer) {
        initializer->Print(stream);
    }
    stream << "; ";
    if (condition) {
        condition->Print(stream);
    }
    stream << "; ";
    if (increment) {
        increment->Print(stream);
    }
    stream << ") ";
    statement->Print(stream);
}
