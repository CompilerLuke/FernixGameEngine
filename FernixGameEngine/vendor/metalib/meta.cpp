#include <utility>
#include <unordered_map>
#include <sstream>
#include <iomanip>

#include "metalib/meta.h"

Type _floatType(Float);
Type _intType(Int);
Type _StringType(String);
Type _boolType(Bool);

Type* floatType = &_floatType;
Type* intType = &_intType;
Type* stringType = &_StringType;
Type* boolType = &_boolType;

Type _uintType(UInt);
Type* uintType = &_uintType;

StructType* NULLType = NULL;

StructType _TypedType("Typed", sizeof(Typed), std::vector<Member>(), &TypedConstructor);
StructType* TypedType = &_TypedType;

std::unordered_map<std::string, Type*>* StructType::namedTypes = NULL; //doesnt initialize before constructor is called, annoying c++

//std::unordered_map<std::string, Type*> namedTypes;

Type* getNamedType(std::string& name) {
	return (*StructType::namedTypes)[name];
}

Type::Type(TypeEnum type) : type(type) {}

Typed::Typed(Type* type) : type(type) {}

Member::Member(std::string name, size_t offset, Type* type, bool inherited) : name(name), offset(offset), type(type), inherited(inherited) {};

Forward::Forward(std::string name) : name(name), Type(ForwardRef) {};

StructType::StructType(std::string name, size_t sizeofStruct, std::vector<Member> members, void* (*constructor)(void* chunk), StructType* inheritsFrom, bool hideInheritedMembers)
	: name(name), inheritsFrom(inheritsFrom), sizeofStruct(sizeofStruct), members(members), constructor(constructor), hideInheritedMembers(hideInheritedMembers), Type(Struct) {


	if (inheritsFrom) {
		std::cout << inheritsFrom->name << std::endl;
		if (inheritsFrom == TypedType) {
			this->isTyped = true;
		}
		else {
			this->isTyped = inheritsFrom->isTyped;
			std::vector<Member> members = inheritsFrom->members;
			for (unsigned int i = 0; i < members.size(); i++) {
				Member member = members[i];
				this->members.push_back(Member(member.name, member.offset, member.type, true));
			}
			this->offsetFromTyped = inheritsFrom->offsetFromTyped;
		}
		if (this->isTyped && members.size() > 0) {
			this->offsetFromTyped = members[0].offset - offsetof(Typed, type) - sizeof(void*);
			this->members.push_back(Member("ID", offsetof(Typed, ID) + offsetFromTyped, intType, false));
		}
	}

	if (namedTypes == NULL) {
		namedTypes = new std::unordered_map<std::string, Type*>();
	}

	(*namedTypes)[name] = this;
}

PointerType::PointerType(Type* type) : type(type), Type(Pointer) {};
ArrayType::ArrayType(Type* type) : type(type), Type(Array) {};


void* TypedConstructor(void* chunk) {
	return new (chunk) Typed(intType);
}

Type* toRealType(Type* type) {
	switch (type->type) {
	case TypeEnum::ForwardRef:
	{
		Forward * forward = (Forward*)type;
		return (*StructType::namedTypes)[forward->name];
	}
	default:
		return type;
	};
}

Typed* castToTyped(StructType*  structT, void* data) {
	return (Typed*)((char*)data + structT->offsetFromTyped);
}

#include <sstream>

void serializeTyped(Typed* typed, std::string& dirName) {
	return serialize(typed->type, typed, dirName);
}

std::string floatTypeToHex(float f)
{
	static_assert(std::numeric_limits<float>::is_iec559,
		"native float must be an IEEE float");

	union { float fval; std::uint32_t ival; };
	fval = f;

	std::ostringstream stm;
	stm << std::hex << std::uppercase << ival;

	return stm.str() + " /*" + std::to_string(f) + "*/ ";
}

#include <fstream>

