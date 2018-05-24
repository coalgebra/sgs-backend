#pragma once

#pragma once

#include <utility>
#include <vector>
#include <string>
#include "TypeSys.h"
#include <map>

namespace sgs_backend {

	using std::vector;
	using std::string;
	using std::map;

	enum AST_TYPE {
		AT_TYPEDEF,
		// AT_EXP,
		AT_STMT,
		AT_FUNC,
		AT_PROTO
	};

	class AST {
	public:
		AST_TYPE astType;
		AST(AST_TYPE t) :astType(t) {}
		virtual ~AST() = default;
	};

	class TypeDef : public AST {
	private:
		SType * decType;
		string name;
	public:
		TypeDef(SType* t, string n) : AST(AT_TYPEDEF), decType(t), name(std::move(n)) {}
		SType *getDecType() const { return decType; }
		string getName() const { return name; }
	};

	enum EXP_TYPE {
		ET_BINOP,
		ET_LITERAL,
		ET_IDENT,
		ET_VISIT,
		ET_CALL,
		ET_ACCESS
	};

	class Expression {
	private:
		EXP_TYPE expType;
		SType * resType;
	public:
		virtual ~Expression() = default;
		Expression(EXP_TYPE t, SType* v) : expType(t), resType(v) {}
		EXP_TYPE getExpType() const { return expType; }
		SType *getResType() const { return resType; }
		void setResType(SType *t) { resType = t; }
	};

	enum BINOP {
		AND,
		OR,
		ADD,
		SUB,
		MUL,
		DIV,
		GT,
		LT,
	};

	class BinopExp : public Expression {
	private:
		BINOP op;
		Expression *left, *right;
	public:
		BinopExp(BINOP op, SType* tp) : Expression(ET_BINOP, tp), op(op), left(nullptr), right(nullptr) {}
		void setLeft(Expression *l) { left = l; }
		void setRigth(Expression *r) { right = r; }
		Expression *getLeft() const { return left; }
		Expression *getRigth() const { return right; }
		BINOP getOp() const { return op; }

	};

	SType* fromBasicType(BasicType tp);

	class LiteralExp : public Expression {
		BasicType btype;
	public:
		explicit LiteralExp(BasicType type) : Expression(ET_LITERAL, fromBasicType(type)), btype(type) {}
		BasicType getBType() const {
			return btype;
		}
	};

	class IntLiteral : public LiteralExp {
		int value;
	public:
		explicit IntLiteral(int value = 0) : LiteralExp(BasicType::INTEGER), value(value) {}
		int getValue() const { return value; }
	};

	class BoolLiteral : public LiteralExp {
		bool value;
	public:
		explicit BoolLiteral(bool value = false) : LiteralExp(BasicType::BOOLEAN), value(value) {}
		bool getValue() const { return value; }
	};

	class FloatLiteral : public LiteralExp {
		double value;
	public:
		explicit FloatLiteral(double value = 0) : LiteralExp(BasicType::FRACTION), value(value) {}
		double getValue() const { return value; }
	};

	// class StrLiteral : public LiteralExp {
	// 	string value;
	// public:
	// 	explicit StrLiteral(string value = "") : LiteralExp(BT_STRING), value(std::move(value)) {}
	// 	string getValue() const { return value; }
	// };

	inline LiteralExp* get_literal(int value = 0) {
		return new IntLiteral(value);
	}

	inline LiteralExp* get_literal(bool value = false) {
		return new BoolLiteral(value);
	}

	inline LiteralExp* get_literal(double value = 0.0) {
		return new FloatLiteral(value);
	}

	class IdExp : public Expression {
		string name;
	public:
		explicit IdExp(string n, SType* type) : Expression(ET_IDENT, type), name(std::move(n)) {}
		string getName() const {
			return name;
		}
	};

	class AccessExp : public Expression {
		Expression* object;
		string member;
	public:
		AccessExp(Expression* obj, string member, SType* resType) : Expression(ET_ACCESS, resType), object(obj), member(std::move(member)) {}
		Expression* getObject() const { return object; }
		string getMember() const { return member; }
	};

	class VisitExp : public Expression {
	private:
		Expression * array;
		Expression * index;
	public:

		VisitExp(Expression* array, Expression* index) : Expression(ET_VISIT, (dynamic_cast<SArrayType*>(array->getResType())->getElementType())), array(array), index(index) {}
		Expression *getArray() const { return array; }
		Expression *getIndex() const { return index; }
	};


	enum STMT_TYPE {
		ST_ASSIGN,
		// ST_CALL,
		ST_IF,
		ST_WHILE,
		ST_RETURN,
		ST_BREAK,
		ST_CONTINUE,
		ST_BLOCK,
		ST_EXP,
		ST_VARDEF
	};

