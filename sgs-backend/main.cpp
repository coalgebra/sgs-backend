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
	// FunctionType* fun_type =
	// 	FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type*>(2, Type::getInt32Ty(TheContext)), false);
	// //
	// Function* fun = Function::Create(fun_type, GlobalValue::InternalLinkage, "function", TheModule.get());
	// // // fun->print(outs(), nullptr);
	// // // TheModule.print(outs(), nullptr);
	// std::vector<Value*> args;
	// int cnt = 0;
	// std::string xx[] = { "a", "b" };
	// for (auto& arg : fun->args()) { arg.setName(xx[cnt++]); args.push_back(&arg); }
 //
	// BasicBlock* bb = BasicBlock::Create(TheContext, "entry", fun);
	// Builder.SetInsertPoint(bb);
	// Value* constant2 = Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 2));
	// Value* constant48 = Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 48));
	// Value* a = Builder.CreateAdd(constant2, constant48, "w");
	// FunctionType* putchar_type =
	// 	FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type*>(1, Type::getInt32Ty(TheContext)), false);
 //
	// Function* fputchar = Function::Create(putchar_type, GlobalValue::ExternalLinkage, "putchar", TheModule.get());
	// Value* b = Builder.CreateAdd(a, args[0], "x");
	// Value* c = Builder.CreateAdd(b, args[1], "y");
	// Value* z = Builder.CreateCall(fputchar, std::vector<Value*>(1, c), "z");
	// Builder.CreateRet(z);
	// // fun->print(outs(), nullptr);

	Type* aryType = ArrayType::get(Type::getInt32Ty(TheContext), 10);

	ConstantAggregateZero* const_array_2 = ConstantAggregateZero::get(aryType);
	GlobalVariable* ga = new GlobalVariable(
		*TheModule, 
		Type::getInt32Ty(TheContext), 
		false, 
		GlobalValue::CommonLinkage,
		Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 0)),
		// nullptr,
		"a");
	// theModule->getGlobalList();/
	// TheModule->getGlobalList().push_back(ga);

	Type* strType = ArrayType::get(Type::getInt8Ty(TheContext), 4);

	Constant* constStr = ConstantDataArray::getString(TheContext, "123", true);

	GlobalVariable* strb = new GlobalVariable(*TheModule, strType, true, GlobalValue::PrivateLinkage, constStr, "fucker");
	// theModule.getstr
	// Value* glbary = Builder.CreateGlobalString("fucker");
	GlobalVariable* gb = new GlobalVariable(*TheModule, aryType, false, GlobalValue::CommonLinkage,
		const_array_2
		, "motherfucker");

	auto* gx = ConstantExpr::getInBoundsGetElementPtr(
		aryType, 
		gb, vector<Constant*>({
		Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 0)) ,
		Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 0))
		}));

	GlobalVariable* gxx = new GlobalVariable(
		*TheModule, 
		Type::getInt32PtrTy(TheContext), 
		false, 
		GlobalValue::CommonLinkage, 
		gx, 
		"momfucker");

	// Constant* gx = ConstantExpr::getInBoundsGetElementPtr()
	// StructType* newType = StructType::create(TheContext, vector<Type*>(2, Type::getInt32Ty(TheContext)), "fucker", false);
 //
	// Type* stPtr = PointerType::get(newType, 0);
 //
	// FunctionType* fuckType =
	// 	FunctionType::get(Type::getInt32PtrTy(TheContext), std::vector<Type*>(1, stPtr), false);
 //
	// Function* fuck = Function::Create(fuckType, GlobalValue::InternalLinkage, "fuck", TheModule.get());
	// BasicBlock* bb2 = BasicBlock::Create(TheContext, "entry", fuck);
	// Value* fuckerA = (fuck->args().begin());
	// fuckerA->setName("a");
	// Builder.SetInsertPoint(bb2);
	// Value* first = Builder.CreateStructGEP(newType, fuckerA, 0, "first");
	// Value* valueOfFirst = Builder.CreateLoad(first, "first.value");
	// Builder.CreateCall(fputchar, vector<Value*>(1, valueOfFirst));
	// // Value* getStructPtr = Builder.create
	// Value* getary = Builder.CreateGEP(gb, vector<Value*>(2, Constant::getIntegerValue(Type::getInt32Ty(TheContext), APInt(32, 0))), "aryelm");
	// Value* getaryvalue = Builder.CreateLoad(getary, "ary.value");
	// // Builder.CreateCall(fputchar, vector<Value*>(1, getaryvalue));
	// Builder.CreateRet(getary);



	TheModule->print(outs(), nullptr);
	// TheModule = llvm::make_unique<Module>("my cool jit", TheContext);

	// Run the main "interpreter loop" now.
	// MainLoop();

	// Print out all of the generated code.
	// TheModule.print(errs(), nullptr);
}