void serialize(Type* type, void* data, std::string& dirName) {
	std::unordered_map<long, TypedPointer> pointers;

	std::ofstream entryPointFile;
	entryPointFile.open(dirName + "/entrypoint.sg"); //serialized game state

	serializeWithPointers(entryPointFile, type, data, pointers, "");

	std::ofstream pointerFile;

	std::vector<long> alreadyOutputed;
	std::vector<long> alreadyOutputedWithID;

	unsigned int sizeOfPointers = 0;

	while (sizeOfPointers < pointers.size()) {
		sizeOfPointers = pointers.size();
		
		for (std::pair<long, TypedPointer> pointerPair : pointers)
		{
			if (sizeOfPointers != 0) {
				if (std::find(alreadyOutputed.begin(), alreadyOutputed.end(), pointerPair.first) != alreadyOutputed.end()) { //@cleanup -linear search which could be very slow
					continue;
				}
				if (std::find(alreadyOutputedWithID.begin(), alreadyOutputedWithID.end(), pointerPair.first) != alreadyOutputedWithID.end()) {
					continue;
				}
			}
			TypedPointer pointer = pointerPair.second;
			pointer.type = toRealType(pointer.type);

			StructType* t = (StructType*)pointer.type;
			if (pointer.type->type == Struct && t->isTyped) {
				int ID = castToTyped(t, pointer.pointingTo)->ID;
				alreadyOutputedWithID.push_back(ID);

				pointerFile.open(dirName + "/ID_" + std::to_string(ID));
				pointerFile << ID << " " + std::to_string(pointer.type->type) + " = ";
				serializeWithPointers(pointerFile, pointer.type, pointer.pointingTo, pointers, "", false);
				pointerFile.close();
			}
			else {
				alreadyOutputed.push_back((long)pointer.pointingTo);
				entryPointFile << "\n\n" << std::to_string((long)pointer.pointingTo) << " " << pointer.type->type << " = ";
				serializeWithPointers(entryPointFile, pointer.type, pointer.pointingTo, pointers, "", false);
			}
		}
	}

	entryPointFile.close();
}

size_t sizeOf(Type* type) {
	switch (type->type) {
	case Int:
		return sizeof(int);
	case Float:
		return sizeof(float);
	case UInt:
		return sizeof(unsigned int);
	case Struct: {
		StructType * s = (StructType*)type;
		return ((StructType*)type)->sizeofStruct;
	}
	case Pointer:
		return sizeof(void*);
	case Array:
		return sizeof(std::vector<void*>);
	case String:
		return sizeof(std::string);
	default:
		std::cout << "unimplemented type" << std::endl;
	}
}

#include "Entity.h"

void serializeWithPointers(std::ofstream& file, Type* type, void* data, std::unordered_map<long, TypedPointer>& pointers, std::string indentation, bool isFirst) {
	type = toRealType(type);
	switch (type->type) {
	case TypeEnum::Int:
		file << std::to_string(*(int*)data);
		break;
	case TypeEnum::UInt:
		file << std::to_string(*(unsigned int*)data);
		break;
	case TypeEnum::Float:
		file << floatTypeToHex(*(float*)data);
		break;
	case TypeEnum::String:
		file << "\"" + *(std::string*)data + "\"";
		break;
	case TypeEnum::Pointer: {
		PointerType* pTypeype = (PointerType*)type;
		void* pointingTo = *(void**)data;

		Type* pointingToType = toRealType(pTypeype->type);

		if (pointingToType->type == Struct && ((StructType*)pointingToType)->isTyped) {
			Entity* entT = (Entity*)pointingTo;
			
			int ID = castToTyped((StructType*)pointingToType, pointingTo)->ID;
		
			pointers[ID] = TypedPointer(pointingTo, pointingToType);
			file << "ID_" << ID;
		}
		else {
			pointers[(long)pointingTo] = TypedPointer(pointingTo, pTypeype->type);
			file << std::to_string((long)pointingTo);
		}
		
		break;
	}
	case TypeEnum::Array: {

		ArrayType* aType = (ArrayType*)type;
		file << std::to_string(aType->type->type) << " [\n";
		std::vector<char> _array = *(std::vector<char>*)data;
		char* data = _array.data();
		indentation += "    ";
		size_t sizeOfElement = sizeOf(aType->type);
		unsigned int size = _array.size() / (sizeOfElement / sizeof(char));

		for (unsigned int i = 0; i < size; i++) {
			void* element = data + (i * sizeOfElement);
			file << indentation;
			serializeWithPointers(file, aType->type, element, pointers, indentation, false);
			file << ",\n";
		}
		indentation = indentation.substr(0, indentation.size() - 4);
		file << indentation << "]";
		break;
	}
	case TypeEnum::Struct:
	{
		StructType * structT = (StructType*)type;
		Entity* entityTyp;
		if (structT->isTyped) {	
			entityTyp = (Entity*)data;
			Typed* typedType = (Typed*)data;
			if (!isFirst) {
				typedType = (Typed*)((char*)data + structT->offsetFromTyped);
			}

			structT = (StructType*)toRealType(typedType->type); //for polymorphic objects
			if (isFirst) {
				data = (char*)data - structT->offsetFromTyped; //when casting down to baseclass without virtual functions, compiler will reduce adress
			}

			entityTyp = (Entity*)data;
			
		}
		
		file << structT->name << " {\n";
		indentation += "    ";
		for (unsigned int i = 0; i < structT->members.size(); i++) {
			Member member = structT->members[i];

			file << indentation << member.name + " : ";
			serializeWithPointers(file, member.type, (char*)data + member.offset, pointers, indentation, false);
			file << ",\n";
		}
		indentation = indentation.substr(0, indentation.size() - 4);
		file << indentation << "}";
		break;
	}

	default:
		std::cout << "unknown Type" << std::endl;
	}
}

