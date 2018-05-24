#include "ast.h"

sgs_backend::SType* sgs_backend::fromBasicType(BasicType tp) {
	switch (tp) {
	case BasicType::INTEGER: return createIntType();
	case BasicType::FRACTION: return createFloatType();
	case BasicType::BOOLEAN: return createBoolType();
	default:
		return nullptr;
	}
}
