#include "state_trackers.hpp"

/*
* This function will check that the parentheses of a given string are balanced. If they are not,
* then a SyntaxError of appropriate type will be thrown.
*/
void CheckEvenParentheses(const string& expression) {
	stack<char> s;
	for (char cur : expression) {
		if (cur != '[' && cur != ']' && cur != '(' && cur != ')' && cur != '{' && cur != '}') {
			// Ignore chars that are not parentheses
			continue;
		}

		if (cur == '[' || cur == '{' || cur == '(') {
			s.push(cur);
		}
		else {
			if (s.empty()) {
				string err = string("Unbalanced parentheses in given expression: \"");
				err.append(expression);
				err.append("\"\n");
				throw SyntaxError(err, SyntaxError::Type::UNBALANCED_PARENTHESES);
			}
			char open_brace = s.top();
			if ((open_brace == '[' && cur != ']') || (open_brace == '{' && cur != '}') || (open_brace == '(' && cur != ')')) {
				string err = string("Parentheses are not ordered correctly in expression: \"");
				err.append(expression);
				err.append("\"\nExpected a closing brace to \'");
				err.push_back(open_brace);
				err.append("\' but found \'");
				err.push_back(cur);
				err.append("\' instead.\n");
				throw SyntaxError(err, SyntaxError::Type::INVALID_ORDERED_PARENTHESES);
			}
			s.pop();
		}
	}
	// If we still have opening parantheses, we need to throw an error
	if (!s.empty()) {
		string err = string("Unbalanced parentheses in given expression: \"");
		err.append(expression);
		err.append("\"\n");
		throw SyntaxError(err, SyntaxError::Type::UNBALANCED_PARENTHESES);
	}
}


void Expression::CheckValidExpressionOperations(const string& expression) {
	static const vector<string> kExpressionStrings = {"round", "roundup", "rounddown", "sqrt", "pow", "min", "max"};
	auto iter = expression.begin();
	vector<string> substrings;
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
				if (*iter != '*' && *iter != '/' && *iter != '+' && *iter != '-' && *iter != '^' && *iter != '(' && *iter != ')') {
					throw SyntaxError("Given expression has unexpected operator\n", SyntaxError::Type::INVALID_OPERATION);
				} else if (*iter == '*' || *iter == '/' || *iter == '+' || *iter == '-' || *iter == '^') {
					if ((iter + 1) == expression.end() || (!isalnum(*(iter + 1)) && *(iter + 1) != '[' && *(iter + 1) != '(')) {
						throw SyntaxError("Given expression is binary but missing righthand side\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
					if (iter == expression.begin() || (!isalnum(*(iter - 1)) && *(iter - 1) != ']' && *(iter - 1) != ')')) {
						printf("found error in %s with char %c\n", expression.c_str(), *iter);
						throw SyntaxError("Given expression is binary but missing lefthand side\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
				}
			}
			iter++;
		}
	}
	for (string s : substrings) {
		bool has_match = find(kExpressionStrings.begin(), kExpressionStrings.end(), s) != kExpressionStrings.end();
		if (!has_match) {
			string err("Given expression does not contain valid operation\nInvalid operation: ");
			err.append(s);
			err.append("\n");
			throw SyntaxError(err, SyntaxError::Type::INVALID_OPERATION);
		}
	}

}

void Expression::ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result) {
	string lowercase_nowhitespace = expression;
	transform(expression.begin(), expression.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
	lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
	CheckEvenParentheses(lowercase_nowhitespace); // Will throw a SyntaxError if the values are not equal.
	CheckValidExpressionOperations(lowercase_nowhitespace); // Will throw a SyntaxError if the operations given are not supported.
	printf("Parsed string: %s\n", lowercase_nowhitespace.c_str());

}

float Expression::GetValue(const GameState& game_state, const CharacterState& character_state) const
{
	return 0.0f;
}

void Expression::SetExpression(const string& expression)
{
	Expression::ExpressionNode* temp = new ExpressionNode();
	ParseAndBuildExpressionTree(expression, temp);
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
