#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG  // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl;  // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;  // 指明命名空间为llvm
#define CONST(num) \
  ConstantInt::get(context, APInt(32, num))  //得到常数值的表示,方便后面多次用到

int main(){
    LLVMContext context;
    Type *TYPE32 = Type::getInt32Ty(context);
    IRBuilder<> builder(context);
    auto module = new Module("if",context);

    //main函数
    auto mainFun = Function::Create(FunctionType::get(TYPE32,false),
                                    GlobalValue::LinkageTypes::ExternalLinkage,
                                    "main",module);
    auto entrybb = BasicBlock::Create(context, "entry", mainFun);
    builder.SetInsertPoint(entrybb); //set entry

    //分支使用的BasicBlock预定义
    auto truebb = BasicBlock::Create(context,"true",mainFun);
    auto falsebb = BasicBlock::Create(context,"false",mainFun);

    //分支
    auto icmp = builder.CreateICmpSGT(CONST(2),CONST(1));
    builder.CreateCondBr(icmp,truebb,falsebb);

    //true bb
    builder.SetInsertPoint(truebb);
    builder.CreateRet(CONST(1));  //返回1

    //falsebb
    builder.SetInsertPoint(falsebb);
    builder.CreateRet(CONST(0));  //返回0

    builder.ClearInsertionPoint();
    FILE *fp = fopen("if.ll","w");
    auto fouts = new raw_fd_ostream(fp->_fileno,true);
    module->print(*fouts,nullptr);  //输出
    delete fouts;
    delete module;
    return 0;
}