/*
 * Program to produce the dynamic traces in the LLVM framework. Each trace in a program is assigned a tag based on the
 * origin.
 *
 * Author: Jyoti Prakash
 */

#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Instrumentation.h"
#include "llvm/ADT/Statistic.h"

#include <vector>
#include <set>
#include <utility>

using std::set;
using std::pair;


#define DEBUG_TYPE "tracer"

using namespace llvm;
using std::vector;
using std::string;

STATISTIC(NbFuncInst, "Number of Functions Instrumented");
STATISTIC(NbInstrCalls, "Number of Instrumented Calls");

namespace {
    struct Tracer : public ModulePass {
        static char ID;

        Tracer() : ModulePass(ID) {}

        GlobalVariable *addGlobalCounter(Module &M) {
            LLVMContext &context = M.getContext();
            IRBuilder<> builder(context);
            GlobalVariable *counter = new GlobalVariable(M,
                                                         builder.getInt64Ty(),
                                                         false,
                                                         llvm::GlobalValue::InternalLinkage,
                                                         builder.getInt64(0),
                                                         "__counter");
            return counter;
        }

        void incrementCounter(Module &M, BasicBlock &B, GlobalVariable *counter) {
            IRBuilder<> builder(&B);
            builder.SetInsertPoint(&B, B.getFirstInsertionPt());
            LoadInst *loadInst = builder.CreateLoad(counter);
            Value *incrementIsnt = builder.CreateAdd(loadInst, builder.getInt64(1));
            StoreInst *storeInst = builder.CreateStore(incrementIsnt, counter);
        }


        void printValues(Module &M, BasicBlock &B, GlobalVariable *counter, int numBb) {
            IRBuilder<> builder(&B);
            builder.SetInsertPoint(B.getTerminator());
            // get the load instruction for the counter
            LoadInst *loadInst = builder.CreateLoad(counter);
            PointerType *PtrType = PointerType::getUnqual(Type::getInt8Ty(M.getContext()));
            FunctionType *printfType = FunctionType::get(FunctionType::getInt32Ty(M.getContext()), PtrType, true);
            FunctionCallee printfFunc = M.getOrInsertFunction("printf", printfType);

            // Add the arguments
            std::vector<Value *> arguments;
            arguments = {builder.CreateGlobalStringPtr("\n\n[[TRACES::EXEC_BASIC_BLOCKS::%d]]\n"), loadInst };
            builder.CreateCall(printfFunc, arguments, "call-block");

            // Insert the total number of basic blocs
            arguments = {builder.CreateGlobalStringPtr("\n\n[[TRACES::STATIC_BASIC_BLOCKS::%d]]\n"), builder.getInt32(numBb)};
            builder.CreateCall(printfFunc, arguments);
        }

        bool runOnModule(Module &M) override {
            LLVMContext &context = M.getContext();
            GlobalVariable *var = addGlobalCounter(M);

            unsigned int nbBasicBlocks = 0;
            for (Function &F: M) {
                if (!F.isDeclaration()) {
                    LLVM_DEBUG(dbgs() << "\n=======INSTRUMENTING " << F.getName() << "=====\n");
                    LLVM_DEBUG(dbgs() << "Instrumenting Function " << F.getName() << '\n');
                    for (BasicBlock &B: F) {
                        incrementCounter(M, B, var);
                        NbInstrCalls++;
                        nbBasicBlocks++;
                    }
                    NbFuncInst++;
                    LLVM_DEBUG(dbgs() << "\n======= INSTRUMENTING " << F.getName() << "========\n");
                }
            }

            // Add to terminator instruction to main functions
            Function *Main = M.getFunction("main");
            for (BasicBlock *B: terminatorBlocks(*Main)) {
                printValues(M, *B, var, nbBasicBlocks);
            }
            return false;
        }

        std::vector<BasicBlock *> terminatorBlocks(Function &F) {
            std::vector<BasicBlock *> terminatorBlks;
            for (BasicBlock &B: F) {
                for (Instruction &I: B) {
                    if (ReturnInst *ri = dyn_cast<ReturnInst>(&I)) {
                        terminatorBlks.push_back(&B);
                    }
                }
            }
            return terminatorBlks;
        }
    };
} //anonymous namespace

char Tracer::ID = 0;
static RegisterPass<Tracer> X("tracer", "Tracing pass", false, false);

static RegisterStandardPasses Y(
        PassManagerBuilder::EP_OptimizerLast,
        [](const PassManagerBuilder &Builder,
           legacy::PassManagerBase &PM) { PM.add(new Tracer()); });
