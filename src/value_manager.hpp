#ifndef VALUE_MANAGER_H
#define VALUE_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

// Not sure if the godot namespace is necessary, as
// this class will not interact with engine functionality
// directly
namespace godot {

/*
 * @brief
 * The value manager is a way to manage all of the
 * various complex states of an Ars Magica Magus.
 * The value manager is able to define value associations
 * between strings and the player's current value. For example,
 * a request for "Creo Exp" would return "21" for a Magus with
 * experience 21 in Creo. 
 * 
 * 
 * What makes this system powerful is the syntax that can be
 * used to define some value relative to an existing value.
 * For example, "Creo" would return "6", but if experience
 * changes to 28, it would return "7" with no extra work.
 * This is because the "Creo" value was defined as
 * "RoundDown((sqrt(8 * [Creo Exp] + 1) - 1) / 2)"
 * 
 * The following are the expressions and operations built-in
 * Expressions:
 *      + : Addition
 *      - : Subtraction
 *      * : Multiplication
 *      / : Division
 * 
 * Operations:
 *      sqrt(val)
 *      pow(base, exp)
 *      RoundDown(val)
 *      RoundUp(val)
 *      Round(val)
 *      Min(val)
 *      Max(val)
 * 
 * In addition to those above, conditionals can be defined
 * by using the ternary operator. Conditionals can be
 * difficult to manage, so try to use external state to
 * manage whether other values should be added. (The
 * ternary operation is more for players rather than to actually
 * be used in code). Conditions, if not provided, are assumed false.
*/
class ValueManager {
private:
    class ValueExpression {
        std::vector<ValueExpression*> children_;
        std::string value_;
    };

    std::unordered_map<std::string, ValueExpression*> expressions_;

    // Caching for efficiency
    class DependencyNode {

    };
    // Saves us from traversing expressions on every get_value call
    std::unordered_map<std::string, float> cache_; 
    // Roots are real values. When we update a real value, we have to update
    // every sub-dependency in the cache. Saves us from updating every
    // value expression that exists in expressions
    // Example Graph for Creo Exp:
    //
    //          Creo Exp
    //              |
    //             \/
    //            Creo
    //            / \ 
    //          /    \ 
    //         \/    \/
    //   Cr*LabTotal Cr*CastingScore
    std::vector<DependencyNode*> dependency_graph_; 

public:
    /*
     * Places the value of the given named expression in result.
     * If the return value is false, the named expression does not
     * exist in the ValueManager. Conditions are optional.
     * If the value-expression holds conditionals, they are given
     * the true/false value found in cond, given as "cond1=true,cond2=false".
     * If a condition is not given, it is assumed false.
    */
    bool get_value(std::string name, float& value, std::string cond = std::string(""));

    /*
     * Places the value of the given named expression into the
     * ValueManager with the value given. Returns false
     * if the value given was invalid syntax. If the value
     * already exists, modifies the value if it is a value.
     * If the value is an expression, the value will return false.
    */
    bool set_value(std::string name, float value);

    /*
     * Sets the expression of the given name to have
     * the new expression given. If the expression is
     * in invalid syntax, will not modify the
     * expression and the method will return false.
    */
    bool set_expression(std::string name, std::string expression);
};

}

#endif