#include <edelib/Scheme.h>
#include <edelib/Debug.h>
#include <stdio.h>

/*
						 cons(sc,
							  sc->QUOTE,
							  cons(sc, mk_atom(sc,p+2), sc->NIL)),
*/	

static bool is_main_widget(scheme *s, const char *w) {
	pointer key, val, ret, func, args = s->NIL;
	val = edelib_scheme_mk_string(s, ":main");
	key = edelib_scheme_mk_string(s, w);

	args = edelib_scheme_cons(s, val, args);
	args = edelib_scheme_cons(s, key, args);

	func = edelib_scheme_mk_symbol(s, "eml-get-as-string");
	/* call the function */
	scheme_eval(s, edelib_scheme_cons(s, func, args));
	//edelib_scheme_call(s, func, args);

	return 1;
#if 0 
	E_DEBUG("> %s\n", w);

	char buf[128];
	//snprintf(buf, sizeof(buf), "(widget-get-id \"%s\")", w);
	snprintf(buf, sizeof(buf), "(eml-get-unquoted \"%s\" ':id)", w);
	E_DEBUG("===> %s\n", buf);
	//edelib_scheme_load_string(s, buf);

	return (s->value == s->T);
#endif
}

static void scan_ids(scheme *s, pointer ids) {
	E_RETURN_IF_FAIL(ids != NULL);
	E_RETURN_IF_FAIL(ids != s->NIL);

	pointer val;
	for(pointer i = ids; edelib_scheme_is_pair(s, i) && i != s->NIL; i = edelib_scheme_pair_cdr(s, i)) {
		val = edelib_scheme_pair_car(s, i);
		if(!edelib_scheme_is_string(s, val)) continue;

		E_DEBUG("%s:%i\n", edelib_scheme_string_value(s, val), is_main_widget(s, edelib_scheme_string_value(s, val)));
		//E_DEBUG("%s\n", edelib_scheme_string_value(s, val));
	}
}

int main(int argc, char **argv) {
	if(argc != 2) {
		puts("Usage: eml-run [file.eml]");
		return 1;
	}

	scheme *s = edelib_scheme_init();
	E_RETURN_VAL_IF_FAIL(s != NULL, 1);

	FILE *fd = fopen("eml.ss", "r");
	E_RETURN_VAL_IF_FAIL(fd != NULL, 1);

	edelib_scheme_load_file(s, fd);
	fclose(fd);

	/* load .eml file */
	fd = fopen(argv[1], "r");
	if(!fd) {
		printf("Unable to load %s. Balling out...\n", argv[1]);
		return 1;
	}

	edelib_scheme_load_file(s, fd);
	fclose(fd);

	/* Now we have access to filled *widget-storage* var. To do so, call eml-get-all-ids. */
	pointer ret;
	ret = scheme_apply0(s, "eml-get-all-ids");

	E_RETURN_VAL_IF_FAIL(ret != NULL, 1);
	scan_ids(s, ret);

	edelib_scheme_deinit(s);
	return 0;
}
