#include "TypeSys.h"

namespace sgs_backend {
	bool sameType(SType* t1, SType* t2) {
		if (t1->getLevel() != t2->getLevel()) return false;
		switch (t1->getLevel()) {
		case Types::BASIC_TYPE:
		{
			const auto bt = dynamic_cast<SBasicType*>(t1);
			const auto bt2 = dynamic_cast<SBasicType*>(t2);
			return bt->getBasicType() == bt2->getBasicType();
		}
		case Types::ARRAY_TYPE:
		{
			const auto ap = dynamic_cast<SArrayType*>(t1);
			const auto ap2 = dynamic_cast<SArrayType*>(t2);
			return ap->getCount() == ap2->getCount() && sameType(ap->getElementType(), ap2->getElementType());
		}
		case Types::TUPLE_TYPE:
		{
			const auto tp = dynamic_cast<STupleType*>(t1);
			const auto tp2 = dynamic_cast<STupleType*>(t2);
			if (tp->getTypes().size() != tp2->getTypes().size()) return false;
			for (size_t i = 0; i < tp->getTypes().size(); i++) {
				if (!sameType(tp->getTypes()[i].second, tp2->getTypes()[i].second) && tp->getTypes()[i].first == tp2->getTypes()[i].first) {
					return false;
				}
			}
			return true;
		}
		default:
			return false;
		}
	}
}