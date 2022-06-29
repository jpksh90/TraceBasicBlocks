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
    struct Tracer: public ModulePass {
        static char ID;
        Tracer() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            LLVMContext &context = M.getContext();
            for (Function &F : M) {
                if (!F.isDeclaration()) {
                    LLVM_DEBUG(dbgs() << "\n=======INSTRUMENTING " << F.getName() << "=====\n");
                    LLVM_DEBUG( dbgs() << "Instrumenting Function " << F.getName() << '\n');
                    for (BasicBlock &B : F) {
                        IRBuilder<> builder(&B);
                        builder.SetInsertPoint(&B, B.getFirstInsertionPt());
                        FunctionCallee incrementCtr = M.getOrInsertFunction("incrementCounter",
                                                                            Type::getVoidTy(context));
                        builder.CreateCall(incrementCtr);
                    }
                    NbFuncInst++;
                    LLVM_DEBUG(dbgs() << "\n======= INSTRUMENTING " << F.getName() << "========\n");
                }
            }

            // Add to terminator instruction
            Function* Main = M.getFunction("main");
            for(BasicBlock* B : terminatorBlocks(*Main)) {
                IRBuilder<> builder(B);
                builder.SetInsertPoint(B, B->getFirstInsertionPt());
                FunctionCallee writeBBInfo = M.getOrInsertFunction("writeBBInfo", Type::getVoidTy(context));
                builder.CreateCall(writeBBInfo);
            }
            return false;
        }

        std::vector<BasicBlock*> terminatorBlocks(Function &F) {
            std::vector<BasicBlock*> terminatorBlks;
            for(BasicBlock &B : F) {
                for (Instruction &I : B) {
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
