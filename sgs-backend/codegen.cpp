#include "codegen.h"
#include <iostream>

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
			const auto intlit = dynamic_cast<IntLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext), APInt(32, intlit->getValue()));
		case BasicType::FRACTION:
			const auto ftlit = dynamic_cast<FloatLiteral*>(lit);
			return ConstantFP::get(theContext, APFloat(ftlit->getValue()));
		case BasicType::BOOLEAN:
			const auto boolit = dynamic_cast<BoolLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext), APInt(1, boolit->getValue()));
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
		// case ST_CALL: break;
		// const auto call = dynamic_cast<CallStmt*>(stmt)
	}
	case ST_IF: {
		Function* fun = builder.GetInsertBlock()->getParent();
		const auto ifs = dynamic_cast<IfStmt*>(stmt);
		Value* cond = exprCodegen(ifs->getCond(), env);
		if (!cond) {
			std::cerr << "Translation error : at IfStmt, condition translation is failed" << std::endl;
			return nullptr;
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
		BasicBlock* while_cond = BasicBlock::Create(theContext, "while.cond");
		BasicBlock* while_body = BasicBlock::Create(theContext, "while.body");
		BasicBlock* while_merge = BasicBlock::Create(theContext, "while.merge");
		builder.CreateBr(while_cond);
		fun->getBasicBlockList().push_back(while_cond);
		builder.SetInsertPoint(while_cond);
		Value* cond = exprCodegen(wstmt->getCondition(), env);
		builder.CreateCondBr(cond, while_body, while_merge);
		fun->getBasicBlockList().push_back(while_body);
		builder.SetInsertPoint(while_body);
		stmtCodegen(wstmt->getBody(), env, while_cond, while_merge);
		const auto res = builder.CreateBr(while_cond);
		fun->getBasicBlockList().push_back(while_merge);
		builder.SetInsertPoint(while_merge);
		return res;
	}
	case ST_RETURN: {
		const auto ret = dynamic_cast<ReturnStmt*>(stmt);
		Value* res;
		if (ret->getExp()) {
			res = builder.CreateRet(exprCodegen(ret->getExp(), env));
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
		const auto typeDef = dynamic_cast<TypeDef*>(ast);
		typeReference[typeDef->getName()] = typeDef->getDecType()->toLLVMType(theContext);
		return nullptr;
	case AT_STMT:
		return stmtCodegen(dynamic_cast<Statement*>(ast), nullptr, nullptr, nullptr);
	case AT_FUNC: break; // TODO
	case AT_PROTO: break; // TODO

	default: ;
	}
	return nullptr;
}
