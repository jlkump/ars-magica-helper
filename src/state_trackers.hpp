#ifndef ARS_STATE_H
#define ARS_STATE_H

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <sstream>
#include <iostream>
#include <unordered_set>

#include "error.hpp"
#include "parsing.hpp"

using namespace std;

class CharacterState;
class Expression;

// TODO:
// These might not need all be seperate classes, but I'm just outlining the various things I wish to track
// for the player
class GameState;
class Note;
class SettingDate;
class SettingDateFormat;
class SettingContext;
class SeasonManager;
class Event;
class ItemManager;
class Item; // Types such as Plain Item, Device, Lab text, Tractus, Summae
class LaboratoryManager; 
class Modifier;

/*
* My pseudo-code thoughts on what classes might look like

class Event {
	name,
	description,
	source, (optional)
	effect, (optional)
	setting_date,
	setting_location, (optional)
	setting_event_tag, (optional)
};

class SettingData {
	year,
	season,
	month,
	day,
	format,
}

class SettingDateFormat {
	months_in_season,
	season_in_year,
	month_num_to_name,
	season_num_to_name,
}


// Might just be a sub-set of a note or specific type of note
class SettingLocation {
	name,
	broad,
	specific,
}

class Note {
	name,
	description,
	tags, (optional) --may define type, such as GM_NOTE, which means players can't view, or PLAYER_SPECIFIC, in which only specific players can view
	references, (optional) --points to other notes this note may refer to.

	class Tag {
		data*,  --a pointer to any relevant data associated with the tag
		enum Type {
			GM_NOTE,
			PLAYER_SPECIFIC, --In this case, data points to a list of players who have access to the note
			CHARACTER_SPECIFIC, --In this case, data points to a list of characters who have access to the note
			INDIVIDUAL,
			WORLD_BUILDING,
			PLAYER_DEFINED, --In this case, data points to a string with the name of the tag
		}
	}

	class Reference {
		some section of the text is defined as a reference,
		name of what is the referenced note,
	}
}


*/

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

class Expression {
private:
	struct ExpressionNode {
		enum Type {
			TRUE_VALUE,  // True value is the case where the value is just a number in the expression.
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
			INVALID,
		};
		Type type_;
		string node_value_; // Union wasn't working out for me, just convert the string to the appropriate type when needed \_(._.)_/
		vector<ExpressionNode*> children_; // Might move this under the value union in the future.
	public:
		ExpressionNode(Type t) : type_(t) {};
		~ExpressionNode();
	};
	string name_;
	ExpressionNode* ast_root_; // Abstract Syntax Tree

	static bool IsValidStringOperation(const string& op);
	static bool IsValidCharOperation(const char op);
	static bool IsValidVariableName(const string& var);
	static int GetOperationPrecedence(const string& op);
	static Expression::ExpressionNode::Type GetOperationNodeType(const string& op);

	/*
	* This function checks that an expression string uses valid format and will throw a
	* SyntaxError if it does not. This will occur if the expression has:
	* 1. Operations in the invalid form "RoundDown 2.0", where parentheses are missing.
	* 2. The given operation does not exist, ex: "DoCoolStuff(29)"
	* 3. The operation is not supported, ex: "1 >> 2", essentially another form of the case above
	*/
	static void CheckValidExpressionOperations(const string& expression);

	/*
	* Given some expression, finds the root operation, i.e., the next operation
	* to place in the AST. That operation is placed in the operation string
	* given and the list of subexpressions (from left to right) is placed in the given
	* sub-expression list.
	*/
	static void FindRootOperation(const string& expression, string& operation, vector<string>& subexpressions);

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

	/*
	* Recursive evaluation helper for getting the value of the expression.
	*/
	float GetValueRecursiveHelper(const ExpressionNode* cur, const GameState& game_state, const CharacterState& character_state) const;
	// Debugging helper
	static void GetPrettyTreeRecursiveHelper(string& result, const string& prefix, const ExpressionNode* node, bool end);

	void GetStateValuesRecursiveHelper(const ExpressionNode* cur, vector<string>& result) const;

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
	* Simple getter for the expression's name
	*/
	inline const string& GetName() const { return name_; }

	/*
	* This will parse the given expression into the appropriate Abstract Syntax Tree for
	* the expression to keep track of. This must be called at least once with valid
	* syntax for the expression to be valid.
	* 
	* If the given expression string is not valid, the method will throw an exception.
	*/
	void SetExpression(const string& expression);