void pushBackIfNotEmpty(std::vector <std::string>& tokens, std::string* tok, std::vector<long>& lookedUpFiles) {
	if (tok->length() > 0) {
		if (tok->length() > 3 && tok[0] == "I" && tok[1] == "D" && tok[2] == "_") {
			tok->erase(0, 3);

			long id = std::stol(*tok);

			if (std::find(std::begin(lookedUpFiles), std::end(lookedUpFiles), id) != std::end(lookedUpFiles)) {
				lookedUpFiles.push_back(id);
			}
		}

		tokens.push_back(*tok);
		*tok = "";
	}
}

void tokenize(std::vector <std::string>& tokens, std::vector<long> &lookedUpFiles, std::ifstream& file, std::string& dirName, int index) {
	std::string tok = "";
	bool inString = false;
	bool inComment = false;

	std::string stringified;

	while (getline(file, stringified)) {
		for (unsigned int i = 0; i < stringified.size(); i++) {
			char t = stringified[i];
			if (!inString && !inComment) {
				if (t == ' ' || t == ' \t' || t == '\n') {
					pushBackIfNotEmpty(tokens, &tok, lookedUpFiles);
				}
				else if (t == '"') {
					pushBackIfNotEmpty(tokens, &tok, lookedUpFiles);
					inString = true;
				}
				else if (t == '/' && i + 1 < stringified.size() && stringified[i + 1] == '*') {
					pushBackIfNotEmpty(tokens, &tok, lookedUpFiles);
					inComment = true;
					i++;
				}
				else if (t == '{' || t == '}' || t == '[' || t == ']' || t == ',' || t == ':' || t == '=') {
					pushBackIfNotEmpty(tokens, &tok, lookedUpFiles);
					tokens.push_back(std::string(1, t));
				}
				else {
					tok += t;
				}
			}
			else if (inString) {
				if (t == '"') {
					tokens.push_back(tok);
					tok = "";
					inString = false;
				}
				else {
					tok += t;
				}
			}
			else {
				if (t == '*' && i + 1 < stringified.size() && stringified[i + 1] == '/') {
					inComment = false;
					i++;
				}
			}
		}
	}

	pushBackIfNotEmpty(tokens, &tok, lookedUpFiles);

	if (lookedUpFiles.size() > index) {
		std::ifstream file;
		file.open(dirName + "\ID_" + std::to_string(lookedUpFiles[index]));
		tokenize(tokens, lookedUpFiles, file, dirName, index+1);
		file.close();
	}

}

void* parse(StateOfParser& state) {
	//std::cout << state.tokenized[state.iter + 2] << std::endl;
	if (state.iter + 1 < state.tokenized.size() && state.tokenized[state.iter + 1] == "{") {
		void* ref = parseStruct(state, NULL);

		while (state.iter + 1 != state.tokenized.size() && state.iter < state.tokenized.size()) {
			state.iter++;
			parse(state);

		};
		return ref;
	}
	else if (state.iter + 2 < state.tokenized.size() && state.tokenized[state.iter + 2] == "=") {
		//std::cout << "about to parse pointer" << std::endl;
		void* ref = parsePointer(state);

		while (state.iter + 1 != state.tokenized.size() && state.iter < state.tokenized.size()) {
			state.iter++;
			parse(state);

		};
		//	if (state.iter + 1 != state.tokenized.size()) {
		//		parse(state);
		//		throw "didnt finish reading all input";
		//	}
		//}
		return ref;
	}
	std::cout << "was expecting struct" << std::endl;
	//throw "was expecting struct"; //didnt find parser
}

