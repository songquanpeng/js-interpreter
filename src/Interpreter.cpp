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

void Interpreter::error(const string &message, const std::string &extra) {
    cerr << "[Interpreter] [Error]: " << message << extra << endl;
    exit(-1);
}

void Interpreter::log(const string &message, const std::string &extra) {
    cout << "[Interpreter] [Log]: " << message << extra << endl;
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
            map<string, Interpreter::Variable>::iterator iter;
            iter = variableTable.find(node->token.value);
            if (iter != variableTable.end()) {
                return iter->second.value;
            } else {
                log("cannot find required variable in variableTable: ", node->token.value);
                return "";
            }
        }
        case Parser::BINARY_OPERATOR_NODE: {
            string opt = node->token.value;
            string left = visitNode(node->child[0]);
            string right = visitNode(node->child[1]);
            return binaryOpt(opt, left, right);
        }
        case Parser::NONE:
            return "";
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
    var.value = visitExpressionNode(node->child[0]);
    variableTable.insert({varName, var});
    visitNode(node->next);
    return "";
}

string Interpreter::visitAssignNode(Parser::ASTNode *node) {
    assert(node->type == Parser::VAR_ASSIGN_NODE);
    string varName = node->token.value;
    Variable var;
    var.type = node->token.type;
    var.value = visitExpressionNode(node->child[0]);
    map<string, Interpreter::Variable>::iterator iter;
    iter = variableTable.find(node->token.value);
    if (iter != variableTable.end()) {
        iter->second.value = var.value;
    } else {
        log("assign value to a undefined variable", node->token.value);
        variableTable.insert({varName, var});
    }
    visitNode(node->next);
    return "";
}

string Interpreter::visitExpressionNode(Parser::ASTNode *node) {
    assert(node->type == Parser::EXPRESSION_NODE);
    if (node->child[1] == nullptr) {
        return node->token.value;
    } else {
        string opt = node->token.value;
        string left = visitNode(node->child[0]);
        string right = visitNode(node->child[1]);
        return binaryOpt(opt, left, right);
    }
}

string Interpreter::binaryOpt(const string &opt, const string &left, const string &right) {
    double lv = stof(left);
    double rv = stof(right);
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
    } else {
        error("unexpected operator: ", opt);
        return "";
    }
}

void Interpreter::printVariableTable() {
    cout << "Variable Table" << endl;
    cout << "+--------------------+" << endl;
    cout << "| " << std::left << setw(3) << "ID"
         << "| " << std::left << setw(2) << "T"
         << "| " << std::left << setw(10) << "Value"
         << "| " << endl;
    cout << "+--------------------+" << endl;
    for (const auto &e : variableTable) {
        cout << "| " << std::left << setw(3) << e.first
             << "| " << std::left << setw(2) << e.second.type
             << "| " << std::left << setw(10) << e.second.value
             << "| " << endl;
    }
    cout << "+--------------------+" << endl;
}





