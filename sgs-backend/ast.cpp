#include "ast.h"

sgs_backend::SType* sgs_backend::getBinopType(BINOP op, SType* lhs, SType* rhs) {
	// return lhs;
	switch (op) {
	case AND:
	case OR:
	case GT:
	case LT:
		return createBoolType();
	case ADD:
	case SUB:
	case MUL:
	case DIV:
		return lhs;
	default:
		return nullptr;
	}
}

sgs_backend::SType* sgs_backend::fromBasicType(BasicType tp) {
	switch (tp) {
	case BasicType::INTEGER: return createIntType();
	case BasicType::FRACTION: return createFloatType();
	case BasicType::BOOLEAN: return createBoolType();
	default:
		return nullptr;
	}
}
