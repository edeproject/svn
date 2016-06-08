#include <stdlib.h>
#include <edelib/Netwm.h>
#include <FL/x.H>

#include "tinyscheme/scheme-private.h"
#include "tinyscheme/scheme.h"
#include "wm.h"

EDELIB_NS_USING(netwm_window_get_all_mapped)
EDELIB_NS_USING(netwm_window_get_title)

/* (wm-get-window-list) */
static pointer s_wm_get_window_list(scheme *sc, pointer args) {
	fl_open_display();

	Window  *wins;
	pointer lst = sc->NIL;
	int sz = netwm_window_get_all_mapped(&wins);

	if(sz < 0)
		return lst;


	for(int i = 0; i < sz; i++)
		lst = cons(sc, mk_integer(sc, wins[i]), lst);

	XFree(wins);
	return scheme_reverse(sc, lst);
}

/* (wm-get-window-title id) */
static pointer s_wm_get_window_title(scheme *sc, pointer args) {
	fl_open_display();

	if(args == sc->NIL)
		return sc->F;

	pointer a = sc->vptr->pair_car(args);
	if(a == sc->NIL || !sc->vptr->is_number(a))
		return sc->F;

	unsigned long  id    = sc->vptr->ivalue(a);
	char          *title = netwm_window_get_title(id);
	pointer        ret   = sc->F;

	if(title) {
		ret = mk_string(sc, title);
		free(title);
	}

	return ret;
}

void register_wm_functions(scheme* sc) {
	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "wm-get-window-list"),
		sc->vptr->mk_foreign_func(sc, s_wm_get_window_list));

	sc->vptr->scheme_define(
		sc,
		sc->global_env,
		sc->vptr->mk_symbol(sc, "wm-get-window-title"),
		sc->vptr->mk_foreign_func(sc, s_wm_get_window_title));
}
