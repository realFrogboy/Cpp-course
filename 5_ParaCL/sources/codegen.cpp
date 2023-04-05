#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "ast.hpp"

namespace ast {

void binop_t::codegen(codegen_info &c_info) const {
    llvm::Value* rhs = c_info.remove_result();
    llvm::Value* lhs = c_info.remove_result();
    c_info.add_result(codegen_op(c_info, lhs, rhs));
}

llvm::Value *g_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpSGT(lhs, rhs, "cmpG");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *l_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpSLT(lhs, rhs, "cmpL");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *e_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpEQ(lhs, rhs, "cmpEq");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *ne_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpNE(lhs, rhs, "cmpNe");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *ge_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpSGE(lhs, rhs, "cmpGe");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *le_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    auto tmp = c_info.builder->CreateICmpSLE(lhs, rhs, "cmpLe");
    return c_info.builder->CreateZExt(tmp, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *add_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateAdd(lhs, rhs, "addtmp");
}

llvm::Value *sub_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateSub(lhs, rhs, "subtmp");
}

llvm::Value *mul_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateMul(lhs, rhs, "multmp");
}

llvm::Value *div_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    llvm::Value *res = c_info.builder->CreateSDiv(lhs, rhs, "divtmp");
    return c_info.builder->CreateFPToSI(res, llvm::Type::getInt32Ty(*c_info.context), "castToInt");
}

llvm::Value *remainder_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateSRem(lhs, rhs, "remtmp");
}

llvm::Value *pow_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    llvm::Value* promotedlhs = c_info.builder->CreateUIToFP(lhs, llvm::Type::getFloatTy(*c_info.context));

    std::vector<llvm::Type*> types{llvm::Type::getFloatTy(*c_info.context), llvm::Type::getInt32Ty(*c_info.context)};
    llvm::Function *pow = llvm::Intrinsic::getDeclaration(c_info.Module, llvm::Intrinsic::powi, types);

    std::vector<llvm::Value*> args{promotedlhs, rhs};
    llvm::ArrayRef<llvm::Value*> args_ref(args);
    llvm::Value *res = c_info.builder->CreateCall(pow, args_ref, "callpow");

    return c_info.builder->CreateFPToSI(res, llvm::Type::getInt32Ty(*c_info.context), "castToInt");
}

llvm::Value *b_and_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateAnd(lhs, rhs, "andtmp");
}

llvm::Value *b_or_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateOr(lhs, rhs, "ortmp");
}

llvm::Value *xor_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateXor(lhs, rhs, "xortmp");
}

llvm::Value *l_shift_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateShl(lhs, rhs, "lshltmp");
}

llvm::Value *r_shift_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    return c_info.builder->CreateLShr(lhs, rhs, "shltmp");
}