	/*
	* Modifies the given list, adding the state-values this expression contains.
	* This is the list of all values this expression needs to get to evaluate.
	* For example, Creo has a state value of CreoExp.
	*/
	void GetStateValues(vector<string>& state_value_names) const;

	/*
	* A simple debugging helper to see the structure of the expression
	*/
	void PrintTreePretty() const;
	void GetPrettyTreeString(string& result) const;
};



////////////////////////////////////////////
//             CharacterState             //
////////////////////////////////////////////

/*
* This class tracks all things related to character state in the tabletop rpg Ars Magica
* (However, this system is extensible to build ontop of existing game rules)
* 
* CharacterState contains notes, values, season timeline of events, 
* laboratory and inventory management.
* 
* Note how values are refered to in this code base:
*	  - base value: a base value is some value such as 
*		experience, which is defined as a single value, but may change over the course of the game.
* 
*	  - expression value: an expression value refers to other values within the 
*		character and possibly the game state as well. It can perform any arbitrary
*		operations on these values, but can be evaluated to a single value given
*		all necessary values it relys on for computation.
* 
*	  - true value: In the context of an expression, a true value will
*		remain the same no matter the situation of the game state or character state.
*		This value is entirely static and hard-coded into the expression itself.
* 
*	  - state value: This value is not known by the expression and is referenced somewhere
*		in either the character state or game state. As such, base values and expression
*		values are also state values.
* 
*	  - string value: these should be thought of as simple one-off strings that
*		represent values on the character sheet of the player, such as the name
*		of the magus, the tradition, or the reputations. These do not represent more complex
*		string values such as items the player has.
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
	unordered_map<string, Expression*> expression_vals_;
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
	//             Creo Exp
	//                |
	//               \ /
	//              Creo
	//            /       \ 
	//           /         \ 
	//          \/         \/
	//   Cr*LabTotal   Cr*CastingScore

	struct DependencyNode {
		string name_;
		vector<string> children_;
	public:
		DependencyNode(const string& name) : name_(name) {}
		~DependencyNode() {}
	};
	unordered_map<string, DependencyNode*> dependency_graphs_;

	const GameState& game_state_;
	const SettingContext* setting_context_;
	SettingDate birth_year_;
	SettingDate currrent_year_;

	/*
	* DoesDependencyContainCycle and its recursive helper ensure that when an expression is
	* updated using SetExpression(), that the expression being created for the character
	* doesn't have some circular dependency. If it did, the stack would overflow on evaluation,
	* thus we throw an exception if we do find a cycle.
	* 
	* UpdateDependencyGraph will toss this exception if there is a cycle in the provided expression.
	* If there is not any exception, then the dependency graph is updated accordinly as shown in sections
	* above.
	*/
	bool DoesDependencyContainCycleRecursiveHelper(const DependencyNode* cur, unordered_set<string>& visited, unordered_set<string>& on_stack, const unordered_map<string, DependencyNode*>& graph) const;
	bool DoesDependencyContainCycle(const DependencyNode* current, const unordered_map<string, DependencyNode*> graph) const;
	void UpdateDependencyGraph(const Expression& expression);

	/*
	* Updates the internal cached_expression_vals_ map
	* The updated_value param is the name of the value being updated
	* and the new_value is the expression's new updated value. This is called
	* recursively on each dependency defined in the dependency graph to
	* make sure each expression value is updated accordingly.
	*/
	void UpdateCache(const string& updated_value, const float new_value);

	/*
	* These are simple helper debug methods to visually display the
	* dependency graph and check if the structure of the tree is correct.
	*/
	void PrintDependencyGraphPretty(const string& value_name) const;
	void GetPrettyTreeRecursiveHelper(string& result, const string& prefix, const DependencyNode* node, bool end) const;
	void GetPrettyTreeString(string& result, const string& value_name) const;

