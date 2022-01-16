/*
 * GLib Usage Info
 *
 * g_strsplit
 * ==========
 * Definitely excited to use this functionality. Worked exactly as hoped!
 *
 * GIO
 * ===
 * Was up in the air about using this or just sticking with `scanf`. Certainly
 * a LOT of extra work to use not including needing to separately include
 * and link unix-specific parts of the library. Will probably not use this
 * going forward.
 */
#include <stdlib.h>
#include <stdio.h>

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixinputstream.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Constants and accessor
enum fields {
	BYR,		// Birth year
	IYR,		// Issue year
	EYR,		// Expiration year
	HGT,		// Height
	HCL,		// Hair colour
	ECL,		// Eye colour
	PID,		// Passport ID
	CID,		// Country ID (ignored)
	NUM_FIELDS,
};

const char* const field_labels[NUM_FIELDS] = {
	[BYR] = "byr",
	[IYR] = "iyr",
	[EYR] = "eyr",
	[HGT] = "hgt",
	[HCL] = "hcl",
	[ECL] = "ecl",
	[PID] = "pid",
	[CID] = "cid",
};

const unsigned REQUIRED_FIELDS = (1U << CID) - 1;

int get_field(const char* s)
{
	for (int i = 0; i < NUM_FIELDS; ++i)
		if (strcmp(field_labels[i], s) == 0)
			return i;
	return NUM_FIELDS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Passport processing
struct Passport {
	unsigned fbits;
};

void process_passport(const char* data, struct Passport* pp)
{
	char** entries = g_strsplit(data, " ", 0);
	for (char** p = entries; *p; ++p) {
		char** line = g_strsplit(*p, ":", 0);
		switch (get_field(line[0])) {
		case BYR:	pp->fbits |= 1U << BYR;	break;
		case IYR:	pp->fbits |= 1U << IYR;	break;
		case EYR:	pp->fbits |= 1U << EYR;	break;
		case HGT:	pp->fbits |= 1U << HGT;	break;
		case HCL:	pp->fbits |= 1U << HCL;	break;
		case ECL:	pp->fbits |= 1U << ECL;	break;
		case PID:	pp->fbits |= 1U << PID;	break;
		case CID:	/* ignore */		break;
		case NUM_FIELDS:
			fprintf(stderr, "Unknown field: %s\n", line[0]);
		}
		g_strfreev(line);
	}
	g_strfreev(entries);
}

int has_required_fields(const struct Passport* pp)
{
	return pp->fbits == REQUIRED_FIELDS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// GIO read from stdin
GString* read_passport_data(GDataInputStream* is, GString* data)
{
	gsize line_len = 0;
	for (char* line; (line = g_data_input_stream_read_line(is, &line_len,
					NULL, NULL)); ) {
		if (line_len == 0) {
			free(line);
			break;
		}

		if (data->len > 0)	// appending multiple lines
			g_string_append_c(data, ' ');

		g_string_append(data, line);
		free(line);
	}
	return data->len ? data : NULL;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main()
{
	printf("Advent of Code 2020 Day 4: Passport Processing\n");

	GInputStream* base = g_unix_input_stream_new(fileno(stdin), FALSE);
	GDataInputStream* is = g_data_input_stream_new(base);

	int part1 = 0;
	int part2 = 0;

	GString* pp_data = g_string_new(NULL);
	while (read_passport_data(is, pp_data)) {
		struct Passport pp = { 0 };
		process_passport(pp_data->str, &pp);

		part1 += has_required_fields(&pp);

		g_string_truncate(pp_data, 0);
	}
	g_string_free(pp_data, TRUE);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	g_object_unref(is);
	g_object_unref(base);

	return EXIT_SUCCESS;
}

