// GUI Imports
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <windows.h>
#include <GL/GL.h>
#include <tchar.h>

// GLM (Math Library)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project Imports
#include <helpers/RootDir.h>

#include "state_trackers.hpp"

#include <iostream>

using namespace std;

bool contains_equals(const string& s) {
	for (const char c : s) {
		if (c == '=') {
			return true;
		}
	}
	return false;
}

void parse_expression(const string& s, string& name, string& expression) {
	auto i = s.begin();
	while (i != s.end() && *i != '=') {
		i++;
	}
	name = string(s.begin(), i);
	expression = string(i + 1, s.end());
}

bool IsValue(string& s) {
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

// Main code
int main(int, char**)
{
	string line;
	string exit = string("exit");
	CharacterState c = CharacterState();
	do {
		getline(cin, line);
		if (line != exit) {
			try {
				// This to-lowercase-no-whitespace transformation should probably be done before the expression is passed here.
				string lowercase_nowhitespace = line;
				transform(line.begin(), line.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
				lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
				if (contains_equals(lowercase_nowhitespace)) {
					string name;
					string expression;
					parse_expression(lowercase_nowhitespace, name, expression);
					if (IsValue(expression)) {
						c.SetValue(name, stod(expression));
					} else {
						c.SetExpression(name, expression);
					}
					printf("Setting value %s with expression %s\n", name.c_str(), expression.c_str());
				}
				else {
					float value;
					c.GetValue(GameState(), lowercase_nowhitespace, value);
					printf("Got value for %s as %f\n", line.c_str(), value);
				}
			} catch (SyntaxError& e) {
				fprintf(stderr, "Got syntax error: \n%s", e.what());
			}
			catch (EvaluationError& e) {
				fprintf(stderr, "Got evaluation error: \n%s", e.what());
			}
		}
	} while (line != exit);
}