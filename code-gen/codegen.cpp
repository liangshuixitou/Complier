
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

#include "node.h"
#include "codegen.h"

using namespace std;
using legacy::PassManager;

extern llvm::Function *output_func;

Value *LogErrorV(string str)
{
    printf("ERROR: %s\n", str.c_str());
    return nullptr;
}

Value *LogErrorV(const char *str)
{
    printf("ERROR: %s\n", str);
    return nullptr;
}

static Type *typeOf(const NIdentifier &type, CodeGenContext &context)
{
    Type *ttype = nullptr;
    if (type.name.compare("int") == 0)
    {
        ttype = Type::getInt32Ty(context.llvmContext);
    }
    if (type.name.compare("void") == 0)
    {
        ttype = Type::getVoidTy(context.llvmContext);
    }
    return ttype;
}

#define ISTYPE(value, id) (value->getType()->getTypeID() == id)
static Value *CastToBoolean(CodeGenContext &context, Value *condValue)
{

    if (ISTYPE(condValue, Type::IntegerTyID))
    {
        condValue = context.builder.CreateIntCast(condValue, Type::getInt1Ty(context.llvmContext), true);
        return context.builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(context.llvmContext), 0, true));
    }
    else
    {
        return condValue;
    }
}

void CodeGenContext::generateCode(NBlock &root)
{
    cout << "Generating IR code" << endl;

    std::vector<Type *> sysArgs;
    FunctionType *mainFuncType = FunctionType::get(Type::getVoidTy(this->llvmContext), makeArrayRef(sysArgs), false);
    Function *mainFunc = Function::Create(mainFuncType, GlobalValue::ExternalLinkage, "main");
    BasicBlock *block = BasicBlock::Create(this->llvmContext, "entry");

    pushBlock(block);
    Value *retValue = root.codeGen(*this);
    popBlock();

    cout << "Code generate success！！！" << endl;

    return;
}

llvm::Value *NInteger::codeGen(CodeGenContext &context)
{
    cout << "Generating Integer:" << this->value << endl;
    return ConstantInt::get(Type::getInt32Ty(context.llvmContext), this->value, true);
}

llvm::Value *NIdentifier::codeGen(CodeGenContext &context)
{ //标识符表达式,用于引用有变量名的表达式中，语义为取出变量对应的值
    llvm::Value *last = nullptr;
    cout << "Generating identifier:" << this->name << endl;
    Value *value = context.getSymbolValue(this->name);
    if (!value)
    {
        return LogErrorV("Undeclared variable name " + this->name);
    }
    last = context.builder.CreateLoad(value, false, "");
    return last;
}

llvm::Value *NMethodCall::codeGen(CodeGenContext &context)
{ //方法调用，获取f，遍历获取参数列表为每个参数生成代码
    llvm::Value *last = nullptr;
    std::cout << "Generating method call of:" << this->id.name << endl;
    if (!this->id.name.compare("output"))
    {
        // auto it = this->arguments.begin();
        // NIdentifier *id = (NIdentifier *)*it;
        // Value *value = context.getSymbolValue(id->name);
        // std::cout << *(long long *)value;

        // return (llvm::Value *)0;

        // ------------------------------------------

        auto it = this->arguments.begin();
        NIdentifier *id = (NIdentifier *)*it;
        cout << "print var " + id->name + " (" + context.getSymbolType(id->name)->name + ")" << endl;
        Value *value = context.getSymbolValue(id->name);

        if (value == nullptr)
        {
            cout << "var " + id->name + " is undefined" << endl;
        }

        auto var = context.builder.CreateLoad(value);

        //输出int变量
        context.builder.CreateCall(output_func,
                                   {context.builder.CreateGlobalStringPtr("value = %d\n"), var});
        return (llvm::Value *)0;
        // ----------------------------------------------------
    }

    Function *f = context.theModule->getFunction(this->id.name);
    //对象调用
    if (!f)
    {
        LogErrorV("Function name not found");
    }
    if (f->arg_size() != this->arguments.size())
    {
        LogErrorV("Function arguments size not match, the function " + this->id.name + " has " + std::to_string(f->arg_size()) + " arguments, but found " + std::to_string(this->arguments.size()) + " arguments");
    }
    std::vector<Value *> args;
    for (auto it = this->arguments.begin(); it != this->arguments.end(); it++)
    {
        args.push_back((*it)->codeGen(context));
    }
    return context.builder.CreateCall(f, args, "callf");
}

