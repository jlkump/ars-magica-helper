#include "parsing.hpp"

#include "state_trackers.hpp"

#include <stack>
#include <string>
#include <fstream>

/*
* This function will check that the parentheses of a given string are balanced. If they are not,
* then a SyntaxError of appropriate type will be thrown.
*/
void CheckEvenParentheses(const string& expression) {
	stack<char> s;
	for (char cur : expression) {
		// Ignore chars that are not parentheses
		if (cur != '[' && cur != ']' && cur != '(' && cur != ')' && cur != '{' && cur != '}') {
			continue;
		}

		// Put open brackets on the stack
		if (cur == '[' || cur == '{' || cur == '(') {
			s.push(cur);
		}
		else {
			if (s.empty()) {
				// We encountered a closed bracket with no corresponding open bracket.
				throw SyntaxError(FormatString("Unbalanced parentheses in given expression: \"%s\".\n", expression.c_str()),
					SyntaxError::Type::UNBALANCED_PARENTHESES);
			}
			char open_brace = s.top();
			if ((open_brace == '[' && cur != ']') || (open_brace == '{' && cur != '}') || (open_brace == '(' && cur != ')')) {
				// We encountered the wrong pairing of brackets.
				throw SyntaxError(
					FormatString("Parentheses are not ordered correctly in expression: "
						"\"%s\"\nExpected a closing brace to \'%c\' but found \'%c\' instead.\n", expression.c_str(),
						open_brace, cur),
					SyntaxError::Type::INVALID_ORDERED_PARENTHESES);
			}
			// If we get here, everything is OK and we can take the opening bracket off the stack.
			s.pop();
		}
	}
	if (!s.empty()) {
		// If we get here, we still have opening brackets that had no closing bracket pair
		throw SyntaxError(
			FormatString("Unbalanced parentheses in given expression: \"%s\"\n", expression.c_str()),
			SyntaxError::Type::UNBALANCED_PARENTHESES);
	}
	// All good, simply exit control flow
}

void TrimExcessParentheses(string& s) {
	auto iter = s.begin();
	auto end = s.end();
	while (iter < end && *iter == '(' && *(end - 1) == ')') {
		iter++;
		end--;
	}
	s = string(iter, end);
}

bool IsTrueValue(string& s) {
	int dot_count = 0;
	for (const char c : s) {
		if (c == '.') {
			dot_count++;
		}
		else if (!(c >= '0' && c <= '9')) {
			return false;
		}
	}
	return dot_count < 2;
}

bool IsStateValue(const string& s) {
	if (s.begin() == s.end() || *s.begin() != '[' || *(s.end() - 1) != ']') {
		return false;
	}

	int brace_count = 0;
	for (const char c : s) {
		if (c == '[') brace_count++;
	}
	return brace_count == 1;
}

string FormatString(const char* format ...)
{
	va_list args;
	va_start(args, format);

	string result;
	auto iter = format;
	while (*iter != '\0') {
		if (*iter == '%') {
			// Check next value
			if (*(iter + 1) == '\0') {
				throw SyntaxError("Format string escape is missing specifier: %?. Available options are %f, %d, %c, %s, and %%.\n", SyntaxError::FORMAT_STRING_ERROR);
			}
			char next = *(iter + 1);
			string s;
			switch (next) {
			case 'f':
				s = to_string(va_arg(args, double));
				break;
			case 'c':
				result.push_back(va_arg(args, char));
				break;
			case 'd':
				s = to_string(va_arg(args, int));
				break;
			case 's':
				s = va_arg(args, char*);
				break;
			case '%':
				result.push_back('%');
				break;
			default:
				throw SyntaxError("Format string escape specifier is not supported. Supported operations are: %f, %d, %c, %s, and %%.\n", SyntaxError::FORMAT_STRING_ERROR);
			}
			auto s_it = s.begin();
			while (s_it != s.end()) {
				result.push_back(*s_it);
				s_it++;
			}
			iter++;
		}
		else {
			result.push_back(*iter);
		}
		iter++;
	}

	return result;
}

bool ContainsEqual(const string& s) {
	for (const char c : s) {
		if (c == '=') {
			return true;
		}
	}
	return false;
}

void ParseExpression(const string& s, string& name, string& expression) {
	auto i = s.begin();
	while (i != s.end() && *i != '=') {
		i++;
	}
	name = string(s.begin(), i);
	expression = string(i + 1, s.end());
}


bool CreateCharacterFromPlainTextFile(const char* file_path, CharacterState& character) {
	if (file_path == nullptr) {
		return false;
	}

	ifstream input_file(file_path);
	if (!input_file.is_open()) {
		return false;
	}
	string line;
	while (getline(input_file, line)) {
		try {
			// This to-lowercase-no-whitespace transformation should probably be done before the expression is passed here.
			string lowercase_nowhitespace = line;
			transform(line.begin(), line.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
			lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
			if (ContainsEqual(lowercase_nowhitespace)) {
				string name;
				string expression;
				ParseExpression(lowercase_nowhitespace, name, expression);
				if (IsTrueValue(expression)) {
					character.SetValue(name, stod(expression));
				}
				else {
					character.SetExpression(name, expression);
				}
			}
		}
		catch (SyntaxError& e) {
			fprintf(stderr, "Got syntax error reading file %s: \n%s", file_path, e.what());
		}
		catch (EvaluationError& e) {
			fprintf(stderr, "Got evaluation error reading file %s: \n%s", file_path, e.what());
		}
	}
}