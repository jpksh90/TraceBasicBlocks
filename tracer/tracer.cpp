//
// Created by Jyoti Prakash on 22.06.22.
//

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
static cl::opt<string> TraceFile("trace-file", cl::desc("Output trace file"), cl::value_desc("filename"), cl::Required);

namespace {

    enum POS {
        BEGIN,
        END
    };

    struct Tracer: public ModulePass {
        static char ID;
        Tracer() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            LLVMContext &context = M.getContext();
            FunctionCallee writeBBInfoFunc = M.getOrInsertFunction("writeBBInfo", Type::getVoidTy(context), Type::getInt8PtrTy(context), /* output trace file */ Type::getInt8PtrTy(context)); /* message */
            for (Function &F : M) {
                if (!F.isDeclaration()) {
                    LLVM_DEBUG(dbgs() << "\n=======INSTRUMENTING " << F.getName() << "=====\n");
                    LLVM_DEBUG( dbgs() << "Instrumenting Function " << F.getName() << '\n');
                    for (BasicBlock &B : F) {
//                        InsertPrintfBlock(B,F,M,BEGIN);
                        InsertBasicBlockInfo(B, writeBBInfoFunc);
                    }
                    NbFuncInst++;
                    LLVM_DEBUG(dbgs() << "\n======= INSTRUMENTING " << F.getName() << "========\n");
                }
            }
            return false;
        }

        void InsertBasicBlockInfo(BasicBlock &B, FunctionCallee function) {
            IRBuilder<> builder(&B);
            builder.SetInsertPoint(&B, B.getFirstInsertionPt());

            //collect the information to write
            vector<Value*> arguments = {};

            auto block_sign = BasicBlockID(B);
            arguments.push_back((Value*) builder.CreateGlobalStringPtr(TraceFile.c_str(), "filename"));
            arguments.push_back((Value*) builder.CreateGlobalStringPtr(block_sign.c_str(), "block_sign"));

            CallInst* call = builder.CreateCall(function, arguments);

            LLVM_DEBUG( dbgs() << "Instrumented" << call->getCalledFunction() );
            LLVM_DEBUG(dbgs() << "Inserted printf calls" << call->getCalledFunction()->getName());
            LLVM_DEBUG(call->dump());

        }

        string BasicBlockID(BasicBlock &bb) {
            std::string str;
            Function *F = bb.getParent();
            raw_string_ostream OS(str);
            bb.printAsOperand(OS, false);
            string BBSignature = F->getName().str() + "@" + OS.str();
            return BBSignature;
        }

        void InsertPrintfBlock(BasicBlock &B, Function &F, Module &M, POS pos) {
            IRBuilder<> builder(&B);
            if (pos == BEGIN) {
                builder.SetInsertPoint(&B, B.getFirstInsertionPt());
            }
            LLVMContext& context = M.getContext();
            GlobalVariable *string_format = createPrivateGlobalForString(M, "%s", true);

            PointerType *PtrType = PointerType::getUnqual(Type::getInt8Ty(context));
            FunctionType *printfType = FunctionType::get(FunctionType::getInt8Ty(context), PtrType, true);
            FunctionCallee printfFunc = M.getOrInsertFunction("printf", printfType);

            //Build the arguments for printf: (Module ID, Function ID, Block ID)
            vector<Value*> arguments = {};
            auto block_sign = BasicBlockID(B);

            arguments.push_back((Value*) builder.CreateGlobalStringPtr("BBB%sBBE\n", "string_fmt"));
            arguments.push_back((Value*) builder.CreateGlobalStringPtr(block_sign, "function_sign"));
            CallInst *call = builder.CreateCall(printfFunc, arguments, "call-block");
            NbInstrCalls++;

            LLVM_DEBUG(dbgs() << "Function " << call->getCalledFunction()->getName()  << "\n"); //<< " created at site " << OS.str() << "\n";
            LLVM_DEBUG(dbgs() << "Inserted printf calls" << call->getCalledFunction()->getName());
            LLVM_DEBUG(call->dump());
        }
    };
} //anonymous namespace

char Tracer::ID = 0;
static RegisterPass<Tracer> X("tracer", "Tracing pass", false, false);

static RegisterStandardPasses Y(
        PassManagerBuilder::EP_OptimizerLast,
        [](const PassManagerBuilder &Builder,
           legacy::PassManagerBase &PM) { PM.add(new Tracer()); });