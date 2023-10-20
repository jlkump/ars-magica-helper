﻿#include "state_trackers.hpp"

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

Expression::ExpressionNode::~ExpressionNode() {
	for (ExpressionNode* child : this->children_) {
		delete child;
	}
}

bool Expression::IsValidCharOperation(const char op) {
	return op == '*' || op == '/' || op == '+' || op == '-' || op == '^';
}

bool Expression::IsValidStringOperation(const string& op) {
	static const vector<string> kExpressionStrings = { "round", "roundup", "rounddown", "sqrt", "pow", "min", "max" };
	return find(kExpressionStrings.begin(), kExpressionStrings.end(), op) != kExpressionStrings.end();
}

bool Expression::IsValidVariableName(const string& var) {
	auto iter = var.begin();
	while (iter != var.end()) {
		if (!isspace(*iter) && !isalpha(*iter)) {
			return false;
		}
		iter++;
	}
	return true;
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
	auto iter = expression.begin();
	while (iter != expression.end()) {
		if (*iter == '[') {
			iter++;
			string variable;
			while (iter != expression.end() && *iter != ']') {
				variable.push_back(*iter);
				iter++;
			}
			if (!IsValidVariableName(variable)) {
				throw SyntaxError(
					FormatString("Given variable [%s] contains invalid characters. Only use letters and spaces.\n", variable.c_str()),
					SyntaxError::Type::INVALID_VARIABLE_NAME);
			}
			if (variable.size() == 0) {
				throw SyntaxError("Given expression does not contain any name within brackets []\n", SyntaxError::Type::EMPTY_STATE_VALUE_NAME);
			}
			iter++;
		} else if (isalpha(*iter)) {
			string op_string;
			// add the current word to the list of strings in the expression
			while (iter != expression.end() && isalpha(*iter)) {
				op_string.push_back(*iter);
				iter++;
			}
			auto temp = iter;
			while (temp != expression.end() && isspace(*temp)) {
				temp++;
			}
			if (temp == expression.end() || *temp != '(') {
				if (!IsValidStringOperation(op_string)) {
					throw SyntaxError(
						FormatString("Given operation \"%s\" is not a valid operation.\n",
							op_string.c_str()), SyntaxError::Type::ILLFORMED_EXPRESSION);
				} else {
					throw SyntaxError(
						FormatString("Given operation \"%s\" is missing parentheses. Should be \"%s(...)\" instead.\n",
							op_string.c_str(), op_string.c_str()), SyntaxError::Type::ILLFORMED_EXPRESSION);
				}
			}
		} else if (*iter == '(' && (iter + 1 == expression.end() || *(iter + 1) == ')')) {
			string op_string;
			iter--;
			while (iter != expression.begin() && isalpha(*iter)) {
				op_string.insert(0, 1, *iter);
				iter--;
			}
			if (isalpha(*iter)) {
				op_string.insert(0, 1, *iter);
			}
			if (!IsValidStringOperation(op_string)) {
				throw SyntaxError(
					FormatString("Given operation \"%s\" is not a valid operation.\n",
						op_string.c_str()), SyntaxError::Type::ILLFORMED_EXPRESSION);
			} else {
				throw SyntaxError(
					FormatString("Given operation \"%s\" has no parameters.\n", op_string.c_str()),
					SyntaxError::Type::EMPTY_OPERATION);
			}
		} else {
			if (!isspace(*iter) && !isalnum(*iter)) {
				if (IsValidCharOperation(*iter)) { 
					// if (IsBinaryCharOperation) { // Currently all char operations are binary, but this check will be necessary if that changes
					if ((iter + 1) == expression.end() || (!isalnum(*(iter + 1)) && *(iter + 1) != '[' && *(iter + 1) != '(')) {
						throw SyntaxError(
							FormatString("Given operation \'%c\' is binary but missing righthand side.\n", *iter), 
							SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
					if (iter == expression.begin() || (!isalnum(*(iter - 1)) && *(iter - 1) != ']' && *(iter - 1) != ')')) {
						throw SyntaxError(
							FormatString("Given operation \'%c\' is binary but missing lefthand side.\n", *iter), 
							SyntaxError::Type::ILLFORMED_EXPRESSION);
					}
				} else if (*iter != '(' && *iter != ')' && *iter != '.' && *iter != ',') {
					// We get here when the char operator is not expected, such as % or ? or "
					throw SyntaxError(
						FormatString("Given operation \'%c\' is not supported.\n", *iter), 
						SyntaxError::Type::INVALID_OPERATION);
				}
			}
			iter++;
		}
	}
}

int Expression::GetOperationPrecedence(const string& op) {
	static const unordered_map<string, int> kOperationPrecedence = {
		{"round", 0}, {"rounddown", 0}, {"roundup", 0}, {"min", 0}, {"max", 0}, {"sqrt", 0}, {"pow", 0}, {"^", 0},
		{"*", 1}, {"/", 1},
		{"-", 2}, {"+", 2}, 
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
		if (op.begin() == op.end()) {
			return Expression::ExpressionNode::Type::INVALID;
		}
		if (*op.begin() == '[' && *(op.end() - 1) == ']') {
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
void Expression::FindRootOperation(const string& expression, string& operation, vector<string>& subexpressions) {
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
			possible_root_operation_ptrs.push_back(iter);
		}
		iter++;
	}

	int count_of_equal_precedence = 0;
	int root_op_index = -1;
	// Had precedence reversed before. Fixed now (Thanks Kevin)
	for (int i = 0; i < precedences.size(); i++) {
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
		// We parse the expression on the left and right sides of the operator
		while (left_iter != iter) {
			subexp.push_back(*left_iter);
			left_iter++;
		}
		subexpressions.push_back(subexp);
		subexp.clear();
		while (right_iter != end) {
			subexp.push_back(*right_iter);
			right_iter++;
		}
		subexpressions.push_back(subexp);
		break;
	case Expression::ExpressionNode::Type::POW:
		if (*iter == '^') {
			// Special case for the char '^' as the power operation
			while (left_iter != iter) {
				subexp.push_back(*left_iter);
				left_iter++;
			}
			subexpressions.push_back(subexp);
			subexp.clear();
			while (right_iter != end) {
				subexp.push_back(*right_iter);
				right_iter++;
			}
			subexpressions.push_back(subexp);
			break;
		}
	case Expression::ExpressionNode::Type::ROUND:
	case Expression::ExpressionNode::Type::ROUND_DOWN:
	case Expression::ExpressionNode::Type::ROUND_UP:
	case Expression::ExpressionNode::Type::SQRT:
	case Expression::ExpressionNode::Type::MIN:
	case Expression::ExpressionNode::Type::MAX:
		// Here we parse a comma seperated list of sub-expressions
		while (iter != end && isalpha(*iter)) {
			iter++;
		}
		iter++;
		end--;
		while (iter != end) {
			if (*iter == ',') {
				subexpressions.push_back(subexp);
				subexp.clear();
			}
			else {
				subexp.push_back(*iter);
			}
			iter++;
		}
		subexpressions.push_back(subexp);
		break;
	case Expression::ExpressionNode::Type::INVALID:
	case Expression::ExpressionNode::Type::TRUE_VALUE:
	case Expression::ExpressionNode::Type::STATE_VALUE:
		// This should never actually happen if we check our expression before passing it to this function.
		throw SyntaxError("Invalid syntax for expression\n", SyntaxError::ILLFORMED_EXPRESSION);
	}
}

bool IsTrueValue(string& s) {
	int dot_count = 0;
	for (const char c : s) {
		if (c == '.') {
			dot_count++;
		} else if (!(c >= '0' && c <= '9')) {
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

void Expression::ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result) {
	CheckEvenParentheses(expression); // Will throw a SyntaxError if the values are not equal.
	CheckValidExpressionOperations(expression); // Will throw a SyntaxError if the operations given are not supported.

	typedef struct expression_parse_item {
		string expression;
		ExpressionNode* parent_of_expression;
		expression_parse_item(const string& e, ExpressionNode* p) : expression(e), parent_of_expression(p) {}
	};

	string operation;
	vector<string> sub_expressions;
	FindRootOperation(expression, operation, sub_expressions);
	result = new ExpressionNode(GetOperationNodeType(operation));
	result->node_value_ = operation;
	queue<expression_parse_item> items;
	for (const string& exp : sub_expressions) {
		items.push(expression_parse_item(exp, result));
	}
	sub_expressions.clear();


	while (!items.empty()) {
		expression_parse_item current = items.front();
		TrimExcessParentheses(current.expression);
		ExpressionNode* child = nullptr;
		if (IsTrueValue(current.expression)) {
			child = new ExpressionNode(ExpressionNode::TRUE_VALUE);
			child->node_value_ = current.expression;
		} else if (IsStateValue(current.expression)) {
			child = new ExpressionNode(ExpressionNode::STATE_VALUE);
			child->node_value_ = current.expression.substr(1, current.expression.size() - 2);
		} else {
			FindRootOperation(current.expression, operation, sub_expressions);
			child = new ExpressionNode(GetOperationNodeType(operation));
			if (child->type_ == ExpressionNode::INVALID) {
				delete result;
				result = nullptr;
				throw SyntaxError("Invalid expression\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
			}
			child->node_value_ = operation;
			for (const string& exp : sub_expressions) {
				items.push(expression_parse_item(exp, child));
			}
			sub_expressions.clear();
		}
		current.parent_of_expression->children_.push_back(child);
		items.pop();
	}
}

float Expression::GetValue(const GameState& game_state, const CharacterState& character_state) const
{
	// TODO: Complete GetValue
	stack<ExpressionNode*> depth_iter;
	depth_iter.push(ast_root_);
	while (!depth_iter.empty()) {
		if (depth_iter.top()->type_ != ExpressionNode::TRUE_VALUE || 
			depth_iter.top()->type_ != ExpressionNode::STATE_VALUE) {
			for (ExpressionNode* child : depth_iter.top()->children_) {
				depth_iter.push(child);
			}
		}
	}

	return 0.0f;
}

/*
* Code from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
*/
void Expression::PrintTreePrettyRecursiveHelper(const std::string& prefix, const ExpressionNode* node, bool end) {
	if (node != nullptr)
	{
		cout << prefix;

		cout << (end ? "|__" : "|__");
		// print the value of the node
		if (node->type_ == ExpressionNode::TRUE_VALUE) {
			std::cout << node->node_value_ << std::endl;
		} else if (node->type_ == ExpressionNode::STATE_VALUE) {
			std::cout << node->node_value_ << std::endl;
		} else {
			std::cout << node->node_value_ << std::endl;
		}

		for (int i = 0; i < node->children_.size(); i++) {
			const ExpressionNode* child = node->children_[i];
			PrintTreePrettyRecursiveHelper(prefix + (end ? "    " : "|   "), child, i == node->children_.size() - 1);
		}
	}
}

void Expression::PrintTreePretty(const ExpressionNode* root) {
	PrintTreePrettyRecursiveHelper("", root, true);
}

void Expression::SetExpression(const string& expression)
{
	// This to-lowercase-no-whitespace transformation should probably be done before the expression is passed here.
	string lowercase_nowhitespace = expression;
	transform(expression.begin(), expression.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
	lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
	ParseAndBuildExpressionTree(lowercase_nowhitespace, ast_root_);
	PrintTreePretty(ast_root_);
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