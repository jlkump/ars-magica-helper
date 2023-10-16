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

bool Expression::IsValidCharOperation(const char op) {
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
					throw SyntaxError("Given variable has invalid characters characters\n", SyntaxError::Type::INVALID_VARIABLE_NAME);
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
				} else if (*iter != '(' && *iter != ')' && *iter != '.' && *iter != ',') {
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

int Expression::GetOperationPrecedence(const string& op) {
	static const unordered_map<string, int> kOperationPrecedence = {
		{"round", 0}, {"rounddown", 0}, {"roundup", 0}, {"min", 0}, {"max", 0}, {"sqrt", 0}, {"pow", 0}, {"^", 0},
		{"-", 1}, {"+", 1}, 
		{"*", 2}, {"/", 2}
	};
	if (kOperationPrecedence.count(op) == 0) {
		return -1;
	}
	return kOperationPrecedence.at(op);
}

Expression::ExpressionNode::Type Expression::GetOperationNodeType(const string& op) {
	static const unordered_map<string, Expression::ExpressionNode::Type> kStringsToEnumMap = {
		{"-", Expression::ExpressionNode::Type::SUBTRACTION}, {"+",  Expression::ExpressionNode::Type::ADDITION},
		{"*",  Expression::ExpressionNode::Type::MULTIPLICATION}, {"/",  Expression::ExpressionNode::Type::DIVISION},
		{"round",  Expression::ExpressionNode::Type::ROUND}, {"rounddown",  Expression::ExpressionNode::Type::ROUND_DOWN},
		{"roundup",  Expression::ExpressionNode::Type::ROUND_UP}, {"sqrt",  Expression::ExpressionNode::Type::SQRT},
		{"pow",  Expression::ExpressionNode::Type::POW}, {"^",  Expression::ExpressionNode::Type::POW},
		{"min",  Expression::ExpressionNode::Type::MIN}, {"max",  Expression::ExpressionNode::Type::MAX},
	};
	if (kStringsToEnumMap.count(op) == 0) {
		if (*op.begin() == '[') {
			return Expression::ExpressionNode::Type::STATE_VALUE;
		} else if (*op.begin() == '.' || (*op.begin() >= '0' && *op.begin() <= '9')) {
			// Is a flat value
			return Expression::ExpressionNode::Type::TRUE_VALUE;
		} else {
			return Expression::ExpressionNode::Type::INVALID;
		}
	}
	return kStringsToEnumMap.at(op);
}

/*
* A fairly long function. To understand it, the steps are as follows:
* 1. Cut off unneccessary parentheses from the begining and end of the expression.
* 2. Find the possible "root" operations in the expression. There could be multiple root operations
*    so pick between the one with the highest precedence. When precedence is tied, pick the one furthest left.
*/
void Expression::FindRootOperation(const string& expression, string& operation, queue<string>& subexpressions) {
	auto begin = expression.begin();
	auto end = expression.end();
	int opening_brace_count = 0;
	bool found_root_op = false;

	vector<string> possible_root_operations;
	vector<string::const_iterator> possible_root_operation_ptrs;
	vector<int> precedences;
	int max_precedence = -1;
	while (*begin == '(' && *(end-1) == ')') {
		begin++;
		end--;
	}
	auto iter = begin;
	while (iter != end) {
		printf("Currently on character %c with brace count %d\n", *iter, opening_brace_count);
		if (*iter == '(') {
			opening_brace_count++;
		} else if (*iter == ')') {
			opening_brace_count--;
		} else if (*iter == '[') {
			// Ignore stuff between brackets
			while (iter != end && *iter != ']') iter++;
		} else if (opening_brace_count == 0 && isalpha(*iter)) {
			// Possible root operation
			string op;
			while (iter != end && isalpha(*iter)) {
				op.push_back(*iter);
				iter++;
			}
			int precedence = GetOperationPrecedence(op);
			if (precedence > max_precedence) {
				max_precedence = precedence;
			}
			precedences.push_back(precedence);
			possible_root_operations.push_back(op);
			printf("Found operation: %s with precedence %d\n", op.c_str(), precedence);
			possible_root_operation_ptrs.push_back(iter);
			opening_brace_count++; // This is done b/c the iter will skip over the opening parentheses for the operation, so just add to brace count manually.
		} else if (opening_brace_count == 0 && IsValidCharOperation(*iter)) {
			string op;
			op.push_back(*iter);
			int precedence = GetOperationPrecedence(op);
			if (precedence > max_precedence) {
				max_precedence = precedence;
			}
			precedences.push_back(precedence);
			possible_root_operations.push_back(op);
			printf("Found operation: %s with precedence %d\n", op.c_str(), precedence);
			possible_root_operation_ptrs.push_back(iter);
		}
		iter++;
	}

	int count_of_equal_precedence = 0;
	int root_op_index = -1;
	// Because we start from the back, the root_op_index will always be the left-most highest precedence operation index.
	for (int i = precedences.size() - 1; i >= 0; i--) {
		printf("Currently possible operation %s with precedence %d\n", possible_root_operations[i].c_str(), precedences[i]);
		if (precedences[i] == max_precedence) {
			count_of_equal_precedence++;
			root_op_index = i;
		}
	}
	if (root_op_index == -1) {
		throw SyntaxError("Expression is poorly formed or not an expression.\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
	}
	operation = possible_root_operations[root_op_index];
	iter = possible_root_operation_ptrs[root_op_index];

	Expression::ExpressionNode::Type type = GetOperationNodeType(operation);
	string subexp;
	auto left_iter = begin;
	auto right_iter = iter + 1;
	switch (type) {
	case Expression::ExpressionNode::Type::ADDITION:
	case Expression::ExpressionNode::Type::SUBTRACTION:
	case Expression::ExpressionNode::Type::DIVISION:
	case Expression::ExpressionNode::Type::MULTIPLICATION:
		// This is for binary operations that have a single char operator
		while (left_iter != iter) {
			subexp.push_back(*left_iter);
			left_iter++;
		}
		subexpressions.push(subexp);
		subexp.clear();
		while (right_iter != end) {
			subexp.push_back(*right_iter);
			right_iter++;
		}
		subexpressions.push(subexp);
		break;
	case Expression::ExpressionNode::Type::POW:
		if (expression == "^") {
			// Special case for the char '^' as the power operation
			while (left_iter != iter) {
				subexp.push_back(*left_iter);
				left_iter++;
			}
			subexpressions.push(subexp);
			subexp.clear();
			while (right_iter != end) {
				subexp.push_back(*right_iter);
				right_iter++;
			}
			subexpressions.push(subexp);
			break;
		}
	case Expression::ExpressionNode::Type::ROUND:
	case Expression::ExpressionNode::Type::ROUND_DOWN:
	case Expression::ExpressionNode::Type::ROUND_UP:
	case Expression::ExpressionNode::Type::SQRT:
	case Expression::ExpressionNode::Type::MIN:
	case Expression::ExpressionNode::Type::MAX:
		while (iter != end && isalpha(*iter)) {
			iter++;
		}
		iter++;
		end--;
		while (iter != end) {
			if (*iter == ',') {
				subexpressions.push(subexp);
				subexp.clear();
			}
			else {
				subexp.push_back(*iter);
			}
			iter++;
		}
		subexpressions.push(subexp);
		break;
	case Expression::ExpressionNode::Type::INVALID:
	case Expression::ExpressionNode::Type::TRUE_VALUE:
	case Expression::ExpressionNode::Type::STATE_VALUE:
		// This should never actually happen if we check our expression before passing it to this function.
		throw SyntaxError("Invalid syntax for expression\n", SyntaxError::ILLFORMED_EXPRESSION);
	}
}

void Expression::ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result) {
	CheckEvenParentheses(expression); // Will throw a SyntaxError if the values are not equal.
	CheckValidExpressionOperations(expression); // Will throw a SyntaxError if the operations given are not supported.

	string operation;
	queue<string> sub_expressions;
	FindRootOperation(expression, operation, sub_expressions);
	result = new ExpressionNode(GetOperationNodeType(operation));
	while (!sub_expressions.empty()) {
		Expression::ExpressionNode::Type t = GetOperationNodeType(sub_expressions.front());
		switch () {

		}
		sub_expressions.pop();
	}

	queue<string> substrings;
	substrings.push(expression);
	while (!substrings.empty()) {
		string current = substrings.front();
		substrings.pop();
		if (IsNumericOnly(current)) {

		} else if (IsStateValue(current)) {

		} else {

			for (const string& s : sub_expressions) {
				substrings.push(s);
			}
		}
	}
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