llvm::Value *l_and_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    llvm::Value* condLhs = c_info.builder->CreateICmpNE(lhs, c_info.builder->getInt32(0), "condLhs");
    llvm::Value* condRhs = c_info.builder->CreateICmpNE(rhs, c_info.builder->getInt32(0), "condrhs");
    llvm::Value* res = c_info.builder->CreateLogicalAnd(condLhs, condRhs, "landtmp");
    return c_info.builder->CreateZExt(res, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value *l_or_t::codegen_op(codegen_info &c_info, llvm::Value *lhs, llvm::Value *rhs) const {
    llvm::Value* condLhs = c_info.builder->CreateICmpNE(lhs, c_info.builder->getInt32(0), "condLhs");
    llvm::Value* condRhs = c_info.builder->CreateICmpNE(rhs, c_info.builder->getInt32(0), "condrhs");
    llvm::Value* res = c_info.builder->CreateLogicalOr(condLhs, condRhs, "lortmp");
    return c_info.builder->CreateZExt(res, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

void scalar_variable::codegen(codegen_info &c_info) const {
    auto search = n_info.scopes.find_all_scopes(name);
    if (search == nullptr) {
        if (n_info.scopes.is_global()) 
            search = n_info.scopes.add_global(name, c_info.builder->getInt32(0));
        else 
            search = n_info.scopes.add_variable(name, c_info.builder->getInt32(0));
        llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
        c_info.add_result(c_info.CreateEntryBlockAlloca(func, name));
    } else 
        c_info.add_result(c_info.builder->CreateLoad(llvm::Type::getInt32Ty(*c_info.context), std::get<llvm::Value*>(search->value), name.c_str()));
}

void var_assign_t::codegen(codegen_info &c_info) const {
    std::string_view name = static_cast<ast::scalar_variable*>(children[0])->get_name();
    auto search = find_name(name);

    llvm::Value* rhs = c_info.remove_result();
    llvm::Value* lhs = c_info.remove_result();

    if (!search->is_init) {
        search->value = lhs;
        c_info.builder->CreateStore(rhs, lhs);
        search->is_init = 1;
    } else {
        c_info.builder->CreateStore(rhs, std::get<llvm::Value*>(search->value));
    }

    c_info.results.push_back(rhs);
}

void pr_increment_t::codegen(codegen_info &c_info) const {
    std::string_view name = static_cast<ast::scalar_variable*>(children[0])->get_name();
    auto search = find_name(name);
    llvm::Value *lhs = c_info.remove_result();
    llvm::Value *inc = c_info.builder->CreateAdd(lhs, c_info.builder->getInt32(1), "inc");
    c_info.builder->CreateStore(inc, std::get<llvm::Value*>(search->value), "loadtmp");
    c_info.add_result(inc);
}

void pr_decrement_t::codegen(codegen_info &c_info) const {
    std::string_view name = static_cast<ast::scalar_variable*>(children[0])->get_name();
    auto search = find_name(name);
    llvm::Value *lhs = c_info.remove_result();
    llvm::Value *dec = c_info.builder->CreateSub(lhs, c_info.builder->getInt32(1), "inc");
    c_info.builder->CreateStore(dec, std::get<llvm::Value*>(search->value), "loadtmp");
    c_info.add_result(dec);
}

llvm::Value * minus_t::codegen_op(codegen_info &c_info, llvm::Value *lhs) const {
    return c_info.builder->CreateSub(c_info.builder->getInt32(0), lhs, "negtmp");
}

llvm::Value * not_t::codegen_op(codegen_info &c_info, llvm::Value *lhs) const {
    auto tmp = c_info.builder->CreateICmpNE(lhs, c_info.builder->getInt32(0));
    auto tmp1 = c_info.builder->CreateXor(tmp, c_info.builder->getTrue(), "lnottmp");
    return c_info.builder->CreateZExt(tmp1, llvm::Type::getInt32Ty(*c_info.context), "boolZExt");
}

llvm::Value * b_not_t::codegen_op(codegen_info &c_info, llvm::Value *lhs) const {
    return c_info.builder->CreateNot(lhs, "nottmp");
}

llvm::Value * abs_t::codegen_op(codegen_info &c_info, llvm::Value *lhs) const {
    llvm::Function *abs = llvm::Intrinsic::getDeclaration(c_info.Module, llvm::Intrinsic::abs, llvm::Type::getInt32Ty(*c_info.context));
    return c_info.builder->CreateCall(abs, lhs, "callabs");
}

void print_t::codegen(codegen_info &c_info) const {
    llvm::Value *res = c_info.results.back();
    llvm::FunctionType *printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*c_info.context), llvm::PointerType::get(llvm::Type::getInt8Ty(*c_info.context), 0), true);
    llvm::FunctionCallee printf = c_info.Module->getOrInsertFunction("printf", printf_type);
    
    auto print_sig = c_info.getGlobalStringPtr("printf_sig");

    std::vector<llvm::Value*> args{print_sig, res};
    c_info.add_result(c_info.builder->CreateCall(printf, args, "printcall"));
}

void get_t::codegen(codegen_info &c_info) const {
    llvm::FunctionType *scanf_type = llvm::FunctionType::get(c_info.builder->getInt32Ty(), true); 
    llvm::FunctionCallee scanf = c_info.Module->getOrInsertFunction("scanf", scanf_type);

    auto scan_sig = c_info.getGlobalStringPtr("scanf_sig");
    
    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    auto tmp = c_info.CreateEntryBlockAlloca(func, "scantmp");

    std::vector<llvm::Value*> args{scan_sig, tmp};
    c_info.builder->CreateCall(scanf, args, "scancall");

    c_info.add_result(c_info.builder->CreateLoad(llvm::Type::getInt32Ty(*c_info.context), tmp, "scanvalue"));
}

void if_t::codegen(codegen_info &c_info) const   {
    llvm::Value *cond = c_info.remove_result();
    cond = c_info.builder->CreateICmpNE(cond, c_info.builder->getInt32(0), "ifcond");

    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*c_info.context, "then", func);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*c_info.context, "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*c_info.context, "if_cont");
    c_info.blocks.push_back({ThenBB, ElseBB, MergeBB});

    c_info.builder->CreateCondBr(cond, ThenBB, ElseBB);
    c_info.builder->SetInsertPoint(ThenBB);
}