	class Statement : public AST {
	private:
		STMT_TYPE stmtType;
	public:
		explicit Statement(STMT_TYPE t) : AST(AT_STMT), stmtType(t) {}
		STMT_TYPE getStmtType() const { return stmtType; }
	};

	class ExpStmt : public Statement {
		Expression* exp;
	public:
		explicit ExpStmt(Expression* exp) : Statement(ST_EXP), exp(exp) {}
		Expression* getExp() const { return exp; }
	};

	class AssignStmt : public Statement {
	private:
		Expression * left, *right;
	public:
		AssignStmt(Expression* left, Expression* right) : Statement(ST_ASSIGN), left(left), right(right) {}
		Expression *getLeft() const { return left; }
		Expression *getRigth() const { return right; }
	};

	class BlockStmt : public Statement {
	private:
		vector<Statement *> content;
		Environment* env;
	public:
		BlockStmt(Environment* env) : Statement(ST_BLOCK), env(env) {}
		const vector<Statement *>& getContent() const { return content; }
		Environment* getEnv() const { return env; }
	};

	class FuncProto : public AST {
	private:
		SType * returnType;
		string name;
		vector <std::pair<SType *, string>> paramList;
	public:
		FuncProto(SType *ret, string n) : AST(AT_PROTO), returnType(ret), name(std::move(n)) {}
		void pushParam(SType *t, string n) {
			paramList.emplace_back(t, n);
		}
		SType *getReturnType() const { return returnType; };
		string getName() const { return name; }
		const vector <std::pair<SType *, string>>& getParam() const { return paramList; };
	};

	class FuncDef : public AST {
	private:
		FuncProto * proto;
		BlockStmt* body;
	public:
		FuncDef(FuncProto* p) : AST(AT_FUNC), proto(p), body(nullptr) {}
		FuncProto *getProto() const { return proto; }
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
	};

	// class CallStmt : public Statement {
	// private:
	// 	FuncDef * function;
	// 	vector <Expression *> paramList;
	// public:
	// 	explicit CallStmt(FuncDef *f, vector<Expression*> paramList) :
	// 		Statement(ST_CALL), function(f), paramList(std::move(paramList)) {
	// 	}
	// 	void pushParam(Expression *e) { paramList.push_back(e); }
	// 	FuncDef *getFunction() const { return function; };
	// 	const vector <Expression *>& getParam() const { return paramList; };
	// };

	class CallExp : public Expression {
	private:
		string caller;
		vector <Expression *> paramList;
	public:
		CallExp(string f, vector<Expression*> paramList, SType* type) :
			Expression(ET_CALL, type), caller(std::move(f)), paramList(std::move(paramList)) {
		}
		void pushParam(Expression *e) { paramList.push_back(e); }
		string getFunction() const { return caller; };
		const vector <Expression *>& getParam() const { return paramList; };
	};

	class IfStmt : public Statement {
		Expression * condition;
		Statement *taken, *untaken;
	public:
		IfStmt(Expression *b, Statement* taken, Statement* untaken) :
			Statement(ST_IF), condition(b), taken(taken), untaken(untaken) {
		}
		void setTaken(Statement *t) { taken = t; }
		void setUntaken(Statement *u) { untaken = u; }
		Statement *getTaken() const { return taken; }
		Statement *getUntaken() const { return untaken; }
		Expression* getCond() const { return condition; }
	};

	class WhileStmt : public Statement {
	private:
		Expression * condition;
		BlockStmt * body;
	public:
		WhileStmt(Expression *c, BlockStmt* body) : Statement(ST_WHILE), condition(c), body(body) {}
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
		Expression* getCondition() const { return condition; }
	};

	class VarDefStmt : public Statement {
		Expression* initValue;
		SType* varType;
		string name;
	public:

		VarDefStmt(STMT_TYPE t, SType* varType, Expression* initValue, string name)
			: Statement(t),
			  initValue(initValue),
			  varType(varType),
			  name(std::move(name)) {}

		Expression* getInitValue() const { return initValue; }
		SType* getVarType() const { return varType; }
		string getName() const { return name; }
	};

	class ReturnStmt : public Statement {
		Expression* exp;
	public:
		explicit ReturnStmt(Expression* exp) : Statement(ST_RETURN), exp(exp) {}
		Expression* getExp() const { return exp; }
	};

	class BreakStmt : public Statement {
	public:
		BreakStmt() : Statement(ST_BREAK) {}
	};

	class ContinueStmt : public Statement {
	public:
		ContinueStmt() : Statement(ST_CONTINUE) {}
	};

	class Context {
		// TODO
	};
}
