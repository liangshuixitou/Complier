#include "node.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void NExpression::print(string prefix, ofstream *output)
{
    string type("> NExpression");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
}

void NStatement::print(std::string prefix, ofstream *output)
{
    string type("> NStatement");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
}

void NInteger::print(std::string prefix, ofstream *output)
{
    string type("> NInteger: value = ");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << this->value << endl;
    }
    cout << out << this->value << endl;
}

void NIdentifier::print(std::string prefix, ofstream *output)
{
    string type("> NIdentifier: name = ");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << this->name << endl;
    }
    cout << out << this->name << endl;
}

void NMethodCall::print(std::string prefix, ofstream *output)
{
    string type("> NMethodCall: name = ");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->id.print(prefix + "--", output);
    for (auto it = this->arguments.begin(); it != this->arguments.end(); it++)
    {
        (*it)->print(prefix + "--", output);
    }
}

void NBinaryOperator::print(std::string prefix, ofstream *output)
{
    string type("> NBinaryOperator: op = ");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << this->op << endl;
    }
    cout << out << this->op << endl;
    this->lhs.print(prefix + "--", output);
    this->rhs.print(prefix + "--", output);
}

void NAssignment::print(std::string prefix, ofstream *output)
{
    string type("> NAssignment");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->lhs.print(prefix + "--", output);
    this->rhs.print(prefix + "--", output);
}

void NBlock::print(std::string prefix, ofstream *output)
{
    string type("> NBlock");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    for (auto it = this->statements.begin(); it != this->statements.end(); it++)
    {
        (*it)->print(prefix + "--", output);
    }
}

void NExpressionStatement::print(std::string prefix, ofstream *output)
{
    string type("> NExpressionStatement");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->expression.print(prefix + "--", output);
}

void NVariableDeclaration::print(std::string prefix, ofstream *output)
{
    string type("> NVariableDeclaration");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->type.print(prefix + "--", output);
    this->id.print(prefix + "--", output);
    if (this->assignmentExpr)
        this->assignmentExpr->print(prefix + "--", output);
}

void NFunctionDeclaration::print(std::string prefix, ofstream *output)
{
    string type("> NFunctionDeclaration");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->type.print(prefix + "--", output);
    this->id.print(prefix + "--", output);
    for (auto it = this->arguments.begin(); it != this->arguments.end(); it++)
    {
        (*it)->print(prefix + "--", output);
    }
    if (this->block)
        this->block->print(prefix + "--", output);
}

void NReturnStatement::print(std::string prefix, ofstream *output)
{
    string type("> NReturnStatement");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    if (this->expression)
        this->expression->print(prefix + "--", output);
}

void NIfStatement::print(std::string prefix, ofstream *output)
{
    string type("> NIfStatement");
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    this->condition.print(prefix + "--", output);
    this->trueBlock->print(prefix + "--", output);
    if (this->falseBlock)
        this->falseBlock->print(prefix + "--", output);
}

void NForStatement::print(std::string prefix, ofstream *output)
{
    string type;
    if (this->initial == nullptr)
        type = "> NWhileStatement";
    else
        type = "> NForStatement";
    string out = prefix + type;
    if (output)
    {
        (*output) << out << endl;
    }
    cout << out << endl;
    if (this->initial)
        this->initial->print(prefix + "--", output);
    this->condition->print(prefix + "--", output);
    if (this->increment)
        this->increment->print(prefix + "--", output);
    this->block->print(prefix + "--", output);
}