void IR_else_t::codegen(codegen_info &c_info) const {
    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    
    auto tp = c_info.blocks.back();

    c_info.setBr(std::get<0>(tp), std::get<2>(tp));
    func->insert(func->end(), std::get<1>(tp));
    c_info.builder->SetInsertPoint(std::get<1>(tp));
}

void IR_if_cont_without_else::codegen(codegen_info &c_info) const   {
    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    
    auto tp = c_info.blocks.back();
    c_info.blocks.pop_back();

    func->insert(func->end(), std::get<1>(tp));
    c_info.setBr(std::get<0>(tp), std::get<1>(tp));
    c_info.builder->SetInsertPoint(std::get<1>(tp));
}

void IR_if_cont_with_else::codegen(codegen_info &c_info) const {
    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();

    auto tp = c_info.blocks.back();
    c_info.blocks.pop_back();

    func->insert(func->end(), std::get<2>(tp));
    c_info.setBr(std::get<1>(tp), std::get<2>(tp));
    c_info.builder->SetInsertPoint(std::get<2>(tp));
}

void first_while_t::codegen(codegen_info &c_info) const {
    llvm::Value *cond = c_info.remove_result();
    cond = c_info.builder->CreateICmpNE(cond, c_info.builder->getInt32(0), "whilecond");

    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*c_info.context, "loop", func);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(*c_info.context, "afterloop");

    c_info.builder->CreateCondBr(cond, LoopBB, AfterBB);
    c_info.Loop_blocks.push_back({LoopBB, AfterBB});

    c_info.builder->SetInsertPoint(LoopBB);
}

void IR_while_end_cond_t::codegen(codegen_info &c_info) const {
    llvm::Value *cond = c_info.remove_result();
    cond = c_info.builder->CreateICmpNE(cond, c_info.builder->getInt32(0), "endcond");

    auto pr = c_info.Loop_blocks.back();
    c_info.Loop_blocks.pop_back();

    c_info.builder->CreateCondBr(cond, pr.first, pr.second);

    llvm::Function *func = c_info.builder->GetInsertBlock()->getParent();
    func->insert(func->end(), pr.second);
    c_info.builder->SetInsertPoint(pr.second);
}

tree_t::codegen_traversal_t::codegen_traversal_t() : traversal_t{}, context{std::make_unique<llvm::LLVMContext>()}, 
            Module{std::make_unique<llvm::Module>("paracl", *context)}, builder{std::make_unique<llvm::IRBuilder<>>(*context)} {
    e_info.context = context.get();
    e_info.Module  = Module.get();
    e_info.builder = builder.get();

    e_info.builder->CreateGlobalStringPtr("%d\n", "printf_sig", 0, e_info.Module);
    e_info.builder->CreateGlobalStringPtr("%d", "scanf_sig", 0, e_info.Module);

    llvm::Function *func = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false), 
                                    llvm::Function::ExternalLinkage, "main", Module.get());
    
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*context, "main.0", func);
    builder->SetInsertPoint(BB);
}

void tree_t::codegen_traversal_t::optimize_functions() const {
    auto fpm = std::make_unique<llvm::legacy::FunctionPassManager>(Module.get());
    fpm->add(llvm::createPromoteMemoryToRegisterPass());
    fpm->add(llvm::createInstructionCombiningPass());
    fpm->add(llvm::createReassociatePass());
    fpm->add(llvm::createGVNPass());
    fpm->add(llvm::createCFGSimplificationPass());

    fpm->doInitialization();

    for (llvm::Function &func : *Module)
        fpm->run(func);

    fpm->doFinalization();
}

void tree_t::codegen_traversal_t::dump() const {
    llvm::verifyModule(*Module);
    Module->print(llvm::errs(), nullptr);

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    Module->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    if (!Target) {
        llvm::errs() << Error;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = std::optional<llvm::Reloc::Model>(llvm::Reloc::Model::PIC_);
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    Module->setDataLayout(TheTargetMachine->createDataLayout());
    Module->setTargetTriple(TargetTriple);

    std::string Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return;
    }

    pass.run(*Module);
    dest.flush();

    delete TheTargetMachine;
}

tree_t::codegen_traversal_t::~codegen_traversal_t() {
    llvm::llvm_shutdown_obj();
}

} // ast