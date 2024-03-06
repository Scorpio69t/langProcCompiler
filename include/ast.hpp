#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast_arithematic.hpp"
#include "ast_binaryOperation.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_jump_statement.hpp"
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include "ast_constant.hpp"
#include "ast_context.hpp"
#include "ast_declarator.hpp"
#include "ast_compound_statement.hpp"
#include "ast_pointer_declarator.hpp"
#include "ast_expression_statement.hpp"
#include "ast_declaration.hpp"
#include "ast_unary.hpp"
#include "ast_for_statement.hpp"
#include "ast_function_call.hpp"
#include "ast_assignment.hpp"
#include "ast_while.hpp"
#include "ast_if_else.hpp"
#include "ast_switch.hpp"
#include "ast_case.hpp"
#include "ast_array.hpp"
#include "ast_default.hpp"


extern Node *ParseAST(std::string file_name);

#endif
