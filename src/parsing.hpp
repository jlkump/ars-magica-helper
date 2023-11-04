#ifndef PARSING_H
#define PARSING_H

#include "error.hpp"
class CharacterState;

void CheckEvenParentheses(const string& expression);

void TrimExcessParentheses(string& s);

inline void RemoveWhitespace(string& s);

bool IsTrueValueFormat(string& s);

bool IsStateValueFormat(const string& s);

string FormatString(const char* format ...);

bool ContainsEqual(const string& s);

void ParseExpression(const string& s, string& lhs, string& rhs);

// TODO: Define Ars File Reader/Writer ? 
bool CreateCharacterFromPlainTextFile(const char* file_path, CharacterState& character);
/*

namespace ArsFileReader {
	void ReadCharacterFromPlainTextFile(const char* file_path, CharacterState& character);
	void ReadCharacterFromCsvText(const char* file_path, CharacterState& character);
};

*/

#endif // !Parsing
