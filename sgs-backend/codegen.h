#pragma once

#include "ast.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>


namespace sgs_backend {

	static map<string, Type*> typeReference;
	static map<string, Function*> funcReference;
	static LLVMContext theContext;
	static Module* theModule;
	static IRBuilder<> builder(theContext);

	void codegenInit();
	Value* exprCodegen(Expression* exp, Environment* env);
	Value* stmtCodegen(Statement* stmt, Environment* env, BasicBlock* cont, BasicBlock* bk);
	Value* codegen(AST* ast);


}