public:
	CharacterState(const GameState& game_state) : game_state_(game_state), setting_context_(game_state_.GetSettingContextForCharacter(*this)) {}
	~CharacterState();

	/*
	 * Given a value_name for some state value of the character, if
	 * the state value exists within character state, then the
	 * given float is modified to contain that value and the method
	 * returns true.
	 * 
	 * If the value does not exist in the character state, this
	 * method returns false and does not modify result.
	 */
	bool GetValue(const string& value_name, float& result) const;

	/*
	* Given a value_name for a string value, if the value exists within
	* the character state, then result is modified to contain the
	* associated string and the method returns true.
	* 
	* If the value does not exist in the character state, this method
	* returns false and does not modify result.
	*/
	bool GetStringValue(const string& value_name, string& result) const;

	/*
	* SetValue() will set the value within CharacterState with the given value_name to the given value.
	* If a value does not exist within the CharacterState on calling, then it is created.
	* If a value exists, but is not modifable b/c it is instead an expression, an exception will be thrown.
	* If a value is modified from its previous value, all values depending upon that value in the character
	* state will be updated. If there is some value outside the CharacterState which needs to be updated
	* when character state changes, it should register a callback with the method `AddCallbackOnValueChange`
	*/
	void SetValue(const string& value_name, const float value);
	void SetStringValue(const string& value_name, const string& value);

	/*
	* Will set the expression within the CharacterState to the given expression assuming the
	* syntax of the expression is valid and does not form a circular dependency of values.
	* If the expression is in invalid syntax or forms a circular dependency, then a exception is thrown.
	* 
	* An exception is also thrown if the expression can not be evaluated at the time it is set. This
	* can safely be ignored if future SetValues or SetExpressions will ensure that the state value
	* referenced in the expression is set. Regardless, it is better to set base values first, then
	* create expressions based on those values afterwards.
	* 
	* Future calls to GetValue() will return the evaulated value based on the expression set here.
	* In this way, expressions are treated the same as values when retrieving data, but are different
	* when setting it.
	*/
	void SetExpression(const string& value_name, const string& expression);

	/*
	* If the removed value was an expression, old expression will point to the expression that represents it.
	* If the removed value was a base value, then the value of old_expression will be null.
	* If the value name does not exist, then the method returns false
	* This will also remove all dependent expressions of the removed value, which are stored in the passed in vector
	*/
	bool RemoveValue(const string& value_name, Expression*& old_expression, vector<Expression*>& dependent_expressions);

	bool IsBaseValue(const string& value_name);
	bool IsExpressionValue(const string& value_name);

	/*
	* AddCallbackOnValueChange will add the given callback function to the CharacterState so that
	* when SetValue is called setting value_name, this callback function will be called if the names match.
	* If they do not match, the callback function will not be called. 
	* 
	* Callback function will be passed the updated value.
	* 
	* Note: if many callbacks are added to a single value, this will decrease the performance of updating 
	* that value with SetValue(); Also if the callbacks are expensive, this will also increase the cost
	* of updating the specific value with SetValue().
	*/
	void AddCallbackOnValueChange(const string& value_name, void(*callback_function)(const float));

	/*
	* This prints ALL information of the character state.
	*	TODO: Maybe add verbosity param to determine how much is actually printed if there comes to be
	*			lots of information in a character state.
	*/
	void DebugPrintInfo() const;
};

/*		The game state will hold many SettingContexts,
* which define the setting data, such as the current year,
* what resources are available to buy easily, the format
* for how to interpret a given date, the various nearby locations,
* known npcs, etc.
*		Each CharacterState has a setting context that it uses
* to evaluate certain expressions. There is a default setting
* context that defines universally shared setting details.
*			GameState
*				|
*				|
*			   \/
*				+
*		  SettingContext	--->+	CharacterState
*/

////////////////////////////////////////////
//             GameState                  //
////////////////////////////////////////////
/*
* The GameState will act essentially as the GM's handle into the game world,
* defining the necessary functionality of the setting and world. For example,
* the GameState defines the SettingDateFormat. The GameState may have multiple
* SettingContexts (for example, in my game world, Sahket doesn't have a 
* traditional spring, winter, fall, summer, but a dry,wet, etc.) Being able to define
* a setting context for the players will be extremely useful in defining what is available
* to the players and how they view the information.
* 
* It may also be that a setting context changes over time based on events. Similar to
* how the CharacterState has SeasonManager for handling the seasonal progression for 
* the player with a timeline of events and their results, the GameState will have
* a SettingManager, which will handle various settings in the world for the GameState,
* with events that can be defined to happen in some timeline (Note: This timeline MUST
* be editable duing play, otherwise the GM will encounter problems when players 
* don't experience the event if they are not in the event context).
* 
* SettingEvent will be very similar to a normal event, but the effect will modify
* SettingContexts rather than CharacterStates. Setting events also have unique tags
* such as: 
*		LOCAL_EVENT			--Only those within the SettingContext can know about it, and only in specific locations
*		SETTING_EVENT		--Only those within the SettingContext can know about the event
*		GLOBAL_EVENT		--Anyone can possibly know about the event
*		SPECIFIC_CHARACTERS --Only specific characters are aware of the event
*		ALL_CHARACTERS		--All characters are aware of the event
*/
class GameState {
private:
	unordered_map<string, float> state_;
	// These define events that are universal to all setting contexts
	unordered_map<SettingDate, Event> defined_events_;
protected:
public:
	bool GetValue(const string& value_name, float& result) const;

