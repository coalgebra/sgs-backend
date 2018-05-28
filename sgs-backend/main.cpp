#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "ast.h"
#include "TypeSys.h"
#include "codegen.h"

using std::vector;
using namespace llvm;
using namespace sgs_backend;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
// static Module TheModule("main", TheContext);

void temp() {
	std::unique_ptr<Module> TheModule = std::make_unique<Module>("main module", TheContext);
	FunctionType* fun_type =
		FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type*>(2, Type::getInt32Ty(TheContext)), false);
	//
	Function* fun = Function::Create(fun_type, GlobalValue::InternalLinkage, "function", TheModule.get());
	// // fun->print(outs(), nullptr);
	// // TheModule.print(outs(), nullptr);
	std::vector<Value*> args;
	int cnt = 0;
	std::string xx[] = { "a", "b" };
	for (auto& arg : fun->args()) { arg.setName(xx[cnt++]); args.push_back(&arg); }

	BasicBlock* bb = BasicBlock::Create(TheContext, "entry", fun);
	Builder.SetInsertPoint(bb);
	Value* constant2 = Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 2));
	Value* constant48 = Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 48));
	Value* a = Builder.CreateAdd(constant2, constant48, "w");
	FunctionType* putchar_type =
		FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type*>(1, Type::getInt32Ty(TheContext)), false);

	Function* fputchar = Function::Create(putchar_type, GlobalValue::ExternalLinkage, "putchar", TheModule.get());
	Value* b = Builder.CreateAdd(a, args[0], "x");
	Value* c = Builder.CreateAdd(b, args[1], "y");
	Value* z = Builder.CreateCall(fputchar, std::vector<Value*>(1, c), "z");
	Builder.CreateRet(z);
	// fun->print(outs(), nullptr);

	Type* aryType = ArrayType::get(Type::getInt32Ty(TheContext), 10);

	ConstantAggregateZero* const_array_2 = ConstantAggregateZero::get(aryType);

	Value* glbary = Builder.CreateGlobalString("fucker");
	GlobalVariable* gb = new GlobalVariable(*TheModule, aryType, false, GlobalValue::CommonLinkage,
		const_array_2
		, "motherfucker");

	StructType* newType = StructType::create(TheContext, vector<Type*>(2, Type::getInt32Ty(TheContext)), "fucker", false);

	Type* stPtr = PointerType::get(newType, 0);

	FunctionType* fuckType =
		FunctionType::get(Type::getInt32PtrTy(TheContext), std::vector<Type*>(1, stPtr), false);

	Function* fuck = Function::Create(fuckType, GlobalValue::InternalLinkage, "fuck", TheModule.get());
	BasicBlock* bb2 = BasicBlock::Create(TheContext, "entry", fuck);
	Value* fuckerA = (fuck->args().begin());
	fuckerA->setName("a");
	Builder.SetInsertPoint(bb2);
	Value* first = Builder.CreateStructGEP(newType, fuckerA, 0, "first");
	Value* valueOfFirst = Builder.CreateLoad(first, "first.value");
	Builder.CreateCall(fputchar, vector<Value*>(1, valueOfFirst));
	// Value* getStructPtr = Builder.create
	Value* getary = Builder.CreateGEP(glbary, vector<Value*>(2, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 0))), "aryelm");
	// Value* getaryvalue = Builder.CreateLoad(getary, "ary.value");
	// Builder.CreateCall(fputchar, vector<Value*>(1, getaryvalue));
	Builder.CreateRet(getary);



	TheModule->print(outs(), nullptr);
	// TheModule = llvm::make_unique<Module>("my cool jit", TheContext);

	// Run the main "interpreter loop" now.
	// MainLoop();

	// Print out all of the generated code.
	// TheModule.print(errs(), nullptr);
}


void simple() {
	// builtinFuncInit();
	/** C code : 
	 *	int fucker(int a) {
	 *		int b = 1;
	 *		bool c = false;
	 *		b = b + a;
	 *		c = b > a;
	 *		if (c) {
	 *			b = 5;
	 *		} else {
	 *			b = 3;
	 *		}
	 *		return b;
	 *	}
	 */

	Context code;
	SType* intType = createIntType();
	FuncProto* proto = 
		new FuncProto(intType, "main", 
			vector<pair<SType*, string>>({ std::make_pair(intType, string("a")) }));
	vector<Statement*> stmts;
	stmts.push_back(new VarDefStmt(createIntType(), getLiteral(1), "b"));
	stmts.push_back(new VarDefStmt(createBoolType(), getLiteral(false), "c"));
	auto b = new IdExp("b", createIntType());
	auto c = new IdExp("c", createIntType());
	auto a = new IdExp("a", createIntType());
	stmts.push_back(new AssignStmt(b, new BinopExp(ADD, createIntType(), a, b)));
	// stmts.push_back(new AssignStmt(c, new BinopExp(GT, createBoolType(), b, a)));
	// Statement* t1 = new AssignStmt(b, getLiteral(5));
	// Statement* t2 = new AssignStmt(b, getLiteral(3));
	// stmts.push_back(new IfStmt(c, t1, t2));
	stmts.push_back(new ExpStmt(new CallExp("printNum", { b }, createIntType())));
	stmts.push_back(new ReturnStmt(b));
	BlockStmt* body = new BlockStmt(stmts);
	auto* funcDef = new FuncDef(proto, body);
	// codegen(funcDef);
	code.push_back(funcDef);
	totalTranslation(code);
}

int main() {

	simple();
	// temp();
	// int n;
	// puts("hell world");
	// std::cin >> n;
	// n = n + 1;
	return 0;
}
