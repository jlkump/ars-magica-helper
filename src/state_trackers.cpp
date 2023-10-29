#include "state_trackers.hpp"

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
		// It is possible that an expression equals exactly another expression in this case
		// Might want to account for that later
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

/*
* Given an expression (as defined in state_trackers.hpp), this function will parse the expression,
* making sure it is in the correct format. If it is, it will build an expression tree which will
* have the root placed in the param: result. If it is not in correct syntax format, then a 
* SyntaxError will be thrown with the appropriate error message associated with the syntax mistake.
* 
* This is an internal helper function that is used for the creation of and modification of the AST
* of an Expression. For more details, see state_trackers.hpp --> Expression and ExpressionNode definitions.
*/
void Expression::ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result) {
	CheckEvenParentheses(expression); // Will throw a SyntaxError if the values are not equal.
	CheckValidExpressionOperations(expression); // Will throw a SyntaxError if the operations given are not supported.

	// This struct is to make parsing the expression easier in terms of building a tree
	// It associates some sub-expression with the parent of the sub-expression to make
	// the tree building easy.
	struct expression_parse_item {
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
			child->node_value_ = current.expression.substr(1, current.expression.size() - 2); // Chop off the brackets []
		} else {
			FindRootOperation(current.expression, operation, sub_expressions);
			child = new ExpressionNode(GetOperationNodeType(operation));
			if (child->type_ == ExpressionNode::INVALID) {
				delete result;
				result = nullptr;
				throw SyntaxError("Invalid expression\n", SyntaxError::Type::ILLFORMED_EXPRESSION);
			}
			child->node_value_ = operation; // Storing the operation isn't strictly necessary, but might as well since we have the space
			for (const string& exp : sub_expressions) {
				items.push(expression_parse_item(exp, child));
			}
			sub_expressions.clear();
		}
		current.parent_of_expression->children_.push_back(child);
		items.pop();
	}
}

float Expression::GetValueRecursiveHelper(const ExpressionNode* cur, const GameState& game_state, const CharacterState& character_state) const {
	float scratch = 0.0f;
	int index_scratch = 0;
	switch (cur->type_) {
	case Expression::ExpressionNode::Type::ADDITION:
		return GetValueRecursiveHelper(cur->children_[0], game_state, character_state) + GetValueRecursiveHelper(cur->children_[1], game_state, character_state);
	case Expression::ExpressionNode::Type::SUBTRACTION:
		return GetValueRecursiveHelper(cur->children_[0], game_state, character_state) - GetValueRecursiveHelper(cur->children_[1], game_state, character_state);
	case Expression::ExpressionNode::Type::DIVISION:
		scratch = GetValueRecursiveHelper(cur->children_[1], game_state, character_state);
		if (scratch == 0.0) {
			throw EvaluationError(FormatString("Divide by zero in expression of name \"%s\".\n", name_), EvaluationError::DIVIDE_BY_ZERO);
		}
		return GetValueRecursiveHelper(cur->children_[0], game_state, character_state) / scratch;
	case Expression::ExpressionNode::Type::MULTIPLICATION:
		return GetValueRecursiveHelper(cur->children_[0], game_state, character_state) * GetValueRecursiveHelper(cur->children_[1], game_state, character_state);
	case Expression::ExpressionNode::Type::POW:
		return pow(GetValueRecursiveHelper(cur->children_[0], game_state, character_state), GetValueRecursiveHelper(cur->children_[1], game_state, character_state));
	case Expression::ExpressionNode::Type::ROUND:
		return round(GetValueRecursiveHelper(cur->children_[0], game_state, character_state));
	case Expression::ExpressionNode::Type::ROUND_DOWN:
		return floor(GetValueRecursiveHelper(cur->children_[0], game_state, character_state));
	case Expression::ExpressionNode::Type::ROUND_UP:
		return ceil(GetValueRecursiveHelper(cur->children_[0], game_state, character_state));
	case Expression::ExpressionNode::Type::SQRT:
		return sqrt(GetValueRecursiveHelper(cur->children_[0], game_state, character_state));
	case Expression::ExpressionNode::Type::MIN:
		scratch = GetValueRecursiveHelper(cur->children_[0], game_state, character_state);
		for (index_scratch = 1; index_scratch < cur->children_.size(); index_scratch++) {
			scratch = min(scratch, GetValueRecursiveHelper(cur->children_[index_scratch], game_state, character_state));
		}
		return scratch;
	case Expression::ExpressionNode::Type::MAX:
		scratch = GetValueRecursiveHelper(cur->children_[0], game_state, character_state);
		for (index_scratch = 1; index_scratch < cur->children_.size(); index_scratch++) {
			scratch = max(scratch, GetValueRecursiveHelper(cur->children_[index_scratch], game_state, character_state));
		}
		return scratch;
	case Expression::ExpressionNode::Type::TRUE_VALUE:
		return stod(cur->node_value_);
	case Expression::ExpressionNode::Type::STATE_VALUE:
		scratch = 0.0f;
		if (!character_state.GetValue(game_state, cur->node_value_, scratch)) {
			// TODO: Try game-state as well then throw exception if that fails too.
			throw EvaluationError(FormatString("Could not find state value \"%s\" in the game or character state.\n", cur->node_value_), EvaluationError::NONEXISTANT_STATE_VALUE);
		}
		return scratch;
	case Expression::ExpressionNode::Type::INVALID:
		throw EvaluationError(FormatString("Found invalid node while evaluating expression of name \"%s\".\n", name_), EvaluationError::INVALID_NODE);
	}
	throw EvaluationError(FormatString("Found unrecognized node type while evaluating expression of name \"%s\"", name_), EvaluationError::INVALID_NODE);
}

