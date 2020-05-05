#include "Interpreter.h"
#include <iostream>
#include <cassert>
#include <iomanip>

using namespace std;

void Interpreter::interpretFile(string &filename) {
    parser.parseFile(filename);
    root = parser.getAST();
    visitNode(root);
    printVariableTable();
}

void Interpreter::shell() {
    cout << "Welcome to Node.js v12.16.1.\n"
            "Type \".help\" for more information." << endl;
    while (true) {
        cout << "> ";
        string input;
        getline(cin, input);
        if (input.empty()) continue;
        if (!shellExecute(input)) break;
    }
}

bool Interpreter::shellExecute(const string &input) {
    if (input == ".exit") {
        return false;
    } else if (input == ".show") {
        printVariableTable();
        return true;
    } else if (input == ".debug") {
        setDebugMode(true);
        cout << "Debug mode enabled." << endl;
        return true;
    } else if (input == ".help") {
        cout << ".help     Print this help message\n"
             << ".exit     Exit the repl\n"
             << ".show     Print the variable table\n"
             << ".debug    Enable debug mode\n"
             << endl
             << "Press ^C to exit the repl" << endl;
        return true;
    }
    Parser::ASTNode *node = parser.parseInput(input);
    string output = visitNode(node);
    cout << (output.empty() ? "undefined" : output) << endl;
    return true;
}

void Interpreter::error(const string &message, const std::string &extra) {
    cerr << "[Interpreter] [Error]: " << message << extra << endl;
    exit(-1);
}

void Interpreter::log(const string &message, const std::string &extra) {
    if (!debug) return;
    cout << "[Interpreter] [Log]: " << message << extra << endl;
}

void Interpreter::printVariableTable() {
    cout << "Variable Table" << endl;
    cout << "+----+---+---------------------+" << endl;
    cout << "| " << std::left << setw(3) << "ID"
         << "| " << std::left << setw(2) << "T"
         << "| " << std::left << setw(20) << "Value"
         << "| " << endl;
    cout << "+----+---+---------------------+" << endl;
    for (const auto &e : variableTable) {
        cout << "| " << std::left << setw(3) << e.first
             << "| " << std::left << setw(2) << e.second.type
             << "| " << std::left << setw(20) << e.second.value
             << "| " << endl;
    }
    cout << "+----+---+---------------------+" << endl;
}

void Interpreter::setDebugMode(bool enable) {
    debug = enable;
    parser.setDebugMode(enable);
}

string Interpreter::getVariableValue(const string &name) {
    map<string, Interpreter::Variable>::iterator iter;
    iter = variableTable.find(name);
    if (iter != variableTable.end()) {
        return iter->second.value;
    } else {
        log("use undefined variable: ", name);
        return "";
    }
}

string Interpreter::visitNode(Parser::ASTNode *node) {
    if (node == nullptr) {
        log("visitNode: given node is nullptr");
        return "";
    }
    switch (node->type) {
        case Parser::VAR_DECLARE_NODE:
            return visitDeclareNode(node);
        case Parser::VAR_ASSIGN_NODE:
            return visitAssignNode(node);
        case Parser::EXPRESSION_NODE:
            return visitExpressionNode(node);
        case Parser::INT_NODE:
        case Parser::REAL_NODE:
        case Parser::STRING_NODE:
        case Parser::CHAR_NODE:
        case Parser::BOOL_NODE:
            return node->token.value;
        case Parser::VAR_NODE: {
            return getVariableValue(node->token.value);
        }
        case Parser::BINARY_OPERATOR_NODE:
            return visitBinaryOperatorNode(node);
        case Parser::NONE:
            return "";
        case Parser::NEGATIVE_NODE:
            return visitNegativeNode(node);
        case Parser::IF_NODE:
            return visitIfNode(node);
        case Parser::WHILE_NODE:
            return visitWhileNode(node);
        case Parser::FOR_NODE:
            return visitForNode(node);
        default:
            error("unexpected node type: ", to_string(node->type));
            return "";
    }
}

