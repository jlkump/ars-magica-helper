#include "state_trackers.hpp"

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
		} else {
			if (s.empty()) {
				// We encountered a closed bracket with no corresponding open bracket.
				string err = string("Unbalanced parentheses in given expression: \"");
				err.append(expression);
				err.append("\"\n");
				throw SyntaxError(err, SyntaxError::Type::UNBALANCED_PARENTHESES);
			}
			char open_brace = s.top();
			if ((open_brace == '[' && cur != ']') || (open_brace == '{' && cur != '}') || (open_brace == '(' && cur != ')')) {
				// We encountered the wrong pairing of brackets.
				string err = string("Parentheses are not ordered correctly in expression: \"");
				err.append(expression);
				err.append("\"\nExpected a closing brace to \'");
				err.push_back(open_brace);
				err.append("\' but found \'");
				err.push_back(cur);
				err.append("\' instead.\n");
				throw SyntaxError(err, SyntaxError::Type::INVALID_ORDERED_PARENTHESES);
			}
			// If we get here, everything is OK and we can take the opening bracket off the stack.
			s.pop();
		}
	}
	if (!s.empty()) {
		// If we get here, we still have opening brackets that had no closing bracket pair
		string err = string("Unbalanced parentheses in given expression: \"");
		err.append(expression);
		err.append("\"\n");
		throw SyntaxError(err, SyntaxError::Type::UNBALANCED_PARENTHESES);
	}
	// All good, simply exit control flow
}

static bool IsValidCharOperation(const char op) {
	return op == '*' || op == '/' || op == '+' || op == '-' || op == '^';
}

bool Expression::IsValidStringOperation(const string& op) {
	static const vector<string> kExpressionStrings = { "round", "roundup", "rounddown", "sqrt", "pow", "min", "max" };
	return find(kExpressionStrings.begin(), kExpressionStrings.end(), op) != kExpressionStrings.end();
}

/*
* This function will check that all the substrings within the expression
* that are made of alpha characters are actually supported operations. It will
* also check that no unsupported operations from non-alphanumeric characters
* are used (for example, "%"). Lastly, it will ignore any text within
* square brackets, as that corresponds to a state value. It will check that the
* text within the brackets only contains spaces and alpha characters and is not empty.
*/
void Expression::CheckValidExpressionOperations(const string& expression) {
	vector<string> substrings;
	auto iter = expression.begin();
	while (iter != expression.end()) {
		if (*iter == '[') {
			int alpha_count = 0;
			iter++;
			while (iter != expression.end() && *iter != ']') {
				if (isalpha(*iter)) {
					alpha_count++;
				} else if (!isspace(*iter)) {
					throw SyntaxError("Given expression has invalid variable characters\n", SyntaxError::Type::INVALID_VARIABLE_NAME);
				}
				iter++;
			}
			if (alpha_count == 0) {
				throw SyntaxError("Given expression does not contain any name within brackets []\n", SyntaxError::Type::EMPTY_STATE_VALUE_NAME);
			}
			iter++;
		} else if (isalpha(*iter)) {
			string current_string;
			// add the current word to the list of strings in the expression
			while (iter != expression.end() && isalpha(*iter)) {
				current_string.push_back(*iter);
				iter++;
			}
			auto temp = iter;
			while (temp != expression.end() && isspace(*temp)) {
				temp++;
			}
			if (temp == expression.end() || *temp != '(') {
				throw SyntaxError("Given expression is missing parentheses\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
			}
			substrings.push_back(move(current_string));
		} else {
			if (!isspace(*iter) && !isalnum(*iter)) {
				if (IsValidCharOperation(*iter)) { 
					// if (IsBinaryCharOperation) { // Currently all char operations are binary, but this check will be necessary if that changes
					if ((iter + 1) == expression.end() || (!isalnum(*(iter + 1)) && *(iter + 1) != '[' && *(iter + 1) != '(')) {
						throw SyntaxError("Given expression is binary but missing righthand side\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
					if (iter == expression.begin() || (!isalnum(*(iter - 1)) && *(iter - 1) != ']' && *(iter - 1) != ')')) {
						printf("found error in %s with char %c\n", expression.c_str(), *iter);
						throw SyntaxError("Given expression is binary but missing lefthand side\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
				} else if (*iter != '(' && *iter != ')') {
					// We get here when the char operator is not expected, such as % or ? or "
					throw SyntaxError("Given expression has unexpected operator\n", SyntaxError::Type::INVALID_OPERATION);
				}
			}
			iter++;
		}
	}
	for (string s : substrings) {
		if (!IsValidStringOperation(s)) {
			string err("Given expression does not contain valid operation\nInvalid operation: ");
			err.append(s);
			err.append("\n");
			throw SyntaxError(err, SyntaxError::Type::INVALID_OPERATION);
		}
	}

}

string FindRootOperation(const string& expression) {
	auto iter = expression.begin();
	string result;
	int opening_brace_count = 0;
	while (iter != expression.end()) {
		if (*iter == '(') {
			opening_brace_count++;
		}
		if (opening_brace_count == 0 && isalpha(*iter)) {

		}
		if (opening_brace_count == 0) {

		}
	}
}

void Expression::ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result) {
	CheckEvenParentheses(expression); // Will throw a SyntaxError if the values are not equal.
	CheckValidExpressionOperations(expression); // Will throw a SyntaxError if the operations given are not supported.
	string root_operation = FindRootOperation(expression);
}

float Expression::GetValue(const GameState& game_state, const CharacterState& character_state) const
{
	return 0.0f;
}

void Expression::SetExpression(const string& expression)
{
	Expression::ExpressionNode* temp = new ExpressionNode();
	// This to-lowercase-no-whitespace transformation should probably be done before the expression is passed here.
	string lowercase_nowhitespace = expression;
	transform(expression.begin(), expression.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
	lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
	ParseAndBuildExpressionTree(lowercase_nowhitespace, temp);
	delete temp;
}

float CharacterState::GetValue(const GameState& game_state, const string& value_name) const
{
	return 0.0f;
}

const string& CharacterState::GetStringValue(const string& value_name) const
{
	return value_name;
}

void CharacterState::SetValue(const string& value_name, const float value)
{
}

void CharacterState::SetValue(const string& value_name, const string& value)
{
}

void CharacterState::SetExpression(const string& value_name, const string& expression)
{
}

void CharacterState::AddCallbackOnValueChange(const string& value_name, void(*callback)(const float))
{

}