float Expression::GetValue(const GameState& game_state, const CharacterState& character_state) const {
	if (ast_root_ == nullptr) {
		return 0.0f;
	}
	return GetValueRecursiveHelper(ast_root_, game_state, character_state);
}

void Expression::SetExpression(const string& expression)
{
	ParseAndBuildExpressionTree(expression, ast_root_);
	cout << "Expression Tree" << endl;
	PrintTreePretty();
}

/*
* Code from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c, slightly modified
*/
void Expression::GetPrettyTreeRecursiveHelper(string& result, const string& prefix, const ExpressionNode* node, bool end) {
	if (node != nullptr)
	{
		result.append(prefix);
		result.append("|__");
		// print the value of the node
		result.append(node->node_value_);
		result.append("\n");

		for (int i = 0; i < node->children_.size(); i++) {
			const ExpressionNode* child = node->children_[i];
			GetPrettyTreeRecursiveHelper(result, prefix + (end ? "    " : "|   "), child, i == node->children_.size() - 1);
		}
	}
}

void Expression::PrintTreePretty() const {
	string result;
	GetPrettyTreeString(result);
	cout << result << endl;
	
}

void Expression::GetPrettyTreeString(string& result) const {
	GetPrettyTreeRecursiveHelper(result, "", ast_root_, true);
}

void Expression::GetStateValuesRecursiveHelper(const ExpressionNode* cur, vector<string>& result) const {
	if (cur == nullptr) {
		return;
	}
	if (cur->type_ == ExpressionNode::STATE_VALUE) {
		result.push_back(cur->node_value_);
	} else if (cur->type_ != ExpressionNode::TRUE_VALUE) {
		for (const ExpressionNode* c : cur->children_) {
			GetStateValuesRecursiveHelper(c, result);
		}
	}
}

void Expression::GetStateValues(vector<string>& state_value_names) const {
	GetStateValuesRecursiveHelper(ast_root_, state_value_names);
}


////////////////////
// CharacterState //
////////////////////
CharacterState::CharacterState() {

}

CharacterState::~CharacterState() {
	// TODO: delete graphs, expressions, etc.
}

bool CharacterState::GetValue(const GameState& game_state, const string& value_name, float& result) const
{
	if (cached_expression_vals_.count(value_name) != 0) {
		// Dumb C++ shenanigans. It won't let me access the map using the operator[] since it could be modified.
		result = (*const_cast<unordered_map<string, float>*>(&cached_expression_vals_))[value_name];
	}
	else if (expression_vals_.count(value_name) != 0) {
		// More shenanigans. Since we are just caching the value, this shouldn't actually count as a state change for character_state
		// TODO: Update Cache through method
		(*const_cast<unordered_map<string, float>*>(&cached_expression_vals_))[value_name] = expression_vals_.at(value_name)->GetValue(game_state, *this);
		result = (*const_cast<unordered_map<string, float>*>(&cached_expression_vals_))[value_name];
	}
	else if (base_vals_.count(value_name) != 0) {
		// Even more!!
		result = (*const_cast<unordered_map<string, float>*>(&base_vals_))[value_name];
	}
	else {
		return false;
	}
	return true;
}

const string& CharacterState::GetStringValue(const string& value_name) const
{
	return value_name;
}