llvm::Value *NBinaryOperator::codeGen(CodeGenContext &context)
{ //二元运算符，其中两操作数为整数 符号包括+ - * / == != >= <= > <
    llvm::Value *last = nullptr;
    std::cout << "Generating binary operator" << std::endl;
    Value *L = this->lhs.codeGen(context);
    Value *R = this->rhs.codeGen(context);
    switch (op)
    {
    case TPLUS:
        return context.builder.CreateAdd(L, R, "addtmp");
    case TMINUS:
        return context.builder.CreateSub(L, R, "subtmp");
    case TMUL:
        return context.builder.CreateMul(L, R, "multmp");
    case TDIV:
        return context.builder.CreateSDiv(L, R, "divtmp");
    case TCLT:
        return context.builder.CreateICmpULT(L, R, "cmptmp");
        // return nullptr;
    case TCLE:
        return context.builder.CreateICmpSLE(L, R, "cmptmp");
    case TCGE:
        return context.builder.CreateICmpSGE(L, R, "cmptmp");
    case TCGT:
        return context.builder.CreateICmpSGT(L, R, "cmptmp");
    case TCEQ:
        return context.builder.CreateICmpEQ(L, R, "cmptmp");
    case TCNE:
        return context.builder.CreateICmpNE(L, R, "cmptmp");
    default:
        return LogErrorV("Unknown binary operator");
    }
    return last;
}

llvm::Value *NAssignment::codeGen(CodeGenContext &context)
{ //赋值语句，为右式生成中间代码
    std::cout << "Generating assignment of " << this->lhs.name << endl;
    Value *dst = context.getSymbolValue(this->lhs.name);
    NIdentifier *dstType = context.getSymbolType(this->lhs.name);
    if (!dst)
    {
        return LogErrorV("Undeclared variable");
    }
    string dstTypeStr = dstType->name;
    Value *exp = this->rhs.codeGen(context);
    context.builder.CreateStore(exp, dst);
    return dst;
}

llvm::Value *NBlock::codeGen(CodeGenContext &context)
{ //调用其内部的语句列表（Statements）中每个语句的codeGen生成对应的中间代码，
    //并将最后一个语句（Return语句）的SSA值作为该结点的值返回。
    cout << "Generating block" << endl;
    Value *last = nullptr;
    for (auto it = this->statements.begin(); it != this->statements.end(); it++)
    {
        last = (*it)->codeGen(context);
    }
    return last;
}

llvm::Value *NExpressionStatement::codeGen(CodeGenContext &context)
{ //表达式声明，直接生成即可
    return this->expression.codeGen(context);
}

llvm::Value *NVariableDeclaration::codeGen(CodeGenContext &context)
{ //定义变量
    std::cout << "Generating variable declaration of " << this->type.name << " " << this->id.name << endl;
    Type *type = typeOf(this->type, context); //此处可能有问题,typeOf
    Value *inst = nullptr;

    inst = context.builder.CreateAlloca(type);

    context.setSymbolType(this->id.name, this->type);
    context.setSymbolValue(this->id.name, inst);

    context.PrintSymTable();

    if (this->assignmentExpr != nullptr)
    {
        NAssignment assignment(this->id, *(this->assignmentExpr));
        //也可能是NAssignment assignment(this->id, *(this->assignmentExpr));
        assignment.codeGen(context);
    }
    return inst;
}

