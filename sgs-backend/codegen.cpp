#include "codegen.h"
#include <iostream>

void sgs_backend::builtinFuncInit() {
	// intToChar :: Int => Char
	FunctionType* intToChar = FunctionType::get(Type::getInt8Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["intToChar"] = Function::Create(intToChar, GlobalValue::ExternalLinkage, "intToChar");

	// charToInt :: Char => Int
	FunctionType* charToInt = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8Ty(theContext)}, false);
	funcReference["charToInt"] = Function::Create(charToInt, GlobalValue::ExternalLinkage, "charToInt");

	// intToFloat :: Int => Float
	FunctionType* intToFloat = FunctionType::get(Type::getInt32Ty(theContext), {Type::getFloatTy(theContext)}, false);
	funcReference["intToChar"] = Function::Create(intToFloat, GlobalValue::ExternalLinkage, "intToFloat");

	// floatToInt :: Float => Int
	FunctionType* floatToInt = FunctionType::get(Type::getFloatTy(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["floatToInt"] = Function::Create(floatToInt, GlobalValue::ExternalLinkage, "floatToInt");

	// intToChar :: Int => Char
	FunctionType* intToBool = FunctionType::get(Type::getInt1Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["intToBool"] = Function::Create(intToBool, GlobalValue::ExternalLinkage, "intToBool");

	// charToInt :: Char => Int
	FunctionType* boolToInt = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt1Ty(theContext)}, false);
	funcReference["boolToInt"] = Function::Create(boolToInt, GlobalValue::ExternalLinkage, "boolToInt");

	// printNum :: Int => ()
	FunctionType* printNum = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["printNum"] = Function::Create(printNum, GlobalValue::ExternalLinkage, "printNum");

	// readNum :: () => Int
	FunctionType* readNum = FunctionType::get(Type::getInt32Ty(theContext), false);
	funcReference["readNum"] = Function::Create(readNum, GlobalValue::ExternalLinkage, "readNum");

	// printStr :: String => ()
	FunctionType* printStr = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8PtrTy(theContext)}, false);
	funcReference["printStr"] = Function::Create(printStr, GlobalValue::ExternalLinkage, "printStr");

	// readStr :: String => ()
	FunctionType* readStr = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8PtrTy(theContext)}, false);
	funcReference["readStr"] = Function::Create(readStr, GlobalValue::ExternalLinkage, "readStr");
	builtInFuncs += "\n\
@printNum.constStr = constant [3 x i8] c\"%d\\00\", align 1\n\
@printStr.constStr = constant[3 x i8] c\"%s\\00\", align 1\n\
declare i32 @printf(i8*, ...)\n\
declare i32 @scanf(i8*, ...)\n\
\n\
define i8 @intToChar(i32) {\n\
	%2 = trunc i32 %0 to i8\n\
	ret i8 %2\n\
}\n\
\n\
define i32 @charToInt(i8) {\n\
	%2 = alloca i8, align 1\n\
	store i8 %0, i8* %2, align 1\n\
	%3 = load i8, i8* %2, align 1\n\
	%4 = sext i8 %3 to i32\n\
	ret i32 %4\n\
}\n\
\n\
define i1 @intToBool(i32) {\n\
	%2 = icmp eq i32 %0, 0\n\
	ret i1 %2\n\
}\n\
\n\
define i32 @boolToInt(i1) {\n\
	%2 = sext i1 %0 to i32\n\
	ret i32 %2\n\
}\n\
\n\
define float @intToFloat(i32) {\n\
	%2 = sitofp i32 %0 to float\n\
	ret float %2\n\
}\n\
\n\
define i32 @floatToInt(float) {\n\
	%2 = fptosi float %0 to i32\n\
	ret i32 %2\n\
}\n\
\n\
define i32 @printNum(i32) {\n\
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)\n\
	ret i32 0\n\
}\n\
\n\
define i32 @printStr(i8*) {\n\
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)\n\
	ret i32 0\n\
}\n\
\n\
define i32 @readNum() {\n\
	%1 = alloca i32, align 4\n\
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)\n\
	%3 = load i32, i32* %1, align 4\n\
	ret i32 %3\n\
}\n\
\n\
define i32 @readStr(i8*)  {\n\
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)\n\
	ret i32 0\n\
}";
}

