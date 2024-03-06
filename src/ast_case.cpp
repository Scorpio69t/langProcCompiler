#include "ast_case.hpp"

Case::Case(ExpressionRoot* _expression, StatementRoot* _statements)
    : expression(_expression), statements(_statements) {}

Case::~Case() {
    delete expression;
    delete statements;
}

int Case::getSize() const {
    return statements->getSize();
}

void Case::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    int reg = context.allocateReg(Specifier::_int);
    context.useReg(reg);

    // start of case
    stream << context.getCurrentLoopStart() << ":" << std::endl;

    // label for next case
    std::string next_case_label = context.makeLabel(".CASE");
    context.updateStartLabel(next_case_label);

    if(expression) {
        // evaluate expression into a reg
        expression->EmitRISC(stream, context, reg);
        // check if the case matches switch expression in destReg
        stream << "bne " << context.getMnemonic(destReg) << ", " << context.getMnemonic(reg) << ", " << next_case_label << std::endl;
    }

    statements->EmitRISC(stream, context, reg);

    context.freeReg(reg);

}

void Case::Print(std::ostream& stream) const {
    stream << "Case(";
    if (expression) {
        stream << "Expression: ";
        expression->Print(stream);
    } else {
        stream << "Default Case";
    }
    stream << ", Statements: ";
    statements->Print(stream);
    stream << ")";
}
