#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "grammar.tab.h"

using namespace llvm;
using std::string;
using std::unique_ptr;

using SymTable = std::map<string, Value *>;

class CodeGenBlock
{
public:
    BasicBlock *block = nullptr;
    Value *returnValue = nullptr;
    std::map<string, Value *> locals;
    std::map<string, NIdentifier *> types; // type name string of vars
    std::map<string, bool> isFuncArg;
    std::map<string, std::vector<uint64_t>> arraySizes;
};

class CodeGenContext
{
private:
    std::vector<CodeGenBlock *> blockStack;

public:
    LLVMContext llvmContext;
    IRBuilder<> builder;
    Module *theModule;
    SymTable globalVars;

    CodeGenContext() : builder(llvmContext)
    {
        theModule = new Module("main", this->llvmContext);
    }

    Value *getSymbolValue(string name) const
    {
        // 逆向迭代器
        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++)
        {
            if ((*it)->locals.find(name) != (*it)->locals.end())
            {
                return (*it)->locals[name];
            }
        }
        return nullptr;
    }

    NIdentifier *getSymbolType(string name) const
    {
        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++)
        {
            //            cout << "(*it)->locals[" << name << "] = " << (*it)->locals[name] << endl;
            if ((*it)->types.find(name) != (*it)->types.end())
            {
                return (*it)->types[name];
            }
        }
        return nullptr;
    }

    bool isFuncArg(string name) const
    {

        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++)
        {
            if ((*it)->isFuncArg.find(name) != (*it)->isFuncArg.end())
            {
                return (*it)->isFuncArg[name];
            }
        }
        return false;
    }

    void setSymbolValue(string name, Value *value)
    {
        blockStack.back()->locals[name] = value;
    }

    void setSymbolType(string name, NIdentifier &value)
    {
        blockStack.back()->types[name] = &value;
    }

    void setFuncArg(string name, bool value)
    {
        std::cout << "Set " << name << " as func arg" << std::endl;
        blockStack.back()->isFuncArg[name] = value;
    }

    BasicBlock *currentBlock() const
    {
        return blockStack.back()->block;
    }

    void pushBlock(BasicBlock *block)
    {
        CodeGenBlock *codeGenBlock = new CodeGenBlock();
        codeGenBlock->block = block;
        codeGenBlock->returnValue = nullptr;
        blockStack.push_back(codeGenBlock);
    }

    void popBlock()
    {
        CodeGenBlock *codeGenBlock = blockStack.back();
        blockStack.pop_back();
        delete codeGenBlock;
    }

    void setCurrentReturnValue(Value *value)
    {
        blockStack.back()->returnValue = value;
    }

    Value *getCurrentReturnValue()
    {
        return blockStack.back()->returnValue;
    }

    void setArraySize(string name, std::vector<uint64_t> value)
    {
        std::cout << "setArraySize: " << name << ": " << value.size() << std::endl;
        blockStack.back()->arraySizes[name] = value;
        //        cout << "blockStack.back()->arraySizes.size()" << blockStack.back()->arraySizes.size() << endl;
    }

    void PrintSymTable() const
    {
        std::cout << "============ 打印符号表 ============" << std::endl;
        string prefix = "";
        for (auto it = blockStack.begin(); it != blockStack.end(); it++)
        {
            for (auto it2 = (*it)->locals.begin(); it2 != (*it)->locals.end(); it2++)
            {
                std::cout << prefix << it2->first << " = " << it2->second << ": " << this->getSymbolType(it2->first)->name << std::endl;
            }
            prefix += "\t";
        }
        std::cout << "===================================" << std::endl;
    }

    void generateCode(NBlock &);
};

Value *LogErrorV(const char *str);
Value *LogErrorV(string str);

#endif