void sgs_backend::codegenInit() {
	typeReference.clear();
	funcReference.clear();
	delete theModule;

	theModule = new Module("wtf", theContext);
}

Value* sgs_backend::exprCodegen(Expression* exp, Environment* env) {
	switch (exp->getExpType()) {
	case ET_BINOP: {
		const auto bexp = dynamic_cast<BinopExp*>(exp);
		Value* lhs = exprCodegen(bexp->getLeft(), env);
		Value* rhs = exprCodegen(bexp->getRigth(), env);
		if (!lhs || !rhs) {
			std::cerr << "Translation Error : when translating <binop-expr> " << std::endl;
			return nullptr;
		}
		if (lhs->getType()->isPointerTy()) {
			lhs = builder.CreateLoad(lhs, "load.val");
		}
		if (rhs->getType()->isPointerTy()) {
			rhs = builder.CreateLoad(rhs, "load.val");
		}

		switch (bexp->getOp()) {
		case AND:
			return builder.CreateAnd(lhs, rhs, "and.res");
		case OR:
			return builder.CreateOr(lhs, rhs, "or.res");
		case ADD:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFAdd(lhs, rhs, "fadd.res");
			}
			if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
				return builder.CreateAdd(lhs, rhs, "add.res");
			}
			return nullptr;
		case SUB:
			if (lhs->getType()->isFloatTy()) {
				return builder.CreateFSub(lhs, rhs, "fsub.res");
			}
			if (lhs->getType()->isIntegerTy(32)) {
				return builder.CreateSub(lhs, rhs, "sub.res");
			}
			return nullptr;
		case MUL:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFMul(lhs, rhs, "fmul.res");
			}
			if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
				return builder.CreateMul(lhs, rhs, "mul.res");
			}
			return nullptr;
		case DIV:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFDiv(lhs, rhs, "fdiv.res");
			}
			if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
				return builder.CreateExactSDiv(lhs, rhs, "div.res");
			}
			return nullptr;
		case GT:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFCmp(CmpInst::Predicate::FCMP_OGT, lhs, rhs, "fgt.res");
			}
			if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
				return builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, lhs, rhs, "gt.res");
			}
			return nullptr;
		case LT:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFCmp(CmpInst::Predicate::FCMP_OLT, lhs, rhs, "flt.res");
			}
			if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
				return builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, lhs, rhs, "lt.res");
			}
			return nullptr;
		default:
			std::cerr << "wtf ??" << std::endl;
		}
	}
	case ET_LITERAL: {
		const auto lit = dynamic_cast<LiteralExp*>(exp);
		switch (lit->getBType()) {
		case BasicType::INTEGER:
		{
			const auto intlit = dynamic_cast<IntLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext), APInt(32, intlit->getValue()));
		}
		case BasicType::FRACTION:
		{
			const auto ftlit = dynamic_cast<FloatLiteral*>(lit);
			return ConstantFP::get(theContext, APFloat(ftlit->getValue()));
		}
		case BasicType::BOOLEAN:
		{
			const auto boolit = dynamic_cast<BoolLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext), APInt(1, boolit->getValue()));
		}
		default:
			std::cerr << "wtf ???" << std::endl;
		}
	}
	case ET_IDENT: {
		const auto idexp = dynamic_cast<IdExp*>(exp);
		Value* id = env->find(idexp->getName());
		return id;
	}
	case ET_VISIT: {
		const auto visitexp = dynamic_cast<VisitExp*>(exp);
		Value* array = exprCodegen(visitexp->getArray(), env);
		Value* index = exprCodegen(visitexp->getIndex(), env);
		if (index->getType()->isPointerTy()) {
			index = builder.CreateLoad(index, "load");
		}
		if (index->getType()->isIntegerTy(1)) {
			index = builder.CreateIntCast(index, Type::getInt32Ty(theContext), true, "icast");
			return builder.CreateInBoundsGEP(
				array, vector<Value*>({Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)), index}), "visit");
		}
		if (index->getType()->isIntegerTy(32)) {
			return builder.CreateInBoundsGEP(
				array, vector<Value*>({Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)), index}), "visit");
		}
		return nullptr;
	}  
	case ET_CALL: {
		const auto callexp = dynamic_cast<CallExp*>(exp);
		Function* fun = funcReference[callexp->getFunction()];
		vector<Value*> params;
		for (auto&& x : callexp->getParam()) {
			auto temp = exprCodegen(x, env);
			if (!temp->getType()->isPointerTy() || x->getResType()->getLevel() != Types::BASIC_TYPE) {
				// do nothing here
			} else {
				temp = builder.CreateLoad(temp, "load");
			}
			params.push_back(temp);
		}
		return builder.CreateCall(fun, params, "call.res");
	}
	case ET_ACCESS: {
		const auto accexp = dynamic_cast<AccessExp*>(exp);
		const string name = accexp->getMember();
		Value* res = exprCodegen(accexp->getObject(), env);
		SType* ty = accexp->getObject()->getResType();
		auto* tp = dynamic_cast<STupleType*>(ty);
		size_t i = 0;
		for (; i < tp->getTypes().size(); i++) {
			if (tp->getTypes()[i].first == name) {
				break;
			}
		}
		if (i == tp->getTypes().size()) {
			std::cerr << "Can't find member " << name << std::endl;
			return nullptr;
		}
		return builder.CreateStructGEP(tp->toLLVMType(theContext), res, i, "access.res");
	}
	default:
		std::cerr << "What the fuck??" << std::endl;
		return nullptr;
	}
}

