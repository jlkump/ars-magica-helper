#ifndef ARS_STATE_H
#define ARS_STATE_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


#define GET_VALUE_DECLARATION friend float GetValue(const State& state, const ValueManager& value_manager, const string& value_name)


#define SET_VALUE_DECLARATION 

/*
 *		This file will be used to define all the structs and classes necessary to
 * define the state of the character and of the game. 
 * 
 *		As a reminder, the goal of the GUI helper is to allow for the user 
 * to easily see all relevant values and have them updated when other values change. 
 * The user should also be able to define their own equations that are based on the 
 * values of the game or character state.
 * 
 *		To facilitate this, we divide the state into Expressions and Values. Values
 * are simply some pair of string to float value. For example, "Creo Exp" is a value
 * name with some value, lets say 7.0. Expressions would similarly be a pair of
 * strings to float values, however, their value depends upon the state of the game
 * and of the character's other values. For example, a request for "Creo" would
 * return 3.0 at the "Creo Exp" value of 7.0, but return 4.0 when "Creo Exp" was
 * updated to 10.0.
 */

class GameState {
private:
protected:
public:
	unordered_map<string, float> state_;
};

class Expression {
private:
	struct ExpressionNode {
		enum NodeType {
			TRUE_VALUE, // True value is the case where the value is just a number in the expression.
			STATE_VALUE, // State value is the case where the value is stored in some state, whether it be character or game state.
			ADDITION,
			SUBTRACTION,
			MULTIPLICATION,
			DIVISION,
			SQRT,
			POW,
			ROUND_DOWN,
			ROUND_UP,
			ROUND,
			MIN,
			MAX,
			TERNERY,
		};
		union Value {
			float true_value_;
			string state_value_;
			bool bool_value_;
		};
		Value node_value_;
	};
	string name_;
	ExpressionNode* ast_root_; // Abstract Syntax Tree

	/*
	* This will actually parse the expression string given. The current syntax I have in
	* mind looks something like the following:
	* 
	* RoundDown((sqrt(8 * [Creo Exp] + 1) - 1) / 2)
	* where the value in brackets, in this case [Creo Exp], is some other value which would be
	* stored as a STATE_VALUE in the AST. It is important to note that all white-space and
	* capitalization of the value_names will be set to lowercase with no whitespace. This will
	* make it easier to compare and use for the player. Thus, CREO EXP would be the same as creoexp.
	*/
	static void ParseAndBuildExpressionTree(const string& expression, ExpressionNode*& result);
protected:
public:
	Expression(const string& name) : name_(name), ast_root_(nullptr) {}

	/*
	* Returns the evaluated expression based on the given state. This operation is
	* O(N), where N is the number of operations plus the number of values in the expression.
	* 
	* An exception is thrown when the Expression is in invalid state (when SetExpression has not been called) 
	* or when there is a value that doesn't exist in the given states.
	*/
	float GetValue(const GameState& game_state, const CharacterState& character_state) const;

	/*
	* This will parse the given expression into the appropriate Abstract Syntax Tree for
	* the expression to keep track of. This must be called at least once with valid
	* syntax for the expression to be valid.
	* 
	* If the given expression string is not valid, the method will throw an exception.
	*/
	void SetExpression(const string& expression);
};

////////////////////////////////////////////
///////////// CharacterState ///////////////
////////////////////////////////////////////

/*
* This class will track all things related to character state, including the database
* of string -> value pairs that exist in tabletop RPGs like Ars Magica. Specifically,
* this class will store:
*  - Strings in the string_vals_ variable (Stores notes, descriptions, etc.)
*  - Values in the base_vals_ and expression_vals_ variables (Arts, Abilities, Exp, Seasonal progress points, etc.)
* 
* Whenever a value is updated, a callback for that value being updated can be set with the
* AddCallbackOnValueChanged() method, which is how all external state that depends upon
* character state should be updated.
*/
class CharacterState {
private:
	unordered_map<string, string> string_vals_;

	// Flat values of string to floats.
	unordered_map<string, float> base_vals_;

	// Maps a given value_name to the associated AST expression
	unordered_map<string, Expression> expression_vals_;
	unordered_map<string, float> cached_expression_vals_; // Cached for efficiency, updated by dependency graph

	// Maps a value to the list of callback functions it should call when the value is updated.
	unordered_map<string, vector<void(*)(const float)>> callbacks_;

	// Maps a given value_name to a dependency graph. Note, the pointer may point to a sub-graph of an
	// existing graph. This is because an expression may be updated as well as a base_value, so
	// we need to be prepared for both cases. This way we can enter the graph at any location
	// and update only the values we need to. The values to be updated are children of the
	// dependency node and all sub-children. It is assumed that every graph or sub-graph is
	// a DAG (Directed Acyclic Graph).
	// Example Graph for Creo Exp:
	//
	//            Creo Exp
	//               |
	//               \/
	//              Creo
	//            /       \ 
	//          /          \ 
	//         \/          \/
	//   Cr*LabTotal   Cr*CastingScore
	struct DependencyNode {
		string name;
		vector<DependencyNode*> children;
	};
	unordered_map<string, DependencyNode*> dependency_graphs_;
public:
	/*
	 * Returns the value of the given value_name which may depend upon the
	 * current GameState. If the given value is not contained within the
	 * CharacterState, an exception is thrown. An exception is also thrown
	 * if the requested value is actually a string value.
	 */
	float GetValue(const GameState& game_state, const string& value_name) const;

	/*
	* Returns the value of the given value_name. There is no dependency on state,
	* simply tracks the character's strings for notes, descriptions, etc.
	* 
	* If a requested value_name does not exist or does not have the value of string,
	* an exception is thrown.
	*/
	const string& GetStringValue(const string& value_name) const;

	/*
	* SetValue() will set the value within CharacterState with the given value_name to the given value.
	* If a value does not exist within the CharacterState on calling, then it is created.
	* If a value exists, but is not modifable b/c it is instead an expression, an exception will be thrown.
	* If a value is modified from its previous value, all values depending upon that value in the character
	* state will be updated. If there is some value outside the CharacterState which needs to be updated
	* when character state changes, it should register a callback with the method `AddCallbackOnValueChange`
	*/
	void SetValue(const string& value_name, const float value);
	void SetValue(const string& value_name, const string& value);

	/*
	* Will set the expression within the CharacterState to the given expression assuming the
	* syntax of the expression is valid and does not form a circular dependency of values.
	* If the expression is in invalid syntax or forms a circular dependency, then a exception is thrown.
	* 
	* Future calls to GetValue() will return the evaulated value based on the expression set here.
	* In this way, expressions are treated the same as values when retrieving data, but are different
	* when setting it.
	*/
	void SetExpression(const string& value_name, const string& expression);

	/*
	* AddCallbackOnValueChange will add the given callback function to the CharacterState so that
	* when SetValue is called setting value_name, this callback function will be called if the names match.
	* If they do not match, the callback function will not be called. 
	* 
	* Callback function will be passed the updated value.
	* 
	* Note: if many callbacks are added to a single value, this will decrease the performance of updating 
	* that value with SetValue();
	*/
	void AddCallbackOnValueChange(const string& value_name, void(*callback_function)(const float));
};




#endif