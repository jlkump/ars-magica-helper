#ifndef TESTS_H
#define TESTS_H

#include "parsing.hpp"
#include "error.hpp"
#include "state_trackers.hpp"

void PrintTestResults(const string& test_name, bool test_result) {
	printf("%s - ", test_name.c_str());
	if (test_result) {
		printf("PASSED\n");
	}
	else {
		printf("FAILED!!!!!\n");
	}
}

bool TestUnevenParentheses() {
	Expression e = Expression("temp");

	try {
		e.SetExpression("( 1 * (8)");
		return false;
	} catch (SyntaxError& e) {
		if (e.GetType() != SyntaxError::UNBALANCED_PARENTHESES) {
			return false;
		}
	}

	try {
		e.SetExpression("rounddown(1))");
		return false;
	}
	catch (SyntaxError& e) {
		if (e.GetType() != SyntaxError::UNBALANCED_PARENTHESES) {
			return false;
		}
	}
	return true;
}

bool TestCyclicDependency() {
	CharacterState s = CharacterState();
	s.SetExpression("creo", "[creoexp]*2.0");
	try {
		s.SetExpression("creoexp", "[creo]*2.0");
	}
	catch (SyntaxError& e) {
		if (e.GetType() == SyntaxError::CYCLIC_DEPENDENCY) {
			return true;
		}
	}
	return false;
}

bool TestExpressionStructure() {
	Expression e = Expression("temp");
	e.SetExpression("8*2+1");
	string structure;
	e.GetPrettyTreeString(structure);
	string expected = "|__+\n    |__*\n    |   |__8\n    |   |__2\n    |__1\n";
	if (structure != expected) {
		cout << "Got: \n" << structure << "Expected: \n" << expected;
		return false;
	}
	return true;
	//ExpressionNode* cur = e.ast_root_;
	//if (cur->node_value_ != "+" || cur->type_ != ExpressionNode::ADDITION) {
	//	return false;
	//}
	//for (ExpressionNode* child : cur->children_) {
	//	if (child->type_ == ExpressionNode::TRUE_VALUE && child->node_value_ != "1") {
	//		return false;
	//	}
	//	if (child->type_ == ExpressionNode::MULTIPLICATION && child->node_value_ != "*") {
	//		return false;
	//	}
	//	else if (child->node_value_ == "*") {
	//		if ((child->children_[0]->node_value_ != "8" && child->children[1] != "2") && (child->children_[0]->node_value_ != "2" && child->children[1] != "8")) {
	//			return false;
	//		}
	//	}
	//	if (child->type_ != ExpressionNode::TRUE_VALUE || child->type_ != ExpressionNode::MULTIPLICATION) {
	//		return false;
	//	}
	//}
	//return true;
}

bool TestExpressionStructure() {
	Expression e = Expression("temp");
	e.SetExpression("8*2+1");
	string structure;
	e.GetPrettyTreeString(structure);
	string expected = "|__+\n    |__*\n    |   |__8\n    |   |__2\n    |__1\n";
	if (structure != expected) {
		cout << "Got: \n" << structure << "Expected: \n" << expected;
		return false;
	}
	return true;
}

void RunTests() {
	PrintTestResults("Uneven Parentheses", TestUnevenParentheses());
	PrintTestResults("Cyclic Dependency", TestCyclicDependency());
	PrintTestResults("Expression Structure", TestExpressionStructure());
}


#endif