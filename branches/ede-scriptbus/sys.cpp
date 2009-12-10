#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <edelib/Missing.h>
#include <edelib/Run.h>

#include "tinyscheme/scheme-private.h"
#include "tinyscheme/scheme.h"
#include "sys.h"

EDELIB_NS_USING(run_sync)

extern char** environ;

/*
 * (getenv <what>) => <string>
 * returns environment value for <what>; if <what> is not given, returns a list of all environment key/value pairs
 */
static pointer s_getenv(scheme* sc, pointer arg) {
	if(arg == sc->NIL) {
		char** env = environ;
		pointer lst = sc->NIL;

		while(*env) {
			lst = cons(sc, mk_string(sc, *env), lst);
			env++;
		}

		return scheme_reverse(sc, lst);
	}
	
	pointer a = sc->vptr->pair_car(arg);
	if(a != sc->NIL && sc->vptr->is_string(a)) {
		const char* val;
		if((val = getenv(sc->vptr->string_value(a))) != NULL)
			return mk_string(sc, val);
	}

	return sc->F;
}

/* (setenv var value) */
static pointer s_setenv(scheme* sc, pointer args) {
	if(args == sc->NIL)
		return sc->F;

	const char* key, *val;
	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	key = sc->vptr->string_value(a);

	args = sc->vptr->pair_cdr(args);
	a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return sc->F;
	val = sc->vptr->string_value(a);

	if(edelib_setenv(key, val, 1) == 0)
		return sc->T;
	return sc->F;
}

/* (clock) */
static pointer s_clock(scheme* sc, pointer args) {
	return mk_real(sc, (double)clock());
}

static int seed_inited = 0;

/* (random [optional-max]) */
static pointer s_random(scheme* sc, pointer args) {
	if(!seed_inited) {
		srand(time(0));
		seed_inited = 1;
	}

	if(args == sc->NIL)
		return mk_integer(sc, rand());

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_integer(a)) {
		/* TODO: this should be an error */
		return sc->F;
	}

	long v = sc->vptr->ivalue(a);
	return mk_integer(sc, rand() % v);
}

/* to speed up some things */
static pointer s_fast_foreach(scheme *sc, pointer args) {
	if(args == sc->NIL)
		return sc->F;

	pointer proc, lst, item;

	proc = sc->vptr->pair_car(args);
	if(proc == sc->NIL || !sc->vptr->is_closure(proc))
		return sc->F;

	args = sc->vptr->pair_cdr(args);
	lst = sc->vptr->pair_car(args);
	if(lst == sc->NIL || !sc->vptr->is_pair(lst))
		return sc->F;

	while(1) {
		item = sc->vptr->pair_car(lst);
		if(item == sc->NIL)
			break;

		/* apply function */
		scheme_call(sc, proc, _cons(sc, item, sc->NIL, 0));
		lst = sc->vptr->pair_cdr(lst);
	}

	return sc->T;
}

/* (exec cmd) */
static pointer s_exec(scheme* sc, pointer args) {
	if(args == sc->NIL)
		return sc->F;

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a)) {
		/* TODO: this should be an error */
		return sc->F;
	}

	const char *cmd = sc->vptr->string_value(a);
	return mk_integer(sc, run_sync(cmd));
}

void register_sys_functions(scheme* sc) {
	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "getenv"),
		sc->vptr->mk_foreign_func(sc, s_getenv));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "setenv"),
		sc->vptr->mk_foreign_func(sc, s_setenv));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "clock"),
		sc->vptr->mk_foreign_func(sc, s_clock));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "random"),
		sc->vptr->mk_foreign_func(sc, s_random));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "fast-for-each"),
		sc->vptr->mk_foreign_func(sc, s_fast_foreach));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "exec"),
		sc->vptr->mk_foreign_func(sc, s_exec));
}
