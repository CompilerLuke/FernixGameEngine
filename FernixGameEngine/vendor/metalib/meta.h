#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstddef>
#include <unordered_map>

#include <fstream>

enum TypeEnum {
	Struct,
	ForwardRef,
	Class,
	Int,
	Long,
	UInt,
	ULong,
	Float,
	Double,
	Bool,
	String,
	Pointer,
	Array,
};

struct Type {
	TypeEnum type;
	Type(TypeEnum type);
};

struct PointerType : Type {
	Type* type;

	PointerType(Type* type);
};

struct ArrayType : Type {
	Type* type;
	
	ArrayType(Type* type);
};

struct Member {
	std::string name;
	Type* type;
	size_t offset;
	bool inherited;

	Member(std::string name, size_t offset, Type* type, bool inherited = false);
};

struct Forward : Type {
	std::string name;
	Forward(std::string name);
};

struct Typed {
	int ID;
	Type* type;
	Typed(Type* type);
};

struct StructType : Type {
	static std::unordered_map<std::string, Type*>* namedTypes;
	
	std::string name;
	std::vector<Member> members;
	bool hideInheritedMembers;
	size_t sizeofStruct;
	bool isTyped = false;
	size_t offsetFromTyped = 0;
	StructType* inheritsFrom;

	void* (*constructor)(void* chunk);

	StructType(std::string name, size_t sizeofStruct, std::vector<Member> members, void* (*constructor)(void* chunk), StructType* inheritsFrom = NULL, bool hideInheritedMembers = true);
};

extern Type* floatType;
extern Type* intType;
extern Type* stringType;
extern StructType* NULLType;
extern Type* uintType;

extern void* TypedConstructor(void* chunk);
extern StructType* TypedType;

Type* toRealType(Type* type);
Type* getNamedType(std::string& name);

struct TypedPointer : Typed {
	void* pointingTo;

	TypedPointer() : pointingTo(NULL), Typed(intType) {};
	TypedPointer(void* pointingTo, Type* type) : pointingTo(pointingTo), Typed(type) {};
};

struct StateOfParser {
	unsigned int iter;
	std::vector<std::string>& tokenized;
	std::unordered_map<long, std::string>& nameOfTypedPointers;
	std::unordered_map<long, void*>& allocatedPointers;
	std::string dirName;
};

void serializeTyped(Typed* typed, std::string& dir);

void serializeWithPointers(std::ofstream& file, Type* type, void* data, std::unordered_map<long, TypedPointer>& pointers, std::string indentation = "", bool isFirst = true);

void serialize(Type* type, void* data, std::string& dir);

void tokenize(std::vector <std::string>& tokens, std::vector<long> &lookedUpFiles, std::ifstream& file, std::string& dirName, int index = 0);

void* parsePointer(StateOfParser& state);
void* parseStruct(StateOfParser& state, void* initInField = NULL);

void* parse(StateOfParser& state);

void setFieldTo(Type* type, StateOfParser& state, void* pointerToField);

void* parseStruct(StateOfParser& state, void* initInField);
void* deserialize(std::string& stringified);

size_t sizeOf(Type* type);

#define DEFTYPE_NAMESPACE(_namespace, name, inherits, ...) void* Constructor_##_namespace##_##name(void* chunk) { return new (chunk) _namespace::name(); }; StructType _##_namespace##_##name##Type(#_namespace "_" #name, sizeof(_namespace::name), std::vector<Member> { __VA_ARGS__ }, &Constructor##_##_namespace##_##name, inherits##Type); StructType* _namespace##_##name##Type = &_##_namespace##_##name##Type;
#define DEFTYPE(name, inherits, ...) void* Constructor##name(void* chunk) { return new (chunk) name(); }; StructType _##name##Type(#name, sizeof(name), std::vector<Member> { __VA_ARGS__ }, &Constructor##name, inherits##Type); StructType* name##Type = &_##name##Type;
#define MEMBER(StructName, field, type) Member(#field, offsetof(StructName, field), type##Type) 
#define MEMBER_NT(StructName, field, type) Member(#field, offsetof(StructName, field), type) 
#define POINTER(type) new PointerType(type##Type)
#define ARRAY(type) new ArrayType(type##Type)

#define POINTER_NT(type) new PointerType(type)
#define ARRAY_NT(type) new ArrayType(type)


#define FORWARD(type) new Forward(#type)