union ulf
{
	unsigned long ul;
	float f;
};

float hexTypeo_float(std::string& str) {
	ulf u;
	std::stringstream ss(str);
	ss >> std::hex >> u.ul;
	float f = u.f;
	//std::cout << "converting " << str << " to " << f << std::endl;

	return f;
}



void* parseArray(StateOfParser&, void*);

void setFieldTo(Type* type, StateOfParser& state, void* pointerToField) {
	type = toRealType(type);
	switch (type->type) {
	case Struct: {
		parseStruct(state, pointerToField);
		break;
	}
	case Array: {
		parseArray(state, pointerToField);
		break;
	}
	case Int:
		*((int*)pointerToField) = std::stoi(state.tokenized[state.iter]);
		break;
	case UInt:
		*((unsigned int*)pointerToField) = std::stoul(state.tokenized[state.iter]);
	case Float:
		*((float*)pointerToField) = hexTypeo_float(state.tokenized[state.iter]);
		break;
	case String:
		*((std::string*)pointerToField) = state.tokenized[state.iter];
		break;
	case Pointer: {
		//std::cout << "setting field, which is of type pointer" << std::endl;
		size_t id = std::stol(state.tokenized[state.iter]);
		PointerType* pointerType = (PointerType*)type;
		size_t sizeofNeededMemory = sizeOf(pointerType->type);

		void* allocatedMemory;
		if (state.allocatedPointers.find(id) == state.allocatedPointers.end()) {
			allocatedMemory = malloc(sizeofNeededMemory);
			state.allocatedPointers[id] = allocatedMemory;
		}
		else {
			allocatedMemory = state.allocatedPointers[id];
		}
		*((void**)pointerToField) = allocatedMemory;
		break;
	}
	default:
		std::cout << "Havent handled type " << type->type << " yet" << std::endl;
	}
}

PointerType tmp(intType);

struct SizeAndType {
	Type* type;
	size_t size;
};

SizeAndType getTypeFromEnum(size_t _type, StateOfParser& state) {
	if (state.iter >= state.tokenized.size()) {
		std::cout << "Expecting another token" << std::endl;
	}
	std::string nameType = state.tokenized[state.iter];
	StructType* structType = NULL;

	if (_type == Struct) {
		structType = (StructType*)(*StructType::namedTypes)[nameType]; //polymorphism
	}

	size_t sizeofNeededMemory = 0;

	Type* type = NULL;
	switch (_type) {
	case Int:
		sizeofNeededMemory = sizeof(int);
		type = intType;
		break;
	case Float:
		sizeofNeededMemory = sizeof(float);
		type = floatType;
		break;
	case UInt:
		sizeofNeededMemory = sizeof(unsigned int);
		type = uintType;
		break;
	case Struct:
		sizeofNeededMemory = structType->sizeofStruct;
		type = structType;
		break;
	case String:
		sizeofNeededMemory = sizeof(std::string);
		type = stringType;
	case Array: {
		state.iter += 2;
		SizeAndType sizeAndT = getTypeFromEnum(std::stoi(nameType), state);
		sizeofNeededMemory = sizeAndT.size;
		type = new ArrayType(sizeAndT.type);
	}
	case Pointer:
		sizeofNeededMemory = sizeof(void*);
		type = &tmp;
		break;
	default:
		std::cout << "Unimplement type " << _type;
	}

	return { type, sizeofNeededMemory };
}

void* parsePointer(StateOfParser& state) {
	//std::cout << "parsing pointer" << std::endl;
	size_t id = stol(state.tokenized[state.iter]);
	std::string nameTypeype = state.nameOfTypedPointers[id];

	state.iter += 3;

	SizeAndType sizeAndType = getTypeFromEnum(std::stoi(state.tokenized[state.iter - 2]), state);
	Type* type = sizeAndType.type;
	size_t sizeofNeededMemory = sizeAndType.size;

	void* pointingTo;

	if (state.allocatedPointers.find(id) != state.allocatedPointers.end()) {
		pointingTo = state.allocatedPointers[id];
		setFieldTo(type, state, pointingTo);
	}
	else {
		void* allocatedMemory = malloc(sizeofNeededMemory);
		setFieldTo(type, state, allocatedMemory);
		pointingTo = allocatedMemory;
	}

	if (type->type == Struct) {
		StructType* sType = (StructType*)type;
		if (sType->isTyped) {
			StructType** pointerToTypeField = (StructType**)((char*)pointingTo + offsetof(Typed, type) + sType->offsetFromTyped);
			*pointerToTypeField = sType;
		}
	}

	return pointingTo;
}

