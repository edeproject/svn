#include <string.h>
#include <stdlib.h>

#include "tinyscheme/scheme-private.h"
#include "tinyscheme/scheme.h"
#include "string.h"

/* (string-split "string" "separator") */
static pointer s_split(scheme* sc, pointer args) {
	pointer lst_ret = sc->NIL;

	if(args == sc->NIL)
		return lst_ret;

	const char *str, *sep;
	char *p, *str2;

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return lst_ret;
	str = sc->vptr->string_value(a);

	args = sc->vptr->pair_cdr(args);
	a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_string(a))
		return lst_ret;
	sep = sc->vptr->string_value(a);

	str2 = strdup(str);
	for(p = strtok(str2, sep); p; p = strtok(NULL, sep))
		lst_ret = cons(sc, mk_string(sc, p), lst_ret);

	free(str2);

	lst_ret = scheme_reverse(sc, lst_ret);
	return lst_ret;
}

void register_string_functions(scheme* sc) {
	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "string-split"),
		sc->vptr->mk_foreign_func(sc, s_split));
}