void simpleTest() {
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
	stmts.push_back(new AssignStmt(b, new BinopExp(ADD, a, b)));
	stmts.push_back(new AssignStmt(c, new BinopExp(GT, b, a)));
	Statement* t1 = new AssignStmt(b, getLiteral(5));
	Statement* t2 = new AssignStmt(b, getLiteral(3));
	stmts.push_back(new IfStmt(c, t1, t2));
	stmts.push_back(new ExpStmt(new CallExp("printNum", { b }, createIntType())));
	stmts.push_back(new ReturnStmt(b));
	BlockStmt* body = new BlockStmt(stmts);
	auto* funcDef = new FuncDef(proto, body);
	// codegen(funcDef);
	code.push_back(funcDef);
	totalTranslation(code, "simpleTest.ll");
}

void floatIntTest() {
	/*  int main() {
	 *		int i = 1;
	 *		float fact = 1;
	 *		float res = 1.0;
	 *		while(i < 11) {
	 *			res = res + 1.0 / fact;
	 *			fact = fact * intToFloat(i);
	 *			i = i + 1;
	 *		}
	 *		printFloat(res);
	 *		return 0;
	 *  }
	 * */
	Context code;
	SType* intType = createIntType();
	SType* floatType = createFloatType();
	SType* boolType = createBoolType();
	FuncProto* proto = new FuncProto(intType, "main", {});
	vector<Statement*> stmts;

	stmts.push_back(new VarDefStmt(intType, getLiteral(1), "i"));
	stmts.push_back(new VarDefStmt(floatType, getLiteral(1.0), "fact"));
	stmts.push_back(new VarDefStmt(floatType, getLiteral(0.0), "res"));
	vector<Statement*> stmts2;
	stmts2.push_back(new AssignStmt(new IdExp("res", floatType), 
		new BinopExp(ADD, new IdExp("res", floatType), 
			new BinopExp(DIV, getLiteral(1.0), new IdExp("fact", floatType)))));
	stmts2.push_back(new AssignStmt(new IdExp("fact", floatType),
		new BinopExp(MUL,  new IdExp("fact", floatType), 
			new CallExp("intToFloat", {new IdExp("i", intType)}, floatType))));
	stmts2.push_back(new AssignStmt(new IdExp("i", intType),
		new BinopExp(ADD, new IdExp("i", intType), getLiteral(1))));
	stmts.push_back(new WhileStmt(new BinopExp(LT, new IdExp("i", floatType), getLiteral(15)), new BlockStmt(stmts2)));
	stmts.push_back(new ExpStmt(new CallExp("printFloat", { new IdExp("res", floatType) }, intType)));
	stmts.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* block = new BlockStmt(stmts);
	FuncDef* func = new FuncDef(proto, block);
	code.push_back(func);
	totalTranslation(code, "floatInt.ll");
}

