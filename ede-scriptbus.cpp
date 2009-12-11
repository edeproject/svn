#include <stdio.h>
#include <string.h>

#include "tinyscheme/scheme.h"
#include "tinyscheme/scheme-private.h"
#include "tinyscheme/init_scm.h"
#include "sys.h"
#include "comm.h"
#include "string.h"
#include "wm.h"

static void help(void) {
	puts("Usage: ede-scriptbus [OPTIONS] [FILE]");
	puts("A simple interpreter for easier talk with EDE desktop");
	puts("Options:");
	puts("   -h           Show this help");
	puts("   -e [expr]    Execute expression and quit");
}

int main(int argc, char **argv) {
	FILE       *fin  = NULL;
	const char *expr = NULL;
	scheme      sc;

	if(argc > 1) { 
		if(argv[1][0] == '-') {
			if((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)) {
				help();
				return 0;
			} else if((strcmp(argv[1], "--expression") == 0) || (strcmp(argv[1], "-e") == 0)) {
				if(!argv[2]) {
					printf("This option requires a parameter\n");
					return 1;
				}
				expr = argv[2];
			} else {
				printf("Unrecognized option. Use 'ede-scriptbus --help' for options\n");
				return 1;
			}
		}

		if(!expr) {
			fin = fopen(argv[1], "r");
			if(!fin) {
				fprintf(stderr, "Unable to load '%s' file!\n", argv[1]);
				return 1;
			}
		}
	}

	if(!scheme_init(&sc)) {
		fprintf(stderr, "Fatal: Unable to initialize interpreter!\n");
		return 1;
	}

	scheme_set_input_port_file(&sc, stdin);
	scheme_set_output_port_file(&sc, stdout);

	/* load basic stuff */
	scheme_load_string(&sc, init_scm_content);

	/* register additional functions */
	register_sys_functions(&sc);
	register_communication_functions(&sc);
	register_string_functions(&sc);
	register_wm_functions(&sc);

	if(!expr) {
		/* load file or go into console */
		if(!fin) {
			fin = stdin;
			printf("Type '(quit)' or press Ctrl-D to quit");
		}

		scheme_load_file(&sc, fin);
	} else {
		/* or execute expression */
		scheme_load_string(&sc, expr);
		if(sc.retcode != 0)
			printf("Bad expression: '%s'\n", expr);
	}

	scheme_deinit(&sc);
	return 0;
}
