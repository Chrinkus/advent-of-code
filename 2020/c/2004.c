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
#include <string.h>

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
	[BYR] = "byr", [IYR] = "iyr", [EYR] = "eyr", [HGT] = "hgt",
	[HCL] = "hcl", [ECL] = "ecl", [PID] = "pid", [CID] = "cid",
};

const unsigned REQUIRED_FIELDS = (1U << CID) - 1;

int get_field(const char* s)
{
	for (int i = 0; i < NUM_FIELDS; ++i)
		if (strcmp(field_labels[i], s) == 0)
			return i;
	return NUM_FIELDS;
}

enum eye_colors { AMB, BLU, BRN, GRY, GRN, HZL, OTH, NUM_EYE_COLORS };

const char* const eye_colors[NUM_EYE_COLORS] = {
	[AMB] = "amb", [BLU] = "blu", [BRN] = "brn", [GRY] = "gry",
	[GRN] = "grn", [HZL] = "hzl", [OTH] = "oth",
};

int get_eye_color(const char* s)
{
	for (int i = 0; i < NUM_EYE_COLORS; ++i)
		if (strcmp(eye_colors[i], s) == 0)
			return i;
	return NUM_EYE_COLORS;
}

enum measurement_systems { MET, IMP, NUM_MEASUREMENT_SYSTEMS };

const char* const measurement_systems[NUM_MEASUREMENT_SYSTEMS] = {
	[MET] = "cm",
	[IMP] = "in",
};

int get_measurement_system(const char* s)
{
	for (int i = 0; i < NUM_MEASUREMENT_SYSTEMS; ++i)
		if (strcmp(measurement_systems[i], s) == 0)
			return i;
	return NUM_MEASUREMENT_SYSTEMS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Passport validation
enum numeric_constraints {
	BYR_LO = 1920,
	BYR_HI = 2002,
	IYR_LO = 2010,
	IYR_HI = 2020,
	EYR_LO = 2020,
	EYR_HI = 2030,
	HGT_MET_LO = 150,
	HGT_MET_HI = 193,
	HGT_IMP_LO = 59,
	HGT_IMP_HI = 76,
	// string lengths
	YEAR_LEN = 4,
	HCL_LEN = 7,
	PID_LEN = 9,
};

const char* validate_year(const char* s, int lo, int hi)
{
	if (strlen(s) != YEAR_LEN)
		return NULL;

	int year = g_ascii_strtoll(s, NULL, 10);
	return lo <= year && year <= hi ? s : NULL;
}

const char* validate_height(const char* s)
{
	char* p = NULL;
	int height = g_ascii_strtoll(s, &p, 10);

	switch (get_measurement_system(p)) {
	case MET:
		return HGT_MET_LO <= height && height <= HGT_MET_HI ? s : NULL;
	case IMP:
		return HGT_IMP_LO <= height && height <= HGT_IMP_HI ? s : NULL;
	case NUM_MEASUREMENT_SYSTEMS:
		// error message?
		break;
	}
	return NULL;
}

const char* validate_hair(const char* s)
{
	if (strlen(s) != HCL_LEN)
		return NULL;
	if (*s != '#')
		return NULL;
	while (*++s)
		if (!g_ascii_isxdigit(*s))
			return NULL;
	return s;
}

const char* validate_pid(const char* s)
{
	if (strlen(s) != PID_LEN)
		return NULL;
	for ( ; *s; ++s)
		if (!g_ascii_isdigit(*s))
			return NULL;
	return s;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Passport processing
struct Passport {
	unsigned has_fields;
	unsigned pass_valid;
};

void process_passport(const char* data, struct Passport* pp)
{
	char** entries = g_strsplit(data, " ", 0);
	for (char** p = entries; *p; ++p) {
		char** line = g_strsplit(*p, ":", 0);
		switch (get_field(line[0])) {
		case BYR:
			pp->has_fields |= 1U << BYR;
			if (validate_year(line[1], BYR_LO, BYR_HI))
				pp->pass_valid |= 1U << BYR;
			break;
		case IYR:
			pp->has_fields |= 1U << IYR;
			if (validate_year(line[1], IYR_LO, IYR_HI))
				pp->pass_valid |= 1U << IYR;
			break;
		case EYR:
			pp->has_fields |= 1U << EYR;
			if (validate_year(line[1], EYR_LO, EYR_HI))
				pp->pass_valid |= 1U << EYR;
			break;
		case HGT:
			pp->has_fields |= 1U << HGT;
			if (validate_height(line[1]))
				pp->pass_valid |= 1U << HGT;
			break;
		case HCL:
			pp->has_fields |= 1U << HCL;
			if (validate_hair(line[1]))
				pp->pass_valid |= 1U << HCL;
			break;
		case ECL:
			pp->has_fields |= 1U << ECL;
			if (get_eye_color(line[1]) != NUM_EYE_COLORS)
				pp->pass_valid |= 1U << ECL;
			break;
		case PID:
			pp->has_fields |= 1U << PID;
			if (validate_pid(line[1]))
				pp->pass_valid |= 1U << PID;
			break;
		case CID:
			/* ignore */
			break;
		case NUM_FIELDS:
			fprintf(stderr, "Unknown field: %s\n", line[0]);
		}
		g_strfreev(line);
	}
	g_strfreev(entries);
}

int has_required_fields(const struct Passport* pp)
{
	return pp->has_fields == REQUIRED_FIELDS;
}

int has_valid_fields(const struct Passport* pp)
{
	return pp->pass_valid == REQUIRED_FIELDS;
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

		if (has_required_fields(&pp)) {
			++part1;
			part2 += has_valid_fields(&pp);
		}
		g_string_truncate(pp_data, 0);
	}
	g_string_free(pp_data, TRUE);

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	g_object_unref(is);
	g_object_unref(base);

	return EXIT_SUCCESS;
}