void localArrayTest() {
	/**
	 *  int printFirst(int* a) {
	 *		return printNum(a[0]);
	 *  }
	 *
	 *	int main() {
	 *		int a[10];
	 *		a[0] = 1;
	 *		a[1] = a[0] + 5;
	 *		printNum(a[1]);
	 *		return 0;
	 *	}
	 */
	Context code;
	SType* intType = createIntType();
	SType* aryType = new SArrayType(intType, 10);
	FuncProto* proto1 = new FuncProto(intType, "printFirst", {std::make_pair(new SArrayType(intType, 10), "a")});
	BlockStmt* block2 = new BlockStmt({ new ReturnStmt(new CallExp("printNum", { new VisitExp(new IdExp("a", aryType), getLiteral(0))}, intType))});
	FuncDef* printFirst = new FuncDef(proto1, block2);
	code.push_back(printFirst);
	FuncProto* proto = new FuncProto(intType, "main", {});
	vector<Statement*> stmts;
	Expression* a = new IdExp("a", aryType);
	stmts.push_back(new VarDefStmt(aryType, nullptr, "a"));
	stmts.push_back(new AssignStmt(new VisitExp(a, getLiteral(0)), getLiteral(1)));
	stmts.push_back(new AssignStmt(new VisitExp(a, getLiteral(1)),
		new BinopExp(ADD, new VisitExp(a, getLiteral(0)), getLiteral(5))));
	stmts.push_back(new ExpStmt(new CallExp("printNum", { new VisitExp(a, getLiteral(1)) }, intType)));
	stmts.push_back(new ExpStmt(new CallExp("printFirst", { a }, intType)));
	stmts.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* block = new BlockStmt(stmts);
	FuncDef* func = new FuncDef(proto, block);
	code.push_back(func);
	totalTranslation(code, "localArray.ll");
}

void glbVarDefTest() {
	/** int a;
	 *	int b[10];
	 *	
	 *	int visitGArray(int i) {
	 *		return b[i];
	 *	}
	 *	
	 *	int writeGArray(int* b, int i, int v) {
	 *		b[i] = v;
	 *		return 0;
	 *	}
	 *	
	 *	int main() {
	 *		writeGArray(b, 1, 2);
	 *		printNum(visitGArray(1));
	 *		return 0;
	 *	}
	 */
	Context code;
	SType* intType = createIntType();
	SType* aryType = new SArrayType(intType, 10);
	SType* aryType2 = new SArrayType(intType, 0);

	GlobalVarDef* aDef = new GlobalVarDef("a", intType);
	GlobalVarDef* bDef = new GlobalVarDef("b", aryType);

	code.push_back(aDef);
	code.push_back(bDef);

	 FuncProto* vaProto = new FuncProto(intType, "visitGArray", { std::make_pair(intType, "i") });
	 vector<Statement*> vaStmt;
	 vaStmt.push_back(new ReturnStmt(new VisitExp(new IdExp("b", aryType), new IdExp("i", intType))));
	 BlockStmt* vaBody = new BlockStmt(vaStmt);
	 code.push_back(new FuncDef(vaProto, vaBody));
 
	 FuncProto* waProto = new FuncProto(intType, "writeGArray", { std::make_pair(aryType2, "b"), std::make_pair(intType, "i") , std::make_pair(intType, "v") });
	 vector<Statement*> waStmt;
	 waStmt.push_back(new AssignStmt(new VisitExp(new IdExp("b", aryType2), new IdExp("i", intType)), new IdExp("v", intType)));
	 waStmt.push_back(new ReturnStmt(getLiteral(0)));
	 BlockStmt* waBody = new BlockStmt(waStmt);
	 code.push_back(new FuncDef(waProto, waBody));
 
	 FuncProto* mainProto = new FuncProto(intType, "main", {});
	 vector<Statement*> mainStmt;
	 mainStmt.push_back(new ExpStmt(new CallExp("writeGArray", { new IdExp("b", aryType), getLiteral(1), getLiteral(2)}, intType)));
	 mainStmt.push_back(new ExpStmt(new CallExp("printNum", { new CallExp("visitGArray", {getLiteral(1)}, intType) }, intType)));
	 mainStmt.push_back(new ReturnStmt(getLiteral(0)));
	 BlockStmt* mainBody = new BlockStmt(mainStmt);
	 code.push_back(new FuncDef(mainProto, mainBody));

	totalTranslation(code, "glbVarDef.ll");
}

void stringSimpleTest() {
	/** 
	 *	int main() {
	 *		char[] x = "123"; 
	 *		x[0] = x[0] + 1;
	 *		printStr(x);
	 *		return 0;
	 *	}
	 */
	SType* intType = createIntType();
	Context code;
	FuncProto* mainProto = new FuncProto(intType, "main", {});
	vector<Statement*> mainStmt;
	SType* strType = new SArrayType(createCharType(), 4);
	mainStmt.push_back(new VarDefStmt(strType, nullptr, "x"));
	mainStmt.push_back(new AssignStmt(new IdExp("x", strType), new ConstString("123")));
	mainStmt.push_back(new AssignStmt(new VisitExp(new IdExp("x", strType), getLiteral(0)),
		new BinopExp(ADD, new VisitExp(new IdExp("x", strType), getLiteral(0)), getLiteral(1))));
	mainStmt.push_back(new ExpStmt(new CallExp("printStr", { new IdExp("x", strType) }, intType)));
	mainStmt.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* mainBody = new BlockStmt(mainStmt);
	code.push_back(new FuncDef(mainProto, mainBody));
	totalTranslation(code, "stringSimpleTest.ll");
}

