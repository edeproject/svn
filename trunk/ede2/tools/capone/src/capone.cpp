#include <stdio.h>
#include <string.h>

extern "C" {
#include "scheme.h"
#include "scheme-private.h"
}

#include "dbus.h"
#include "re.h"
#include "sys.h"

#define VERSION   "0.1"
#define BASE_FILE "capone.init"
#define CHECK_ARGV(argv, pshort, plong) ((strcmp(argv, pshort) == 0) || (strcmp(argv, plong) == 0))

const char* next_param(int curr, char** argv, int argc) {
	int j = curr + 1;
	if(j >= argc)
		return NULL;
	if(argv[j][0] == '-')
		return NULL;
	return argv[j];
}

void help(void) {
	puts("Usage: capone [OPTIONS] [FILE]\n");
	puts("Options:");
	puts("   -h, --help             Show this help");
	puts("   -v, --version          Show version");
	puts("   -d, --lib-dir [dir]    Directory with startup libraries");
	puts("   -e, --eval    [str]    Evaluate given expression\n");
}

void do_file_or_expr(FILE* f, const char* expr, const char* dir) {
	scheme sc;
	if(!scheme_init(&sc)) {
		puts("Unable to load interpreter!");
		return;
	}

	scheme_set_input_port_file(&sc, stdin);
	scheme_set_output_port_file(&sc, stdout);

	char path[256];
	snprintf(path, sizeof(path), "%s/" BASE_FILE, dir);

	FILE* init = fopen(path, "r");
	if(!init) {
		printf("\n\nCan't load startup from %s\n", path);
		scheme_deinit(&sc);
		return;
	}

	scheme_load_file(&sc, init);
	if(sc.retcode != 0)
		puts("Errors in " BASE_FILE);

	register_dbus_functions(&sc);
	register_re_functions(&sc);
	register_sys_functions(&sc);

	if(f) {
		scheme_load_file(&sc, f);
		if(sc.retcode != 0)
			puts("Errors in file");
	}

	if(expr) {
		scheme_load_string(&sc, expr);
		if(sc.retcode != 0)
			printf("Errors in expression '%s'\n", expr);
	}

	scheme_deinit(&sc);
}

int main(int argc, char** argv) {
	const char* a, *l, *filename, *expr;
	l = "../lib";
	filename = NULL;
	expr = NULL;

	for(int i = 1; i < argc; i++) {
		a = argv[i];
		if(a[0] == '-') {
			if(CHECK_ARGV(a, "-h", "--help")) {
				help();
				return 0;
			} else if(CHECK_ARGV(a, "-v", "--version")) {
				puts(VERSION);
				return 0;
			} else if(CHECK_ARGV(a, "-d", "--lib-dir")) {
				l = next_param(i, argv, argc);
				if(!l) {
					puts("Missing directory parameter");
					return 1;
				}
				i++;
			} else if(CHECK_ARGV(a, "-e", "--expr")) {
				expr = next_param(i, argv, argc);
				if(!expr) {
					puts("Missing expression");
					return 1;
				}
				i++;
			} else {
				printf("Unknown '%s' parameter. Run capone -h for more options\n", a);
				return 1;
			}
		} else {
			filename = a;
			break;
		}
	}

	if(expr)
		do_file_or_expr(NULL, expr, l);
	else if(filename) {
		FILE* f = fopen(filename, "r");
		if(!f) {
			printf("Unable to open '%s'!\n", filename);
			return 1;
		}

		do_file_or_expr(f, NULL, l);
		fclose(f);
	} else {
		printf("\033[33mcapone " VERSION "\033[0m (based on tinyscheme 1.39)\n");
		printf("Type \"(quit)\" or press Ctrl-C to exit interpreter when you are done.");
		do_file_or_expr(stdin, NULL, l);
	}

	return 0;
}