	const SettingContext* GetDefaultSettingContext() const;

	/*
	* Returns default setting context if the character has no specific context
	*/
	const SettingContext* GetSettingContextForCharacter(CharacterState& c) const;

	const SettingDate GetCurrrentYear(CharacterState& c) const;
	const SettingDate GetMaximumYear(CharacterState& c) const;
};

class SettingContext {
private:
	GameState& parent_;
	SettingDateFormat date_format_;
	// These define all the events for the setting context
	unordered_map<SettingDate, Event> defined_events_; 

public:
	SettingContext(GameState& game_state) : parent_(game_state) {}

	bool GetValue(const string& value_name, float& result) const;


};

/*
* This defines the date for a setting.
* Hard limits:
*	- There are only 4 seasons in a year, evenly spaced across the months
*   - SettingDates are ordered by their year and month
*/
class SettingDate {
	friend bool operator==(const SettingDate& lhs, const SettingDate& rhs) {
		return lhs.year_ == rhs.year_ && lhs.month_ == rhs.month_ && lhs.day_ == rhs.day_;
	}
	friend bool operator!=(const SettingDate& lhs, const SettingDate& rhs) {
		return !(lhs == rhs);
	}
	friend bool operator>(const SettingDate& lhs, const SettingDate& rhs) {
		if (lhs.year_ > rhs.year_) {
			return true;
		} else if (lhs.year_ < rhs.year_) {
			return false;
		}
		if (lhs.month_ > rhs.month_) {
			return true;
		} else if (lhs.month_ < rhs.month_) {
			return false;
		}
		return lhs.day_ > rhs.day_;
	}
	friend bool operator<(const SettingDate& lhs, const SettingDate& rhs) {
		if (lhs.year_ < rhs.year_) {
			return true;
		}
		else if (lhs.year_ > rhs.year_) {
			return false;
		}
		if (lhs.month_ < rhs.month_) {
			return true;
		}
		else if (lhs.month_ > rhs.month_) {
			return false;
		}
		return lhs.day_ < rhs.day_;
	}
	friend bool operator>=(const SettingDate& lhs, const SettingDate& rhs) {
		return lhs > rhs || lhs == rhs;
	}
	friend bool operator<=(const SettingDate& lhs, const SettingDate& rhs) {
		return lhs < rhs || lhs == rhs;
	}

public:
	enum Season {
		FIRST,
		SECOND,
		THIRD,
		FOURTH,
	};
private:
	int year_;
	Season season_;
	int month_;
	int day_;
public:
};

// Given a setting date, converts between:
// - season enum and season name
// - month number and month name
// - ordered in a nice string display
class SettingDateFormat {
private:
	unordered_map<int, int> days_in_month_;
	unordered_map<SettingDate::Season, string> season_name_;
	unordered_map<int, string> month_name_;
	unordered_map<SettingDate, string> unique_day_names_;

public:
	void DefineDaysInMonth(int month, int num_days);
	void DefineMonthsInYear(int num_months);
	void DefineSeasonName(SettingDate::Season season, const string& name);
	void DefineMonthName(int month, const string& name);
	void DefineUniqueDayName(const SettingDate& date, const string& name);
	/*
	* Format is defined by a string in the following syntax:
	*		- %y  :  defines the year	(printed as an int)
	*		- %S  :  defines the season (printed as a string)
	*		- %M  :  defines the month  (printed as a string)
	*		- %m  :  defines the month  (printed as an int)
	*		- %d  :  defines the day	(printed as an int)
	*		- %D  :  defines the day    (printed as a string if available, int otherwise)
	*/
	void DefineDateFormat(const string& format);

	/*
	* Returns the name of the season for the given setting date
	*/
	string GetSeasonName(const SettingDate& s) const;

	/*
	* Returns the name of the month for the given setting date. If the
	* month is outside the range for this format, returns an empty string.
	*/
	string GetMonthName(const SettingDate& s) const;

	/*
	* Returns the unique name for the day of a given date. If there is no
	* unique name for the day, then an empty string is returned.
	* (Tracks things like local holidays and such).
	*/
	string GetUniqueDayName(const SettingDate& s) const;

	/*
	* Returns the full string for the date in the appropriate format
	* defined for this setting. Defined by DefineDateFormat()
	*/
	string GetFullDateString(const SettingDate& s) const;

	/*
	* Returns the number of days within a given month. Returns -1
	* if the month is not defined in this setting format.
	*/
	int GetDayCountForMonth(const SettingDate& s) const;

