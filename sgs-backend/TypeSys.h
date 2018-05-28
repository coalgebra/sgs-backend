#pragma once
#include <vector>
#include <string>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <cstddef>
#include <map>


using namespace llvm;

namespace sgs_backend {

	using std::vector;
	using std::string;
	using std::pair;
	using llvm::Type;
	using std::map;


	enum class Types {
		BASIC_TYPE,
		ARRAY_TYPE,
		TUPLE_TYPE
	};

	class SType {
		Types level;
	public:
		virtual ~SType() = default;
		explicit SType(Types level) : level(level) {}
		Types getLevel() const { return level; }
		virtual Type* toLLVMType(LLVMContext& context) const {
			return nullptr;
		}
	};

	enum class BasicType {
		INTEGER, // int
		FRACTION, // double
		BOOLEAN, // bool
		CHAR, // char
	};

	class SBasicType : public SType {
		BasicType type;
	public:
		explicit SBasicType(BasicType type) : SType(Types::BASIC_TYPE), type(type) {}
		BasicType getBasicType() const { return type; }
		Type* toLLVMType(LLVMContext& context) const override {
			if (type == BasicType::INTEGER) {
				return Type::getInt32Ty(context);
			} else if (type == BasicType::FRACTION) {
				return Type::getFloatTy(context);
			} else if (type == BasicType::CHAR){
				return Type::getInt8Ty(context);
			} else if (type == BasicType::BOOLEAN) { 
				return Type::getInt1Ty(context);
			}
			return nullptr;
		}
	};

	inline SBasicType* createIntType() {
		return new SBasicType(BasicType::INTEGER);
	}

	inline SBasicType* createFloatType() {
		return new SBasicType(BasicType::FRACTION);
	}

	inline SBasicType* createBoolType() {
		return new SBasicType(BasicType::BOOLEAN);
	}

	inline SBasicType* createCharType() {
		return new SBasicType(BasicType::CHAR);
	}

	class SArrayType : public SType {
		SType* type;
		size_t count;
	public:
		SArrayType(SType* type, size_t count) : SType(Types::ARRAY_TYPE), type(type), count(count) {}
		size_t getCount() const {
			return count;
		}
		SType* getElementType() const { return type; }
		Type* toLLVMType(LLVMContext& context) const override {
			return ArrayType::get(type->toLLVMType(context), count);
		}
	};

	class STupleType : public SType {
		std::vector<std::pair<std::string, SType*>> types;
		string name;
	public:
		STupleType(vector<pair<string, SType*>> types, string name) : SType(Types::TUPLE_TYPE), types(std::move(types)), name(std::move(name)) {}
		auto& getTypes() const {
			return types;
		}
		Type* toLLVMType(LLVMContext& context) const override {
			vector<Type*> res;
			for (auto && type : types) {
				res.push_back(type.second->toLLVMType(context));
			}
			return StructType::create(context, res, name);
		}
	};

	class Environment {
		Environment* parent;
		map<string, Value*> bindings;
	public:
		explicit Environment(Environment* parent = nullptr) : parent(parent) {}
		Value* operator [] (const string& str) {
			auto temp = this;
			while (temp) {
				if (temp->bindings.find(str) != temp->bindings.end()) {
					return temp->bindings[str];
				}
				temp = temp->parent;
			}
			return nullptr;
		}
		Value* find(const string& str) {
			return operator[](str);
		}
		map<string, Value*>& getBindings() {
			return bindings;
		}
		static Environment* derive(Environment* env) {
			return new Environment(env);
		}
	};

	// typedef SType* TypeObj;

	bool sameType(SType* t1, SType* t2);

}