void simpleStructTest() {

	/** 
	 *  struct fucker {
	 *		int a;
	 *		char b;
	 *		int c;
	 *  };
	 *  
	 *  int printFucker(struct fucker* f) {
	 *		printNum(f->a);
	 *		putchar(' ');
	 *		putchar(f->b);
	 *		putchar(' ');
	 *		printNum(f->c);
	 *		return 0;
	 *  }
	 *  
	 *  int main() {
	 *		struct fucker a;
	 *		a.a = 1;
	 *		a.b = 'G';
	 *		a.c = 10;
	 *		while(a.c > 0) {
	 *			putchar(a.b);
	 *			a.a = a.a + a.c;
	 *			a.c = a.c - 1;
	 *		}
	 *		printNum(a.c);
	 *		printFucker(&a);
	 *		return 0;
	 *  }
	 *  
	 */

	SType* intType = createIntType();
	SType* charType = createCharType();
	SType* fuckerType = new STupleType({std::make_pair("a", intType), std::make_pair("b", charType), std::make_pair("c", intType)}, "fucker");
	Context code;

	code.push_back(new TypeDef(fuckerType, "fucker"));
	Expression* a = new IdExp("a", fuckerType);
	FuncProto* printFuckerProto = new FuncProto(intType, "printFucker", { std::make_pair(fuckerType, "a") });
	vector<Statement*> printFuckerStmt;
	printFuckerStmt.push_back(new ExpStmt(new CallExp("printNum", { new AccessExp(a, "a") }, intType)));
	printFuckerStmt.push_back(new ExpStmt(new CallExp("putchar", { getLiteral(' ')}, intType)));
	printFuckerStmt.push_back(new ExpStmt(new CallExp("putchar", { new AccessExp(a, "b") }, intType)));
	printFuckerStmt.push_back(new ExpStmt(new CallExp("putchar", { getLiteral(' ')}, intType)));
	printFuckerStmt.push_back(new ExpStmt(new CallExp("printNum", { new AccessExp(a, "c") }, intType)));
	printFuckerStmt.push_back(new ReturnStmt(getLiteral(0)));
	code.push_back(new FuncDef(printFuckerProto, new BlockStmt(printFuckerStmt)));
	FuncProto* mainProto = new FuncProto(intType, "main", {});
	vector<Statement*> mainStmt;
	
	mainStmt.push_back(new VarDefStmt(fuckerType, nullptr, "a"));
	mainStmt.push_back(new AssignStmt(new AccessExp(a, "a"), getLiteral(1)));
	mainStmt.push_back(new AssignStmt(new AccessExp(a, "b"), getLiteral('G')));
	mainStmt.push_back(new AssignStmt(new AccessExp(a, "c"), getLiteral(10)));
	mainStmt.push_back(new WhileStmt(new BinopExp(GT, new AccessExp(a, "c"), getLiteral(0)),
		new BlockStmt({
			new ExpStmt(new CallExp("putchar", {new AccessExp(a, "b")}, intType)),
			new AssignStmt(new AccessExp(a, "a"), new BinopExp(ADD, new AccessExp(a, "a"), getLiteral(1))),
			new AssignStmt(new AccessExp(a, "c"), new BinopExp(SUB, new AccessExp(a, "c"), getLiteral(1)))
		})));
	mainStmt.push_back(new ExpStmt(new CallExp("printFucker", {a}, intType)));
	mainStmt.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* mainBody = new BlockStmt(mainStmt);
	code.push_back(new FuncDef(mainProto, mainBody));

	totalTranslation(code, "simpleStructTest.ll");
}