	/*
	* Returns the number of months for a year. Returns -1 if
	* the value has not yet been defined.
	*/
	int GetMonthCountForYear(const SettingDate& s) const;
};


////////////////////////////////////////////
//             SeasonManager              //
////////////////////////////////////////////
/*
* This class manages a given CharacterState, performing actions on that
* character in a format of per-season events. These events are performed
* within some SettingContext that is associated with the character.
* 
* An event is performed at a given SettingDate which defines what point
* the event happened to the character.
* 
* The SeasonManager has a set of all events that happen to a character.
* These events are ordered in a timeline within the SeasonManager and
* the ownering CharacterState can be changed to various different points
* in time by setting the active date. Events can be created at any
* point in the timeline and the CharacterState will be updated accordingly.
* 
* Every SettingDate can be ordered according to it's year, season, month,
* and day. The way a SettingDate is displayed is defined by the SettingContext,
* which holds a SettingDateFormat specifier.
* 
* The limit to how far forward the player can go is defined by the GameState
* as an upper-bound limit. The player's birth year is also the lower-bound
* on how far back an event can be created.
*/

class CharacterEvent {
public:
	// Given some character, modifies values for the character
	void ApplyEvent(CharacterState& c) const;
};

/*
* Effects on the character from character effect may add, subtract, set, or remove
* base values of the character.
* TODO: Need a separate way of adding / removing character expressions in a similar format
*/
class CharacterEffect {
public:
	enum Operation {
		ADD,
		SUBTRACT,
		SET,
		REMOVE,
	};
private:
	Operation op_;
	string value_name_;
	string modify_value_; // A simple value to be added to the 
	string previous_value_; // value of NaN when previous value didn't exist
	bool is_expression_;
	vector<Expression*> dependent_expressions_; // When we remove a value, there may exist some dependencies, so keep track of them so we can undo.
public:
	/*
	* Throws exceptions if the modify value is not in the right format for the corresponding operation.
	*/
	CharacterEffect(const string& value_to_effect, Operation op, string modify_value);

	void ApplyEffect(CharacterState& c);
	void UndoEffect(CharacterState& c);
};

class SeasonAction {
private:
	string name_of_action_;
	string desciption_of_action_;
	CharacterEffect effect_of_activity_;
	vector<string> action_tags_; // Used for organization and sorting
};

class SeasonActivity {
private:
	SettingDate date_of_activity_;
	SettingContext context_of_activity_;
	SeasonAction action_;
};


class SeasonManager {
private:
	CharacterState& owning_character_;
	SettingDate viewing_year_;
public:
	/*
	* These following methods change the true events and
	* actual current year for the player.
	*/
	void AdvanceByDay();
	void AddDailyEvent();
	void RemoveDailyEvent();

	void AdvanceByMonth();
	void AddMonthlyEvent(); // Usefull for tracking wounds and such
	void RemoveMonthlyEvent();

	void AdvanceBySeason();
	void AddSeasonalEvent();
	void RemoveSeasonalEvent();

	void AdvanceByYear();
	void AddYearlyEvent(const SettingDate& date_of_occurance, const CharacterEvent& ev);
	void RemoveYearlyEvent();

	/*
	* These are ways to view the future or past time, up to the limit
	* defined by the character's GameState. This temporarily
	* modifies the character's data, but all changes are tracked by
	* the season manager.
	*/
	void ViewCharacterAtTime(const SettingDate& date) const;

	const vector<const SeasonActivity&> GetActivitiesDuringTime(const SettingDate& date) const;

	/*
	* This allows the player to modify a past seasonal activity
	* The two activities must occur within the same season.
	* The Character's GameState has a limit on how far back
	* the player can modify previous activities, but this can
	* be overriden with the ForceModifyPastActivity.
	*/
	void ModifyPastActivity(const SeasonActivity& old_activity, const SeasonActivity& new_activity);
	/*
	* Overrides the GameState limitation of the ModifyPastActivity
	*/
	void ForceModifyPastActivity(const SeasonActivity& old_activity, const SeasonActivity& new_activity);

	/*
	* Note: does not actually remove the activity object, just makes it an empty / no-op
	* activity. This way modify past activity will still work on a removed activity.
	* Similar to ModifyPastActivity, RemovePastActivity will only work if the
	* 
	* GameState allows for it. This can be overriden with ForceRemovePastActivity
	*/
	void RemovePastActivity(const SeasonActivity& old_activity);
	void ForceRemovePastActivity(const SeasonActivity& old_activity);
};

#endif