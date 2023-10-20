#ifndef PARSING_H
#define PARSING_H

#include "error.hpp"

void CheckEvenParentheses(const string& expression);

void TrimExcessParentheses(string& s);

bool IsTrueValue(string& s);

bool IsStateValue(const string& s);

string FormatString(const char* format ...);

#endif // !Parsing
