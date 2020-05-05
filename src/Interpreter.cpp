#include "Interpreter.h"
#include <iostream>
#include <cassert>
#include <iomanip>
#include <ctime>

using namespace std;

Interpreter::Interpreter() {
    variableTable.push_back(new map<string, Variable>);
    scopeLevel = 0;
    root = nullptr;
}

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
        string line;
        getline(cin, line);
        if (!line.empty() && line[line.size() - 1] == '{') {
            do {
                cout << "... ";
                input += line;
            } while (getline(cin, line) && (!line.empty())
                     && line[line.size() - 1] != '}');
        }
        input += line;
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
    for (int i = scopeLevel; i >= 0; --i) {
        map<string, Variable> *scope = variableTable[i];
        map<string, Interpreter::Variable>::iterator iter;
        for (const auto &e : *scope) {
            cout << "| " << std::left << setw(3) << e.first
                 << "| " << std::left << setw(2) << e.second.type
                 << "| " << std::left << setw(20) << e.second.value
                 << "| " << endl;
        }
        cout << "+----+---+---------------------+" << endl;
    }
}

void Interpreter::setDebugMode(bool enable) {
    debug = enable;
    parser.setDebugMode(enable);
}

void Interpreter::enterScope() {
    assert(variableTable.size() == scopeLevel + 1);
    scopeLevel++;
    variableTable.push_back(new map<string, Variable>);
}

void Interpreter::exitScope() {
    scopeLevel--;
    variableTable.pop_back();
    assert(variableTable.size() == scopeLevel + 1);
}

bool Interpreter::declareVariable(const std::string &name, const Variable &variable) {
    map<string, Variable> *scope = variableTable[scopeLevel];
    map<string, Variable>::iterator iter;
    iter = scope->find(name);
    if (iter != scope->end()) {
        log("define a variable multiple times: ", name);
    }
    scope->insert({name, variable});
    return true;
}

bool Interpreter::setVariableValue(const std::string &name, const Variable &variable) {
    bool success = false;
    for (int i = scopeLevel; i >= 0; --i) {
        map<string, Variable> *scope = variableTable[i];
        map<string, Interpreter::Variable>::iterator iter;
        iter = scope->find(name);
        if (iter != scope->end()) {
            iter->second.value = variable.value;
            success = true;
            break;
        }
    }
    return success;
}

string Interpreter::getVariableValue(const string &name) {
    for (int i = scopeLevel; i >= 0; --i) {
        map<string, Variable> *scope = variableTable[i];
        map<string, Interpreter::Variable>::iterator iter;
        iter = scope->find(name);
        if (iter != scope->end()) {
            return iter->second.value;
        }
    }
    log("use undefined variable: ", name);
    return "";
}

Parser::ASTNode *Interpreter::getFunction(const std::string &name) {
    map<string, Parser::ASTNode *>::iterator iter;
    iter = functionTable.find(name);
    if (iter != functionTable.end()) {
        return iter->second;
    } else {
        log("use undefined function: ", name);
    }
    return nullptr;
}

string Interpreter::input() {
    string input;
    getline(cin, input);
    return input;
}

void Interpreter::output(const string &value) {
    cout << value << endl;
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
        case Parser::VAR_NODE:
            return getVariableValue(node->token.value);
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
        case Parser::FUNCTION_DECLARE_NODE:
            return visitFunctionDeclareNode(node);
        case Parser::FUNCTION_CALL_NODE:
            return visitFunctionCallNode(node);
        case Parser::RETURN_NODE:
            return visitReturnNode(node);
        case Parser::ARRAY_ACCESS_NODE:
            return visitArrayAccessNode(node);
        case Parser::ARRAY_DECLARE_NODE:
            return visitArrayDeclareNode(node);
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
    declareVariable(varName, var);
    visitNode(node->next);
    return var.value;
}

