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

#include <iostream>

// Project Imports
#include <helpers/RootDir.h>
#include "state_trackers.hpp"

//#define DEBUG_TEST

#ifdef DEBUG_TEST
#include "tests.h"
#endif

using namespace std;

// Main code
int main(int, char**)
{
#ifdef DEBUG_TEST
	RunTests();
	getc(stdin);
#endif

	string line;
	string exit = string("exit");
	GameState g = GameState();
	CharacterState c = CharacterState(g);
	CreateCharacterFromPlainTextFile("C:\\Users\\lando\\OneDrive\\Documents\\code_projects\\Ars-Magica-Helper\\data\\test-characters\\simple.txt", c);
	do {
		getline(cin, line);
		if (line != exit) {
			try {
				// This to-lowercase-no-whitespace transformation should probably be done before the expression is passed here.
				string lowercase_nowhitespace = line;
				transform(line.begin(), line.end(), lowercase_nowhitespace.begin(), [](unsigned char c) { return std::tolower(c); });
				lowercase_nowhitespace.erase(remove_if(lowercase_nowhitespace.begin(), lowercase_nowhitespace.end(), isspace), lowercase_nowhitespace.end());
				if (line == "debug") {
					c.DebugPrintInfo();
				}
				else if (ContainsEqual(lowercase_nowhitespace)) {
					string name;
					string expression;
					ParseExpression(lowercase_nowhitespace, name, expression);
					if (IsTrueValue(expression)) {
						c.SetValue(name, stod(expression));
					}
					else {
						c.SetExpression(name, expression);
					}
					//printf("Setting value %s with expression %s\n", name.c_str(), expression.c_str());
				}
				else {
					float value;
					c.GetValue(lowercase_nowhitespace, value);
					printf("Got value for %s as %f\n", line.c_str(), value);
				}
			}
			catch (SyntaxError& e) {
				fprintf(stderr, "Got syntax error: \n%s", e.what());
			}
			catch (EvaluationError& e) {
				fprintf(stderr, "Got evaluation error: \n%s", e.what());
			}
		}
	} while (line != exit);
}