void CharacterState::SetValue(const string& value_name, const float value)
{
	base_vals_[value_name] = value;
}

void CharacterState::SetValue(const string& value_name, const string& value)
{
	string_vals_[value_name] = value;
}

void CharacterState::SetExpression(const string& value_name, const string& expression)
{
	if (expression_vals_.count(value_name) == 0) {
		// Add the expression if it doesn't exist yet
		expression_vals_[value_name] = new Expression(value_name);
	}
	expression_vals_[value_name]->SetExpression(expression);
	// In either case, we must update the expression dependency graph
	// (This shouldn't ever be a null deref, but maybe it could happen?)
	UpdateDependencyGraph(*expression_vals_[value_name]);
}

bool CharacterState::DoesDependencyContainCycleRecursiveHelper(const DependencyNode* cur, unordered_set<string>& visited, unordered_set<string>& on_stack) const {
	if (on_stack.count(cur->name_) >= 1) {
		return true;
	}
	if (visited.count(cur->name_) >= 1) {
		return false;
	}

	visited.insert(cur->name_);
	on_stack.insert(cur->name_);
	for (const DependencyNode* c : cur->children_) {
		if (DoesDependencyContainCycleRecursiveHelper(c, visited, on_stack)) {
			return true;
		}
	}
	on_stack.erase(cur->name_);
	return false;
}

bool CharacterState::DoesDependencyContainCycle(const DependencyNode* current) const {
	unordered_set<string> visited;
	unordered_set<string> on_stack;
	return DoesDependencyContainCycleRecursiveHelper(current, visited, on_stack);
}

void CharacterState::UpdateDependencyGraph(const Expression& expression) 
{
	if (dependency_graphs_.count(expression.GetName()) == 0) {
		// Create new dependency graph
		dependency_graphs_[expression.GetName()] = new DependencyNode();
		dependency_graphs_[expression.GetName()]->name_ = expression.GetName();
	}
	vector<string> state_names;
	vector<string> undo_dependencies;
	unordered_map<string, vector<CharacterState::DependencyNode*>::const_iterator> undos;
	expression.GetStateValues(state_names);
	for (const string& name : state_names) {
		if (dependency_graphs_.count(name) == 0) {
			// Create new dependency graph
			dependency_graphs_[name] = new DependencyNode();
			dependency_graphs_[name]->name_ = name;
			undo_dependencies.push_back(name);
		}
		dependency_graphs_[name]->children_.push_back(dependency_graphs_[expression.GetName()]);
		undos[name] = (dependency_graphs_[name]->children_.end() - 1);
	}
	// Now detect if there is a cycle and undo if there is
	if (DoesDependencyContainCycle(dependency_graphs_[expression.GetName()])) {
		// TODO: Keep track of graphs to undo
		for (const string& name : state_names) {
			dependency_graphs_[name]->children_.erase(undos[name]);
		}
		for (const string& undo_name : undo_dependencies) {
			delete dependency_graphs_[undo_name];
			dependency_graphs_.erase(undo_name);
		}
		dependency_graphs_.erase(expression.GetName());
		throw SyntaxError(FormatString("Expression %s has a cyclic dependency.\n", expression.GetName().c_str()), SyntaxError::CYCLIC_DEPENDENCY);
	}
	PrintDependencyGraphPretty(expression.GetName());
}

void CharacterState::GetPrettyTreeRecursiveHelper(string& result, const string& prefix, const DependencyNode* node, bool end) const {
	if (node != nullptr)
	{
		result.append(prefix);
		result.append("|__");
		// print the value of the node
		result.append(node->name_);
		result.append("\n");

		for (int i = 0; i < node->children_.size(); i++) {
			const DependencyNode* child = node->children_[i];
			GetPrettyTreeRecursiveHelper(result, prefix + (end ? "    " : "|   "), child, i == node->children_.size() - 1);
		}
	}
}

void CharacterState::GetPrettyTreeString(string& result, const string& value_name) const {
	if (dependency_graphs_.count(value_name) == 0) {
		return;
	}
	GetPrettyTreeRecursiveHelper(result, "", dependency_graphs_.at(value_name), true);
}

void CharacterState::PrintDependencyGraphPretty(const string& value_name) const {
	string result;
	GetPrettyTreeString(result, value_name);
	cout << result << endl;
}

void CharacterState::AddCallbackOnValueChange(const string& value_name, void(*callback)(const float))
{
	callbacks_[value_name].push_back(callback);
}

// Maybe add a callback for changing string value?