Value* sgs_backend::stmtCodegen(Statement* stmt, Environment* env, BasicBlock* cont, BasicBlock* bk) {
	switch (stmt->getStmtType()) {
	case ST_ASSIGN: {
		const auto ass = dynamic_cast<AssignStmt*>(stmt);
		Value* lhs = exprCodegen(ass->getLeft(), env);
		Value* rhs = exprCodegen(ass->getRigth(), env);
		if (rhs->getType()->isPointerTy()) {
			rhs = builder.CreateLoad(rhs, "load.val");
		}
		if (!lhs || !rhs) {
			std::cerr << "Translation error : at AssignStmt" << std::endl;
			return nullptr;
		}
		return builder.CreateStore(rhs, lhs);
	}
	case ST_IF: {
		Function* fun = builder.GetInsertBlock()->getParent();
		const auto ifs = dynamic_cast<IfStmt*>(stmt);
		Value* cond = exprCodegen(ifs->getCond(), env);
		if (!cond) {
			std::cerr << "Translation error : at IfStmt, condition translation is failed" << std::endl;
			return nullptr;
		}
		if (cond->getType()->isPointerTy()) {
			cond = builder.CreateLoad(cond, "if.cond.load");
		}
		BasicBlock* taken = BasicBlock::Create(theContext, "if.take", fun);
		BasicBlock* untaken = BasicBlock::Create(theContext, "if.fail");
		BasicBlock* merge = BasicBlock::Create(theContext, "if.merge");
		builder.CreateCondBr(cond, taken, untaken);
		builder.SetInsertPoint(taken);
		stmtCodegen(ifs->getTaken(), env, cont, bk);
		builder.CreateBr(merge);

		fun->getBasicBlockList().push_back(untaken);
		builder.SetInsertPoint(untaken);
		stmtCodegen(ifs->getUntaken(), env, cont, bk);
		const auto res = builder.CreateBr(merge);

		fun->getBasicBlockList().push_back(merge);
		builder.SetInsertPoint(merge);

		return res;
	}
	case ST_WHILE: {
		const auto wstmt = dynamic_cast<WhileStmt*>(stmt);
		Function* fun = builder.GetInsertBlock()->getParent();
		BasicBlock* whileCond = BasicBlock::Create(theContext, "while.cond");
		BasicBlock* whileBody = BasicBlock::Create(theContext, "while.body");
		BasicBlock* whileMerge = BasicBlock::Create(theContext, "while.merge");
		builder.CreateBr(whileCond);
		fun->getBasicBlockList().push_back(whileCond);
		builder.SetInsertPoint(whileCond);
		Value* cond = exprCodegen(wstmt->getCondition(), env);
		builder.CreateCondBr(cond, whileBody, whileMerge);
		fun->getBasicBlockList().push_back(whileBody);
		builder.SetInsertPoint(whileBody);
		stmtCodegen(wstmt->getBody(), env, whileCond, whileMerge);
		const auto res = builder.CreateBr(whileCond);
		fun->getBasicBlockList().push_back(whileMerge);
		builder.SetInsertPoint(whileMerge);
		return res;
	}
	case ST_RETURN: {
		const auto ret = dynamic_cast<ReturnStmt*>(stmt);
		Value* res;
		if (ret->getExp()) {
			const auto exp = exprCodegen(ret->getExp(), env);
			if (exp->getType()->isPointerTy()) {
				const auto res2 = builder.CreateLoad(exp, "ret.load");
				res = builder.CreateRet(res2);
			} else {
				res = builder.CreateRet(exp);
			}
		} else {
			res = builder.CreateRetVoid();
		}
		return res;
	}
	case ST_BREAK: {
		return builder.CreateBr(bk);
	}
	case ST_CONTINUE: {
		return builder.CreateBr(cont);
	}
	case ST_BLOCK: {
		const auto block = dynamic_cast<BlockStmt*>(stmt);
		Environment* new_env = Environment::derive(env);
		Value* res = nullptr;
		for (auto&& x : block->getContent()) {
			res = stmtCodegen(x, new_env, cont, bk);
		}
		return res;
	}
	case ST_EXP: {
		const auto exp = dynamic_cast<ExpStmt*>(stmt);
		return exprCodegen(exp->getExp(), env);
	}
	case ST_VARDEF: {
		const auto vardef = dynamic_cast<VarDefStmt*>(stmt);
		Value* res = builder.CreateAlloca(vardef->getVarType()->toLLVMType(theContext), nullptr, vardef->getName());
		env->getBindings()[vardef->getName()] = res;
		if (vardef->getInitValue()) {
			builder.CreateStore(exprCodegen(vardef->getInitValue(), env), res);
		}
		return res;
	}
	default:
		std::cerr << "Unexpected statment type???" << std::endl;
		return nullptr;
	}
}

