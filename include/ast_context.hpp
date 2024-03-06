#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <ostream>

constexpr int unit = 16;

enum class Specifier
{
    _int,
    _char,
    _void,
    _float,
    _double,
    _unsigned,
    INVALID_TYPE = -1,
};


const std::unordered_map<Specifier, int> typeSizes = {
    {Specifier::_int,       4},
    {Specifier::_char,      1},
    {Specifier::_void,      0},
    {Specifier::_float,     4},
    {Specifier::_double,    8},
    {Specifier::_unsigned,  4}
};

class Variable
{

    public:
    Specifier type;
    int offset;
    /*---TBC---*/
    bool is_pointer;


    Variable(): type(Specifier::_int), offset(0), is_pointer(false) {}

    Variable(Specifier _type, int _offset, bool _is_pointer = false)
        : type(_type), offset(_offset), is_pointer(_is_pointer) {}

};

class Scope
{
    public:
    std::unordered_map<std::string, Variable> bindings;

    void addLocalVar(const std::string& name, Specifier type, int offset, bool isPointer = false);

    Variable getLocalVar(const std::string& name) const;

};

class LoopLabel {
    public:
    std::string start_label;
    std::string end_label;
};


class Context
{
    /* TODO decide what goes inside here */
    public:
    std::array<bool, 64> usedRegs = {
        // base registers
        1, 1, 1, 1, 1,
        0, 0, 0,
        1, 1,
        1, 1, 1, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0,
        // float registers
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1,
        1, 1,
        1, 1, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0
    };

    // stack
    int local_var_offset = -unit;
    int param_offset = -2 * unit;

    // local context
    bool is_function_def = 0;
    std::string ret_label;
    Specifier current_func_type;
    std::vector<Scope> scopes;
    std::vector<LoopLabel> loopLabels;
    std::unordered_map<std::string, Specifier> functionReturnTypes;

    Context() {
        scopes.push_back(Scope());
    }

    union Conversion {
        float f;
        double d;
        uint32_t i;
    } Conversion;

    int FloatToInt(float f){
        Conversion.f = f;
        return Conversion.i;
    }

    int DoubleToInt(double d){
        Conversion.d = d;
        return Conversion.i;
    }

    //register handling
    void useReg(int i) { usedRegs[i] = 1; }
    void freeReg(int i) { usedRegs[i] = 0; }
    int allocateReg(Specifier type);
    std::string getMnemonic(int i) const;

    //variables handling
    Variable getVar(const std::string& name) const;
    int getVarOffset(const std::string& name) const;
    Specifier getVarType(const std::string& name) const;
    int addVar(const std::string& name, Specifier type, bool is_pointer = false);
    int addParam(const std::string& name, Specifier type, int param_index, bool is_pointer = false);

    //scope handling
    void newScope();
    void popScope();
    void resetOffsets();
    void saveFuncReturnType(const std::string& func_name, Specifier return_type);

    //loops handling
    void addLabels(const std::string& start_label, const std::string& end_label);
    void popLabels();
    void updateStartLabel(const std::string& new_start_label);
    std::string getCurrentLoopStart() const;
    std::string getCurrentLoopEnd() const;
    int calculateStackSize(int totalLocalVarBytes, int totalParamBytes);

    //utility
    void loadInstruction(std::ostream& os, Specifier type, int dest_reg, int offset, int address_reg = 8 ) const;
    void storeInstruction(std::ostream& os, Specifier type, int dest_reg, int offset, int address_reg = 8 ) const;

    inline std::string makeLabel (std::string label){
        static int unique = 0 ;
        return label + std::to_string(unique++);
    }

    std::string specifierToString(Specifier specifier) const;
    void printVariableInfo(const std::string& func_name, const Variable& var) const;

    /*---TBC---*/
    bool getIsPointer(const std::string& name) const;
    void addArray(const std::string& array_name, Specifier type, int array_size);
};

#endif
