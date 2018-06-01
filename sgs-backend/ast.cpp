#include "ast.h"


sgs_backend::SType* sgs_backend::fromBasicType(BasicType tp, Context& context) {
	switch (tp) {
	case BasicType::INTEGER: return context.getIntType();
	case BasicType::FLOAT: return context.getFloatType();
	case BasicType::BOOLEAN: return context.getBoolType();
	case BasicType::CHAR: return context.getCharType();
	default:
		return nullptr;
	}
}
sgs_backend::SType* sgs_backend::getBinopType(BINOP op, SType* lhs, SType* rhs, Context& context) {
	// return lhs;
	switch (op) {
	case AND:
	case OR:
	case GT:
	case LT:
		return context.getBoolType();
	case ADD:
	case SUB:
	case MUL:
	case DIV: {
		assert(lhs->getLevel() == Types::BASIC_TYPE);
		assert(rhs->getLevel() == Types::BASIC_TYPE);
		const auto ls = dynamic_cast<SBasicType*>(lhs);
		const auto rs = dynamic_cast<SBasicType*>(rhs);
		if (ls->getBasicType() == BasicType::FLOAT) {
			return lhs;
		}
		if (ls->getBasicType() == BasicType::INTEGER || rs->getBasicType() == BasicType::INTEGER) {
			return context.getIntType();
		}
		if (ls->getBasicType() == BasicType::CHAR || rs->getBasicType() == BasicType::CHAR) {
			return context.getCharType();
		}
		return lhs;
	}
	default:
		return nullptr;
	}
}