llvm::Value *NFunctionDeclaration::codeGen(CodeGenContext &context) //修改
{                                                                   //函数定义,返回值 函数名 入口 参数列表
    llvm::Value *last = nullptr;
    std::cout << "Generating function declaration of " << this->id.name << endl;
    std::vector<Type *> argTypes;
    VariableList::const_iterator it;
    for (it = this->arguments.begin(); it != this->arguments.end(); it++)
    {
        argTypes.push_back(typeOf((**it).type, context));
    }
    Type *retType = typeOf(this->type, context);

    FunctionType *functionType = FunctionType::get(retType, argTypes, false);
    Function *function = Function::Create(functionType, GlobalValue::ExternalLinkage,
                                          this->id.name.c_str(), context.theModule);
    BasicBlock *basicBlock = BasicBlock::Create(context.llvmContext,
                                                "entry", function, nullptr);
    context.builder.SetInsertPoint(basicBlock);
    context.pushBlock(basicBlock);
    //声明参数
    auto origin_arg = this->arguments.begin();
    for (auto &ir_arg_it : function->args())
    {
        ir_arg_it.setName((*origin_arg)->id.name);
        Value *argAlloc = (*origin_arg)->codeGen(context);
        context.builder.CreateStore(&ir_arg_it, argAlloc, false);
        context.setSymbolValue((*origin_arg)->id.name, argAlloc);
        context.setSymbolType((*origin_arg)->id.name, (*origin_arg)->type);
        context.setFuncArg((*origin_arg)->id.name, true);
        origin_arg++;
    }
    this->block->codeGen(context);
    if (context.getCurrentReturnValue())
    {
        context.builder.CreateRet(context.getCurrentReturnValue());
    }
    else if (retType != Type::getVoidTy(context.llvmContext))
    {
        return LogErrorV("Function block return value not founded");
    }
    context.popBlock();
    return function;
}

llvm::Value *NIfStatement::codeGen(CodeGenContext &context)
{ //条件语句声明
    std::cout << "Generating if statement" << std::endl;

    Value *condValue = this->condition.codeGen(context);
    if (!condValue)
        return nullptr;
    //缺失该函数
    condValue = CastToBoolean(context, condValue);
    //if语句所在的函数
    Function *theFunction = context.builder.GetInsertBlock()->getParent(); // the function where if statement is in
    //if为真 if为假 合并后
    BasicBlock *thenBB = BasicBlock::Create(context.llvmContext, "then", theFunction);
    BasicBlock *falseBB = BasicBlock::Create(context.llvmContext, "else");
    BasicBlock *mergeBB = BasicBlock::Create(context.llvmContext, "ifcont");

    if (this->falseBlock)
    { //如果有else部分
        context.builder.CreateCondBr(condValue, thenBB, falseBB);
    }
    else
    {
        context.builder.CreateCondBr(condValue, thenBB, mergeBB);
    }

    context.builder.SetInsertPoint(thenBB);
    context.pushBlock(thenBB);
    this->trueBlock->codeGen(context);
    context.popBlock();
    thenBB = context.builder.GetInsertBlock();

    if (thenBB->getTerminator() == nullptr)
    {
        context.builder.CreateBr(mergeBB);
    }

    if (this->falseBlock)
    { //如果有else部分
        theFunction->getBasicBlockList().push_back(falseBB);
        context.builder.SetInsertPoint(falseBB);

        context.pushBlock(thenBB);
        this->falseBlock->codeGen(context);
        context.popBlock();
        context.builder.CreateBr(mergeBB);
    }

    theFunction->getBasicBlockList().push_back(mergeBB);
    context.builder.SetInsertPoint(mergeBB);

    return nullptr;
}

llvm::Value *NReturnStatement::codeGen(CodeGenContext &context)
{ //返回值，直接生成即可
    llvm::Value *last = nullptr;
    std::cout << "Generating return statement";
    Value *returnValue = this->expression->codeGen(context);
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

llvm::Value *NForStatement::codeGen(CodeGenContext &context)
{
    std::cout << "Generating loop statement";

    Function *theFunction = context.builder.GetInsertBlock()->getParent();

    BasicBlock *block = BasicBlock::Create(context.llvmContext, "forloop", theFunction);
    BasicBlock *after = BasicBlock::Create(context.llvmContext, "forcont");

    // 操作initial
    if (this->initial)
        this->initial->codeGen(context);

    Value *condValue = this->condition->codeGen(context);
    if (!condValue)
        return nullptr;

    condValue = CastToBoolean(context, condValue);

    context.builder.CreateCondBr(condValue, block, after);
    context.builder.SetInsertPoint(block);
    context.pushBlock(block);
    this->block->codeGen(context);
    context.popBlock();

    if (this->increment)
    {
        this->increment->codeGen(context);
    }

    condValue = this->condition->codeGen(context);
    condValue = CastToBoolean(context, condValue);
    context.builder.CreateCondBr(condValue, block, after);

    theFunction->getBasicBlockList().push_back(after);
    context.builder.SetInsertPoint(after);

    return nullptr;
}