void complexTypeTest1() {
	/** 
	 *  This is a test for basic array inside a structure
	 *  
	 *  and here is the corresponding C code
	 *  
	 *  struct fucker {
	 *		int a;
	 *		char b[10];
	 *		int c;
	 *	};
	 *	
	 *	int main() {
	 *		struct fucker a;
	 *		a.b = "123";
	 *		a.b[1] = 'a';
	 *		a.c = 6;
	 *		printStr(a.b);
	 *	}
	 */
	SType* intType = createIntType();
	SType* charType = createCharType();
	SType* aryType = new SArrayType(charType, 10);
	SType* fuckerType = new STupleType({ std::make_pair("a", intType), std::make_pair("b", aryType), std::make_pair("c", intType) }, "fucker");
	Context code;

	code.push_back(new TypeDef(fuckerType, "fucker"));
	Expression* a = new IdExp("a", fuckerType);
	
	FuncProto* mainProto = new FuncProto(intType, "main", {});
	vector<Statement*> mainStmt;

	mainStmt.push_back(new VarDefStmt(fuckerType, nullptr, "a"));
	mainStmt.push_back(new AssignStmt(new AccessExp(a, "b"), new ConstString("123")));
	mainStmt.push_back(new AssignStmt(new AccessExp(a, "c"), getLiteral(6)));
	mainStmt.push_back(new AssignStmt(new VisitExp(new AccessExp(a, "b"), getLiteral(1)), getLiteral('a')));
	mainStmt.push_back(new ExpStmt(new CallExp("printStr", { new AccessExp(a, "b") }, intType)));
	mainStmt.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* mainBody = new BlockStmt(mainStmt);
	code.push_back(new FuncDef(mainProto, mainBody));

	totalTranslation(code, "complexTypeTest1.ll");
}

void complexTypeTest2() {
	/** This is a test for structure array
	 *  struct Fucker {
	 *		int a;
	 *		char b[10];
	 *		int c;
	 *  };
	 *  
	 *  int printFucker(struct Fucker* a, int i) {
	 *		return printStr(a[i].b);
	 *  }
	 *  
	 *  int main() {
	 *		struct Fucker fuckers[10];
	 *		fuckers[0].b = "12345";
	 *		printFucker(fuckers, 0);
	 *		return 0;
	 *  }
	 */

	SType* intType = createIntType();
	SType* charType = createCharType();
	SType* aryType = new SArrayType(charType, 10);
	SType* fuckerType = new STupleType({ std::make_pair("a", intType), std::make_pair("b", aryType), std::make_pair("c", intType) }, "Fucker");
	SType* arFuckerType = new SArrayType(fuckerType, 10);

	Context code;

	code.push_back(new TypeDef(fuckerType, "Fucker"));

	IdExp* a = new IdExp("a", arFuckerType);
	IdExp* i = new IdExp("i", intType);

	FuncProto* printFuckerProto = new FuncProto(intType, "printFuckers", {std::make_pair(arFuckerType, "a"), std::make_pair(intType, "i")});
	code.push_back(new FuncDef(printFuckerProto,
		new BlockStmt({
			new ReturnStmt(new CallExp("printStr",
				{new AccessExp(new VisitExp(a, i), "b") }, intType))
		})));

	FuncProto* mainProto = new FuncProto(intType, "main", {});
	vector<Statement*> mainStmt;
	mainStmt.push_back(new VarDefStmt(arFuckerType, nullptr, "fuckers"));
	IdExp* fuckers = new IdExp("fuckers", arFuckerType);
	mainStmt.push_back(new AssignStmt(
		new AccessExp(new VisitExp(fuckers, getLiteral(0)), "b"), new ConstString("12345")));
	mainStmt.push_back(new ExpStmt(new CallExp("printFuckers", {fuckers, getLiteral(0)}, intType)));
	mainStmt.push_back(new ReturnStmt(getLiteral(0)));
	BlockStmt* mainBody = new BlockStmt(mainStmt);
	code.push_back(new FuncDef(mainProto, mainBody));

	totalTranslation(code, "complexTypeTest2.ll");
}

void test() {
	simpleTest();
	floatIntTest();
	localArrayTest();
	glbVarDefTest();
	stringSimpleTest();
	simpleStructTest();
	complexTypeTest1();
	complexTypeTest2();
}

int main() {
	// test();
	// temp();
	// puts("hell world");
	// std::cin >> n;
	// n = n + 1;
	return 0;
}
