#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#ifndef ASCII_VERSION
# define ASCII_VERSION "1.0"
#endif /* ASCII_VERSION */

static int	ascii_process_options(int ac, char** av);
static int	ascii_print_table(void);

static const char			*ascii_options_short = ":c:s:e:v";
static struct option		ascii_options_long[] = {
	{ "columns",	required_argument,	0, 'c' },
	{ "start",		required_argument,	0, 's' },
	{ "end",		required_argument,	0, 'e' },
	{ "version",	no_argument,		0, 'v' },
	{ 0, 0, 0, 0 }
};

static const char			*ascii_whitespaces[32] = {
	"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
	"BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI",
	"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
	"CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US",
};

static unsigned int			ascii_print_columns =	16;
static char					ascii_print_start =		0;
static char					ascii_print_end	=		CHAR_MAX;

int main(int ac, char** av) {
	if (!ascii_process_options(ac, av))
		return (EXIT_FAILURE);

	if (!ascii_print_table())
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

static int	ascii_process_options(int ac, char** av) {
	char c = 0;
	while ((c = getopt_long(ac, av, ascii_options_short, ascii_options_long, NULL)) != -1) {
		switch (c) {
			case 'c': { ascii_print_columns = atoi(optarg); } break;
			case 's': { ascii_print_start = atoi(optarg); } break;
			case 'e': { ascii_print_end = atoi(optarg); }	break;
			case 'v': {	fprintf(stdout, "%s\n", ASCII_VERSION); exit(EXIT_SUCCESS); };
		}
	}

	if (ascii_print_start > ascii_print_end) {
		fprintf(stderr, "[ERR] Starting point (%i) greater than finish point (%i)\n", ascii_print_start, ascii_print_end);
		return (0);
	}

	return (1);
}

static int	ascii_print_table(void) {
	for (unsigned int y = 0; y < ascii_print_columns; y++) {
		for (unsigned int x = 0; x < (CHAR_MAX + 1) / ascii_print_columns + 1; x++) {
			int	c = ascii_print_start + (y + ascii_print_columns * x);
			if (c > ascii_print_end)
				break;

			if (!isprint(c)) {
				if (c < 32)
					fprintf(stdout, "%3i %s\t", c, ascii_whitespaces[c]);
				else
					fprintf(stdout, "%3i %s\t", c, "ESC");
			}

			else
				fprintf(stdout, "%3i %c\t", c, c);	
		}
		
		fputc('\n', stdout);	
	}
	return (0);
}