void* parseArray(StateOfParser& state, void* initInField) {
	if (state.tokenized[state.iter + 1] != "[") {
		std::cout << "expecting [" << std::endl;
	}
	state.iter += 2;
	std::string it = state.tokenized[state.iter];
	SizeAndType sizeAndType = getTypeFromEnum(std::stoi(state.tokenized[state.iter - 2]), state);
	Type* type = sizeAndType.type;
	size_t sizeofNeededMemory = sizeAndType.size;

	std::vector<char>* array = new (initInField) std::vector<char>();

	unsigned int i = 0;
	while (it != "]" || state.iter == state.tokenized.size()) {
		it = state.tokenized[state.iter];
		if (it == ",") { state.iter++; it = state.tokenized[state.iter]; continue; }

		i++;
		array->reserve(i * sizeofNeededMemory);
		for (unsigned int i = 0; i < sizeofNeededMemory; i++) {
			array->push_back(' '); //increment the size of the array
		}

		void* pointerToElement = array->data() + (sizeofNeededMemory * (i - 1));

		setFieldTo(type, state, pointerToElement);
		state.iter++;
	}
	if (it != "]") {
		std::cout << "missing closing bracket" << std::endl;
	}
	return array;
}

void* parseStruct(StateOfParser& state, void* initInField) {

	std::string name = state.tokenized[state.iter];

	StructType* type = (StructType*)getNamedType(name);
	if (!type) { std::cout << "Unknown type " + name << std::endl; }
	state.iter += 2; //past opening bracket
	std::string it = state.tokenized[state.iter];
	void* obj = NULL;
	if (initInField) {
		obj = initInField;
	}
	else {
		obj = malloc(type->sizeofStruct);
	}
	type->constructor(obj);
	while (it != "}" || state.iter == state.tokenized.size()) {
		it = state.tokenized[state.iter];
		if (it == ",") { state.iter++; it = state.tokenized[state.iter]; continue; }

		std::string field = it;
		state.iter++;
		if (state.tokenized[state.iter] != ":") {
			std::cout << "expecting :" << std::endl;
		}
		state.iter++;

		Member member("", 0, NULL);
		for (unsigned int i = 0; i < type->members.size(); i++) {
			if (type->members[i].name == field) {
				member = type->members[i];
				break;
			}
		}

		if (member.name == "") { std::cout << "no field called " + field + " in struct " + name << std::endl; }

		setFieldTo(member.type, state, (void*)((char*)obj + member.offset));
		state.iter++;
		it = state.tokenized[state.iter];
	}
	if (it != "}") {
		std::cout << "missing closing bracket" << std::endl;
	}

	if (type->isTyped) {
		StructType** pointerToTypeField = (StructType**)((char*)obj + offsetof(Typed, type) + type->offsetFromTyped);
		*pointerToTypeField = type;
	}

	return obj;
}

void getTypeOfPointers(StateOfParser& state) {
	for (int i = 0; i < state.tokenized.size(); i++) {
		if (state.tokenized[i] == "=") {
			if (i >= 2 && i + 1 < state.tokenized.size()) {
				state.nameOfTypedPointers[std::stol(state.tokenized[i - 2])] = state.tokenized[i + 1];
				//std::cout << "pointer " << state.tokenized[i - 2] << " = " << state.tokenized[i + 1] << std::endl;
			}
		}
	}
}


void* deserialize(std::string& dirName) {
	std::ifstream entryFile;
	entryFile.open(dirName + "/entrypoint.gs", std::ios::in);
	
	std::vector<std::string> tokenized;
	std::vector<long> lookedUpFiles;

	tokenize(tokenized, lookedUpFiles, entryFile, dirName);

	entryFile.close();

	std::unordered_map<long, std::string> typeOfPointers;
	std::unordered_map<long, void*> allocatedPointers;

	StateOfParser state{ 0, tokenized, typeOfPointers, allocatedPointers };

	unsigned int iter = 0;

	getTypeOfPointers(state);

	return parse(state);
}