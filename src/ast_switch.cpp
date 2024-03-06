#include "ast_switch.hpp"
#include <iostream>
#include <map>

SwitchStatement::SwitchStatement(ExpressionRoot* _condition, StatementRoot* _body)
    : condition(_condition), body(_body) {}

SwitchStatement::~SwitchStatement()
{
    delete condition;
    delete body;
}

int SwitchStatement::getSize() const
{
    return body->getSize();
}

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {

    std::string start_label = context.makeLabel(".SWITCH_START");
    std::string end_label = context.makeLabel(".SWITCH_END");
    context.addLabels(start_label, end_label);

    // evaluate expression into a reg
    int reg = context.allocateReg(Specifier::_int);
    context.useReg(reg);
    condition->EmitRISC(stream, context, reg);

    // label for next case
    std::string case_label = context.makeLabel(".CASE");
    context.updateStartLabel(case_label);

    // case statements
    body->EmitRISC(stream, context, reg);

    // last case will still need something to branch to
    stream << context.getCurrentLoopStart() << ":" << std::endl;

    stream << end_label << ":" << std::endl;
    context.freeReg(reg);
    context.popLabels();

}

void SwitchStatement::Print(std::ostream& stream) const
{
    stream << "SwitchStatement(";
    condition->Print(stream);
    stream << ", ";
    if (body == nullptr)
    {
        stream << "null";
    }
    else
    {
        body->Print(stream);
    }
    stream << ")";
}

