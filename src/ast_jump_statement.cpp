#include "ast_jump_statement.hpp"

ReturnStatement::ReturnStatement(ExpressionRoot* _expression) : expression(_expression) {}

ReturnStatement::~ReturnStatement() {
    if (expression){
        delete expression;
    }
}


void ReturnStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    if (expression) {
        int reg;
        if (context.current_func_type == Specifier::_float || context.current_func_type == Specifier::_double) {
            reg=47;
        }else{
            reg=15;
        }
        expression->EmitRISC(stream, context, reg);
    }
    stream << "j " << context.ret_label << std::endl;
}

void ReturnStatement::Print(std::ostream& stream) const {
    stream << "return ";
    if (expression) {
        expression->Print(stream);
    }
    stream << ";" << std::endl;
}

ContinueStatement::ContinueStatement() {}

ContinueStatement::~ContinueStatement() {}

void ContinueStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    // Jump to start of the current loop
    stream << "j " << context.getCurrentLoopStart() << std::endl;
}

void ContinueStatement::Print(std::ostream& stream) const {
    stream << "continue;" << std::endl;
}

BreakStatement::BreakStatement() {}

BreakStatement::~BreakStatement() {}

void BreakStatement::EmitRISC(std::ostream& stream, Context& context, int destReg) const {
    stream << "j " << context.getCurrentLoopEnd() << std::endl;
}


void BreakStatement::Print(std::ostream& stream) const {
    stream << "break;" << std::endl;
}
