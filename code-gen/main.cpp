#include <iostream>
#include <fstream>
#include "node.h"
#include "codegen.h"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/raw_os_ostream.h>
#include <system_error>

extern NBlock *programBlock;
extern int yyparse();
extern int input(int argc, char **argv);

using namespace std;
using namespace llvm;

llvm::Function *output_func = nullptr;

int printModule(CodeGenContext &context)
{
    auto Filename = "main.ll";
    error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::F_None);
    if (EC)
    {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }
    // TheModule->print(errs(), nullptr); // print IR to terminal
    context.theModule->print(dest, nullptr);
}

llvm::Function *get_output(llvm::Module *module)
{
    const char *fun_name = "printf";
    llvm::Function *func = module->getFunction(fun_name);
    if (func == nullptr)
    {
        llvm::FunctionType *func_type = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(module->getContext()),
            {llvm::Type::getInt8PtrTy(module->getContext())},
            true);
        func = llvm::Function::Create(func_type, llvm::GlobalValue::ExternalLinkage, fun_name, module);
    }
    return func;
}

int main(int argc, char **argv)
{
    input(argc, argv);

    yyparse();

    CodeGenContext context;
    output_func = get_output(context.theModule);
    //    createCoreFunctions(context);
    context.generateCode(*programBlock);
    printModule(context);

    std::ofstream out("ASTtree.txt");
    out << "\nSTART AST" << std::endl;
    std::cout << "\nSTART AST" << std::endl;
    programBlock->print("-", &out);

    return 0;
}