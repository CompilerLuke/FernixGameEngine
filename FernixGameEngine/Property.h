#pragma once

#include <string>
#include <map>

class Type {
	virtual std::string toString() { return ""; }
	virtual bool isPrimitive() { return true;  }
	virtual std::string serialize() { return ""; }
	virtual void* deserialize(std::string) { return NULL; }
};

class Int : public Type {
	virtual std::string toString() {
		return "int";
	}
};

class Long : public Type {
	virtual std::string toString() {
		return "long";
	}
};

class Float : public Type {
	virtual std::string toString() {
		return "float";
	}
};

class Double : public Type {
	virtual std::string toString() {
		return "double";
	}
};

class Class : public Type {
	std::string name;
	std::map<std::string, Type> members;

	virtual std::string toString() {

	}
	virtual bool isPrimitve() { return false; }
};

