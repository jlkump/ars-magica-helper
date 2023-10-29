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

bool TestExpressionStructure_TestOne() {
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

bool TestExpressionStructure_TestTwo() {
	Expression e = Expression("temp");
	e.SetExpression("8*rounddown(2)+1");
	string structure;
	e.GetPrettyTreeString(structure);
	string expected = "|__+\n    |__*\n    |   |__8\n    |   |__rounddown\n    |       |__2\n    |__1\n";
	if (structure != expected) {
		cout << "Got: \n" << structure << "Expected: \n" << expected;
		return false;
	}
	return true;
}

void RunTests() {
	PrintTestResults("Uneven Parentheses", TestUnevenParentheses());
	PrintTestResults("Cyclic Dependency", TestCyclicDependency());
	PrintTestResults("Expression Structure Test 1", TestExpressionStructure_TestOne());
	PrintTestResults("Expression Structure Test 2", TestExpressionStructure_TestTwo());
}


#endif