string Interpreter::visitDeclareNode(Parser::ASTNode *node) {
    assert(node->type == Parser::VAR_DECLARE_NODE);
    string varName = node->token.value;
    Variable var;
    var.type = node->token.type;
    var.value = visitNode(node->child[0]);
    variableTable.insert({varName, var});
    visitNode(node->next);
    return var.value;
}

string Interpreter::visitAssignNode(Parser::ASTNode *node) {
    assert(node->type == Parser::VAR_ASSIGN_NODE);
    string varName = node->token.value;
    Variable var;
    var.type = node->token.type;
    var.value = visitNode(node->child[0]);
    map<string, Interpreter::Variable>::iterator iter;
    iter = variableTable.find(node->token.value);
    if (iter != variableTable.end()) {
        iter->second.value = var.value;
    } else {
        log("assign value to a undefined variable", node->token.value);
        variableTable.insert({varName, var});
    }
    visitNode(node->next);
    return var.value;
}

string Interpreter::visitExpressionNode(Parser::ASTNode *node) {
    assert(node->type == Parser::EXPRESSION_NODE);
    return visitNode(node->child[0]);
}


string Interpreter::visitNegativeNode(Parser::ASTNode *node) {
    string result = visitNode(node->child[0]);
    if (!result.empty() && result[0] == '-') {
        result = result.substr(1, result.size() - 1);
    } else {
        result = string("-") + result;
    }
    return result;
}

string Interpreter::visitIfNode(Parser::ASTNode *node) {
    assert(node->type == Parser::IF_NODE);
    string condition = visitNode(node->child[0]);
    if (condition != "0" && condition != "false" && !condition.empty()) {
        return visitNode(node->child[1]);
    } else {
        if (node->child[2] != nullptr) {
            return visitNode(node->child[2]);
        } else {
            return "";
        }
    }
}

string Interpreter::visitBinaryOperatorNode(Parser::ASTNode *node) {
    string opt = node->token.value;
    string left = visitNode(node->child[0]);
    string right = visitNode(node->child[1]);
    double lv, rv;
    try {
        lv = stof(left);
    } catch (std::invalid_argument &e) {
        if (left == "false") lv = 0;
        else lv = 1;
    }
    try {
        rv = stof(right);
    } catch (std::invalid_argument &e) {
        if (right == "false") rv = 0;
        else rv = 1;
    }
    if (opt == "+") {
        return to_string(lv + rv);
    } else if (opt == "-") {
        return to_string(lv - rv);
    } else if (opt == "*") {
        return to_string(lv * rv);
    } else if (opt == "/") {
        return to_string(lv / rv);
    } else if (opt == "<=") {
        return lv <= rv ? "true" : "false";
    } else if (opt == ">=") {
        return lv >= rv ? "true" : "false";
    } else if (opt == "==") {
        return lv == rv ? "true" : "false";
    } else if (opt == "<") {
        return lv < rv ? "true" : "false";
    } else if (opt == ">") {
        return lv > rv ? "true" : "false";
    } else if (opt == "!=") {
        return lv != rv ? "true" : "false";
    } else if (opt == "&&") {
        return (lv == 0 || rv == 0) ? "false" : "true";
    } else if (opt == "||") {
        return (lv != 0 || rv != 0) ? "true" : "false";
    } else {
        error("unexpected operator: ", opt);
        return "";
    }
}

string Interpreter::visitWhileNode(Parser::ASTNode *node) {
    assert(node->type == Parser::WHILE_NODE);
    string condition = visitNode(node->child[0]);
    while (condition != "0" && condition != "false" && !condition.empty()) {
        visitNode(node->child[1]);
        condition = visitNode(node->child[0]);
    }
    return "";
}

string Interpreter::visitForNode(Parser::ASTNode *node) {
    assert(node->type == Parser::FOR_NODE);
    visitNode(node->child[0]); // Initialization
    string condition = visitNode(node->child[1]); // Condition
    while (condition != "0" && condition != "false" && !condition.empty()) {
        visitNode(node->child[3]); // Body
        visitNode(node->child[2]); // Update
        condition = visitNode(node->child[1]); // Check condition
    }
    return "";
}