Value* sgs_backend::codegen(AST* ast) {
	switch (ast->astType) {
	case AT_TYPEDEF:
	{
		const auto typeDef = dynamic_cast<TypeDef*>(ast);
		typeReference[typeDef->getName()] = typeDef->getDecType()->toLLVMType(theContext);
		return nullptr;
	}
	case AT_STMT:
		return stmtCodegen(dynamic_cast<Statement*>(ast), nullptr, nullptr, nullptr);
	case AT_FUNC:
	{
		const auto funDef = dynamic_cast<FuncDef*>(ast);
		FunctionType* funType = funDef->getProto()->getLLVMType(theContext);
		Function* fun = Function::Create(funType, GlobalValue::ExternalLinkage, funDef->getProto()->getName(), theModule);
		funcReference[funDef->getProto()->getName()] = fun;
		BasicBlock* funBB = BasicBlock::Create(theContext, "entry", fun);
		builder.SetInsertPoint(funBB);
		auto* env = Environment::derive(nullptr);
		auto iter = fun->args().begin();
		for (const auto& x : funDef->getProto()->getParam()) {
			const auto temp = builder.CreateAlloca(iter->getType(), 0, nullptr, x.second);
			env->getBindings()[x.second] = temp;
			builder.CreateStore(iter, temp);
			iter++;
		}
		return stmtCodegen(funDef->getBody(), env, nullptr, nullptr);
	}
	case AT_PROTO:
	{
		const auto funProto = dynamic_cast<FuncProto*>(ast);
		FunctionType* funTy = funProto->getLLVMType(theContext);
		return Function::Create(funTy, GlobalValue::CommonLinkage, funProto->getName(), theModule);
	}
	default: ;
	}
	return nullptr;
}

void sgs_backend::totalTranslation(const Context& cont) {
	codegenInit();
	builtinFuncInit();
	for (const auto& x : cont) codegen(x);
	std::error_code ec;
	raw_fd_ostream stream("out.ll", ec, sys::fs::OpenFlags());
	stream << builtInFuncs; 
	theModule->print(stream, nullptr);
}
