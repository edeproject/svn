#include <edelib/Debug.h>

#include "MenuRules.h"
#include "DesktopEntry.h"

typedef list<bool> Stack;
typedef list<bool>::iterator StackIt;

MenuRules *menu_rules_new(void) {
	MenuRules *r = new MenuRules;
	r->rule_operator = MENU_RULES_OPERATOR_NONE;
	return r;
}

MenuRules *menu_rules_append_rule(MenuRulesList &rules, short rule, const char *data) {
	MenuRules *r = menu_rules_new();
	r->rule_operator = rule;

	if(data)
		r->data = data;

	rules.push_back(r);
	return r;
}

void menu_rules_delete(MenuRules *r) {
	if(!r->subrules.empty()) {
		MenuRulesListIt it = r->subrules.begin(), it_end = r->subrules.end();

		while(it != it_end) {
			menu_rules_delete(*it);
			it = r->subrules.erase(it);
		}
	}

	delete r;
}

/* 
 * Rules are evaluated starting from the deepest one, and are much more like
 * lisp expressions. Each rule behaves like function, returning false or true and top function
 * gets applied on those results.
 */
static bool eval_with_stack(MenuRules *m, DesktopEntry *en, Stack &result_stack) {
	if(!m->subrules.empty()) {
		MenuRulesListIt it = m->subrules.begin(), it_end = m->subrules.end();

		/* go to the deepest node */
		for(; it != it_end; ++it)
			eval_with_stack(*it, en, result_stack);
	}

	/* this will always evaluate to true so all items can be included */
	if(m->rule_operator == MENU_RULES_OPERATOR_ALL) {
		result_stack.push_front(true);
		return true;
	}

	if(m->rule_operator == MENU_RULES_OPERATOR_NOT) {
		result_stack.push_front(false);
		return false;
	}

	if(m->rule_operator == MENU_RULES_OPERATOR_FILENAME) {
		bool st = (m->data == en->get_id());
		result_stack.push_front(st);
		return st;
	}

	if(m->rule_operator == MENU_RULES_OPERATOR_CATEGORY) {
		bool st = en->in_category(m->data.c_str());
		result_stack.push_front(st);
		return st;
	}

	if(m->rule_operator == MENU_RULES_OPERATOR_AND) {
		StackIt it = result_stack.begin(), it_end = result_stack.end();

		/* 'and' always evalutes to true */
		bool st = true;

		while(it != it_end) {
			st &= *it;
			it = result_stack.erase(it);
		}

		result_stack.push_front(st);
		return st;
	}

	if(m->rule_operator == MENU_RULES_OPERATOR_OR) {
		StackIt it = result_stack.begin(), it_end = result_stack.end();

		/* 'or' always evalutes to false*/
		bool st = false;

		while(it != it_end) {
			st |= *it;
			it = result_stack.erase(it);
		}

		result_stack.push_front(st);
		return st;
	}

	/* for everything else */
	return false;
}

bool menu_rules_eval(MenuRules *m, DesktopEntry *en) {
	Stack result_stack;
	return eval_with_stack(m, en, result_stack);
}
