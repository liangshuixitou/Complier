#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <llvm/IR/Value.h>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;
typedef std::vector<NStatement *> StatementList;
typedef std::vector<NExpression *> ExpressionList;
typedef std::vector<NVariableDeclaration *> VariableList;

class Node
{
public:
    virtual ~Node() {}
    virtual std::string getTypeName() const = 0;
    virtual void print(std::string prefix, std::ofstream *output) const {}
    virtual llvm::Value *codeGen(CodeGenContext &context) { return (llvm::Value *)0; }
};

class NExpression : public Node
{
public:
    std::string getTypeName() const override
    {
        return "NExpression";
    }
    virtual void print(std::string prefix, std::ofstream *output);
};

class NStatement : public Node
{
public:
    std::string getTypeName() const override
    {
        return "NStatement";
    }
    virtual void print(std::string prefix, std::ofstream *output);
};

class NInteger : public NExpression
{
public:
    long long value;

    NInteger(long long value) : value(value) {}

    std::string getTypeName() const override
    {
        return "NInteger";
    }

    void print(std::string prefix, std::ofstream *output);
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NIdentifier : public NExpression
{
public:
    std::string name;

    bool isType = false;

    NIdentifier(const std::string &name) : name(name) {}

    void print(std::string prefix, std::ofstream *output);

    std::string getTypeName() const override
    {
        return "NIdentifier";
    }

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NMethodCall : public NExpression
{
public:
    NIdentifier &id;

    ExpressionList arguments;

    NMethodCall(NIdentifier &id, ExpressionList &arguments) : id(id), arguments(arguments) {}

    NMethodCall(NIdentifier &id) : id(id) {}

    void print(std::string prefix, std::ofstream *output);

    std::string getTypeName() const override
    {
        return "NMethodCall";
    }

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NBinaryOperator : public NExpression
{
public:
    NExpression &lhs;

    int op = 0;

    NExpression &rhs;

    NBinaryOperator(NExpression &lhs, int op, NExpression &rhs) : lhs(lhs), op(op), rhs(rhs) {}

    std::string getTypeName() const override
    {
        return "NBinaryOperator";
    }
    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NAssignment : public NExpression
{
public:
    NIdentifier &lhs;

    NExpression &rhs;

    NAssignment(NIdentifier &lhs, NExpression &rhs) : lhs(lhs), rhs(rhs) {}

    std::string getTypeName() const override
    {
        return "NAssignment";
    }

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NBlock : public NExpression
{
public:
    StatementList statements;

    NBlock() {}

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);

    std::string getTypeName() const override
    {
        return "NBlock";
    }
};

class NExpressionStatement : public NStatement
{
public:
    NExpression &expression;

    NExpressionStatement(NExpression &expression) : expression(expression) {}

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);

    std::string getTypeName() const override
    {
        return "NExpressionStatement";
    }
};

class NVariableDeclaration : public NStatement
{
public:
    NIdentifier &type;

    NIdentifier &id;

    NExpression *assignmentExpr = nullptr;

    NVariableDeclaration(NIdentifier &type, NIdentifier &id) : type(type), id(id) {}

    NVariableDeclaration(NIdentifier &type, NIdentifier &id, NExpression *assignmentExpr) : type(type), id(id), assignmentExpr(assignmentExpr) {}

    std::string getTypeName() const override
    {
        return "NVariableDeclaration";
    }
    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NFunctionDeclaration : public NStatement
{
public:
    NIdentifier &type;

    NIdentifier &id;

    VariableList &arguments;

    NBlock *block;

    NFunctionDeclaration(NIdentifier &type, NIdentifier &id,
                         VariableList &arguments, NBlock *block) : type(type), id(id), arguments(arguments), block(block) {}

    std::string getTypeName() const override
    {
        return "NFunctionDeclaration";
    }

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NReturnStatement : public NStatement
{
public:
    NExpression *expression;

    NReturnStatement(NExpression *expression)
        : expression(expression)
    {
    }

    std::string getTypeName() const override
    {
        return "NReturnStatement";
    }

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NIfStatement : public NStatement
{
public:
    NExpression &condition;
    NBlock *trueBlock;  // should not be null
    NBlock *falseBlock; // can be null

    NIfStatement(NExpression &condition, NBlock *trueBlock, NBlock *falseBlock = nullptr)
        : condition(condition), trueBlock(trueBlock), falseBlock(falseBlock)
    {
    }

    std::string getTypeName() const override
    {
        return "NIfStatement";
    }

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class NForStatement : public NStatement
{
public:
    NBlock *block;

    NExpression *initial;

    NExpression *condition;

    NExpression *increment;

    NForStatement(NBlock *block, NExpression *initial, NExpression *condition, NExpression *increment)
        : block(block), initial(initial), condition(condition), increment(increment)
    {
    }

    std::string getTypeName() const override
    {
        return "NForStatement";
    }

    void print(std::string prefix, std::ofstream *output);

    virtual llvm::Value *codeGen(CodeGenContext &context);
};