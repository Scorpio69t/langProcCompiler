#include "ast_context.hpp"

void Scope::addLocalVar(const std::string& name, Specifier type, int offset, bool isPointer) {
    bindings[name] = Variable(type, offset, isPointer);
}

Variable Scope::getLocalVar(const std::string& name) const {
    auto it = bindings.find(name);
    if(it == bindings.end()){
        return Variable(Specifier::INVALID_TYPE, -1);
    }else{
        return it->second;
    }
}


//register handling
int Context::allocateReg(Specifier type) {
    int start = 5;
    int end = 32;
    if (type == Specifier::_float || type == Specifier::_double) {
        start = 37;
        end = 64;
    }
    for (int i = start; i < end; i++) {
        if (!usedRegs[i]) {
            useReg(i);
            return i;
        }
    }
    return -1;
}

std::string Context::getMnemonic(int i) const {
    static const std::array<std::string, 64> regNames = {
        "zero", "ra", "sp", "gp", "tp",
        "t0", "t1", "t2",
        "s0", "s1",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
        "t3", "t4", "t5", "t6",

        "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",
        "fs0", "fs1",
        "fa0", "fa1",
        "fa2", "fa3", "fa4", "fa5", "fa6", "fa7",
        "fs2", "fs3", "fs4", "fs5", "fs6", "fs7","fs8", "fs9", "fs10", "fs11",
        "ft8", "ft9", "ft10", "ft11"
    };
    return regNames[i];
}


//variables handling
Variable Context::getVar(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        Variable var = it->getLocalVar(name);
        if (var.offset != -1) return var;
    }
    return Variable(Specifier::INVALID_TYPE, -1, false);
}

int Context::getVarOffset(const std::string& name) const {
    Variable var = getVar(name);
    return var.offset;
}

Specifier Context::getVarType(const std::string& name) const {
    Variable var = getVar(name);
    return var.type;
}

int Context::addVar(const std::string& name, Specifier type, bool is_pointer) {
    int var_size = typeSizes.at(type);
    if (is_pointer == true){
        var_size = 4;
    }
    local_var_offset -= var_size;
    scopes.back().addLocalVar(name, type, local_var_offset, is_pointer);
    return local_var_offset;
}

int Context::addParam(const std::string& name, Specifier type, int param_index, bool is_pointer) {
    int param_size = typeSizes.at(type);
    if (is_pointer == true){
        param_size = 4;
    }

    if (param_index < 8) {
        param_offset -= param_size;
        scopes.back().addLocalVar(name, type, param_offset, is_pointer);
        return param_offset;
    }

    std::cerr << "Adding param: " << name << " with type: " << specifierToString(type)
          << " and size: " << param_size << " at offset: " << param_offset << std::endl;


    return param_offset;
}



//scope handling
void Context::newScope() {
    scopes.emplace_back();
}

void Context::popScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

void Context::resetOffsets() {
    local_var_offset = -unit;
    param_offset = -2 * unit;
}

void Context::saveFuncReturnType(const std::string& func_name, Specifier return_type) {
    functionReturnTypes[func_name] = return_type;
    current_func_type = return_type;
}


//loop handling
void Context::addLabels(const std::string& start_label, const std::string& end_label) {
    loopLabels.push_back({start_label, end_label});
}

void Context::popLabels() {
    if (!loopLabels.empty()) {
        loopLabels.pop_back();
    }
}

void Context::updateStartLabel(const std::string& new_start_label) {
    if (!loopLabels.empty()) {
        loopLabels.back().start_label = new_start_label;
    }
}

std::string Context::getCurrentLoopStart() const {
    if (!loopLabels.empty()) {
        return loopLabels.back().start_label;
    }
    return "loop lable empty";
}

std::string Context::getCurrentLoopEnd() const {
    for (auto it = loopLabels.rbegin(); it != loopLabels.rend(); ++it) {
        if (!it->end_label.empty()) {
            return it->end_label;
        }
    }
    return "loop end empty";
}

int Context::calculateStackSize(int totalLocalVarBytes, int totalParamBytes) {
    int stack_size = unit;

    totalParamBytes += 4;

    if (totalLocalVarBytes > 0) {
        stack_size += unit * std::ceil(static_cast<double>(totalLocalVarBytes) / unit);
    }

    param_offset = -stack_size;

    if (totalParamBytes > 0) {
        stack_size += unit * std::ceil(static_cast<double>(totalParamBytes) / unit);
    }

    return stack_size;
}

//utility
void Context::loadInstruction(std::ostream& os, Specifier type, int dest_reg, int offset, int address_reg ) const {
    if(type == Specifier::_int){
        os << "lw " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else if(type == Specifier::_float){
        os << "flw " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else if(type == Specifier::_double){
        os << "fld " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else if(type == Specifier::_char){
        os << "lb " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else{
        std::cerr << "unknown type of instruction" << std::endl;
        exit(1);
    }
}

void Context::storeInstruction(std::ostream& os, Specifier type, int dest_reg, int offset, int address_reg ) const {
    if(type == Specifier::_int){
        os << "sw " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else if(type == Specifier::_float){
        os << "fsw " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else if(type == Specifier::_double){
        os << "fsd " << getMnemonic(dest_reg) << ", " << offset << "(" << getMnemonic(address_reg) << ") " << std::endl;
    }else{
        std::cerr << "Store Instruction with unknown type" << std::endl;
        exit(1);
    }
}

std::string Context::specifierToString(Specifier specifier) const {
    switch (specifier) {
        case Specifier::_int:       return "int";
        case Specifier::_char:      return "char";
        case Specifier::_void:      return "void";
        case Specifier::_float:     return "float";
        case Specifier::_double:    return "double";
        case Specifier::_unsigned:  return "unsigned";
        default:                    return "INVALID_TYPE";
    }
}

void Context::printVariableInfo(const std::string& func_name, const Variable& var) const {
    std::cerr << "Variable:" << func_name << "\n";
    std::cerr << "  Type: " << specifierToString(var.type) << "\n";
    std::cerr << "  Offset: " << var.offset << "\n";
    std::cerr << "  Is Pointer: " << (var.is_pointer ? "Yes" : "No") << "\n";
}



/*---TBC---*/
bool Context::getIsPointer(const std::string& name) const {
    Variable var = getVar(name);
    return var.is_pointer;
}


void Context::addArray(const std::string& array_name, Specifier type, int array_size) {
    int var_size = typeSizes.at(type);
    int total_array_size = array_size * var_size;

    scopes.back().addLocalVar(array_name, type, local_var_offset - var_size);

    local_var_offset -= total_array_size;
}

