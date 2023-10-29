#ifndef ERROR_H
#define ERROR_H

#include <cstdarg>
#include <stdexcept>
#include <string>

using namespace std;

class EvaluationError : public exception {
public:
	enum Type {
		NONEXISTANT_STATE_VALUE,
		INVALID_NODE,
		DIVIDE_BY_ZERO,
		INVERTED_DEPENDENCY,
	};
private:
	string error_;
	Type type_;
public:
	EvaluationError(const string& error, Type t) : error_(error), type_(t) {}
	virtual const char* what() const noexcept { return error_.c_str(); }
	EvaluationError::Type GetType() { return type_; }
};

class SyntaxError : public exception {
public:
	enum Type {
		UNBALANCED_PARENTHESES,		 // Ex:    8 * (1 + 2))
		INVALID_ORDERED_PARENTHESES, // Ex:    8 * (1 + 2]
		EMPTY_STATE_VALUE_NAME,      // Ex:    8 + []
		ILLFORMED_EXPRESSION,        // Ex:    sqrt 10
		INVALID_OPERATION,           // Ex:    nonexistent(2004)
		INVALID_VARIABLE_NAME,       // Ex:    8 + [(*asjlkaj)]
		EMPTY_OPERATION,		     // Ex:	   sqrt()
		INVALID_NUMBER_FORMAT,       // Ex:    8.0.0 or 8e324a33 or 0x10AD3
		FORMAT_STRING_ERROR,         // Ex:    passing in "%" instaed of "%%"
		CYCLIC_DEPENDENCY,
	};
private:
	string error_;
	Type type_;
public:
	SyntaxError(const string& error, Type t) : error_(error), type_(t) {}
	virtual const char* what() const noexcept { return error_.c_str(); }
	SyntaxError::Type GetType() { return type_; }
};

#endif