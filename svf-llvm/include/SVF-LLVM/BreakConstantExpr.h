//===- BreakConstantGEPs.h - Change constant GEPs into GEP instructions --- --//
//
//                          The SAFECode Compiler
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This pass changes all GEP constant expressions into GEP instructions.  This
// permits the rest of SAFECode to put run-time checks on them if necessary.
//
//===----------------------------------------------------------------------===//

#ifndef BREAKCONSTANTGEPS_H
#define BREAKCONSTANTGEPS_H

#include "SVFIR/SVFValue.h"
#include "llvm/IR/PassManager.h"

namespace SVF
{

//
// Pass: BreakConstantGEPs
//
// Description:
//  This pass modifies a function so that it uses GEP instructions instead of
//  GEP constant expressions.
//
class BreakConstantGEPs : public ModulePass
{
private:
    // Private methods

    // Private variables

public:
    static char ID;
    BreakConstantGEPs() : ModulePass(ID) {}
    llvm::StringRef getPassName() const
    {
        return "Remove Constant GEP Expressions";
    }
    virtual bool runOnModule (Module & M);
};


//
// Pass: MergeFunctionRets
//
// Description:
//  This pass modifies a function so that each function only have one unified exit basic block
//
class MergeFunctionRets : public ModulePass
{
private:
    // Private methods

    // Private variables

public:
    static char ID;
    MergeFunctionRets() : ModulePass(ID) {}
    llvm::StringRef getPassName() const
    {
        return "unify function exit into one dummy exit basic block";
    }
    virtual bool runOnModule (Module & M)
    {
        UnifyFunctionExit(M);
        return true;
    }
    inline void UnifyFunctionExit(Module& module)
    {
        std::unique_ptr<UnifyFunctionExitNodes> unify_pass = std::make_unique<UnifyFunctionExitNodes>();
        std::unique_ptr<llvm::FunctionAnalysisManager> FAM = std::make_unique<llvm::FunctionAnalysisManager>();
        for (Module::const_iterator iter = module.begin(), eiter = module.end();
                iter != eiter; ++iter)
        {
            const Function& fun = *iter;
            if(fun.isDeclaration())
                continue;
            unify_pass->run(const_cast<Function&>(fun), *FAM);
        }
    }
};

} // End namespace SVF

#endif