string Interpreter::visitAssignNode(Parser::ASTNode *node) {
    assert(node->type == Parser::VAR_ASSIGN_NODE);
    string varName = node->token.value;
    Variable var;
    var.type = node->token.type;
    var.value = visitNode(node->child[0]);
    setVariableValue(varName, var);
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
    string result;
    string condition = visitNode(node->child[0]);
    if (condition != "0" && condition != "false" && !condition.empty()) {
        result = visitNode(node->child[1]);
    } else {
        if (node->child[2] != nullptr) {
            result = visitNode(node->child[2]);
        }
    }
    visitNode(node->next);
    return result;
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
        if (node->child[0]->type == Parser::STRING_NODE) {
            return left + right;
        }
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
    enterScope();
    assert(node->type == Parser::WHILE_NODE);
    string condition = visitNode(node->child[0]);
    while (condition != "0" && condition != "false" && !condition.empty()) {
        visitNode(node->child[1]);
        condition = visitNode(node->child[0]);
    }
    exitScope();
    visitNode(node->next);
    return "";
}

string Interpreter::visitForNode(Parser::ASTNode *node) {
    enterScope();
    assert(node->type == Parser::FOR_NODE);
    visitNode(node->child[0]); // Initialization
    string condition = visitNode(node->child[1]); // Condition
    while (condition != "0" && condition != "false" && !condition.empty()) {
        visitNode(node->child[3]); // Body
        visitNode(node->child[2]); // Update
        condition = visitNode(node->child[1]); // Check condition
    }
    exitScope();
    visitNode(node->next);
    return "";
}

string Interpreter::visitFunctionDeclareNode(Parser::ASTNode *node) {
    assert(node->type == Parser::FUNCTION_DECLARE_NODE);
    string name = node->token.value;
    map<string, Parser::ASTNode *>::iterator iter;
    iter = functionTable.find(name);
    if (iter == functionTable.end()) {
        functionTable.insert({name, node});
    } else {
        log("define a function multiple times: ", name);
    }
    visitNode(node->next);
    return "";
}

string Interpreter::visitFunctionCallNode(Parser::ASTNode *node) {
    enterScope();
    string result;
    string functionName = node->token.value;
    Parser::ASTNode *parameterNode = node->child[0];
    if (functionName == "input") {
        result = input();
    } else if (functionName == "output") {
        string outputValue = visitNode(parameterNode);
        output(outputValue);
    } else {
        // First we should initialize the parameters with arguments.
        Parser::ASTNode *functionNode = getFunction(functionName);
        Parser::ASTNode *argumentNode = functionNode->child[0];
        while (argumentNode != nullptr && parameterNode != nullptr) {
            Variable var;
            var.type = argumentNode->token.type;
            var.value = visitNode(parameterNode);
            declareVariable(argumentNode->token.value, var);
            argumentNode = argumentNode->next;
            parameterNode = parameterNode->next;
        }
        // The we execute this function's body.
        result = visitNode(functionNode->child[1]);
    }
    exitScope();
    visitNode(node->next);
    return result;
}

string Interpreter::visitReturnNode(Parser::ASTNode *node) {
    assert(node->type == Parser::RETURN_NODE);
    return visitNode(node->child[0]);
}

string Interpreter::visitArrayDeclareNode(Parser::ASTNode *node) {
    assert(node->type == Parser::ARRAY_DECLARE_NODE);
    string identifier("__array_" + to_string(time(nullptr)));
    auto *store = new vector<string>;
    auto *current = node->child[0];
    while (current != nullptr) {
        store->push_back(visitNode(current));
        current = current->next;
    }
    arrayTable.insert({identifier, store});
    return identifier;
}

string Interpreter::visitArrayAccessNode(Parser::ASTNode *node) {
    assert(node->type == Parser::ARRAY_ACCESS_NODE);
    string arrayName = node->token.value;
    vector<string> *v = getArray(arrayName);
    string index = visitNode(node->child[0]);
    int i = 0;
    try {
        i = stoi(index);
    } catch (std::invalid_argument &e) {
        i = 0;
    }
    return (*v)[i];
}

std::vector<string> *Interpreter::getArray(const std::string &name) {
    string identifier = getVariableValue(name);
    assert(identifier[0] == '_');
    map<std::string, vector<string> *>::iterator iter;
    iter = arrayTable.find(identifier);
    if (iter != arrayTable.end()) {
        return iter->second;
    }
    log("use of undefined array: ", name);
    return nullptr;
}
