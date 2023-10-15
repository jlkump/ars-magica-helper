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

// Main code
int main(int, char**)
{
	Expression e = Expression("temp expression");
	string line;
	string exit = string("exit");
	do {
		getline(cin, line);
		if (line != exit) {
			try {
				e.SetExpression(line);
			} catch (SyntaxError& e) {
				fprintf(stderr, "Got syntax error: \n%s", e.what());
			}
		}
	} while (line != exit);
}