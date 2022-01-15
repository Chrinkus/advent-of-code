#include <stdlib.h>
#include <stdio.h>

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixinputstream.h>

GString* read_passport(GDataInputStream* is, GString* data)
{
	gsize line_len = 0;
	gsize total_len = 0;
	for (char* line; (line = g_data_input_stream_read_line(is, &line_len,
					NULL, NULL)); ) {
		if (line_len == 0) {
			free(line);
			break;
		}

		if (total_len > 0)
			g_string_append_c(data, ' ');

		total_len += line_len;
		g_string_append(data, line);
		free(line);
	}
	return total_len ? data : NULL;
}

int main()
{
	printf("Advent of Code 2020 Day 4: Passport Processing\n");

	GInputStream* base = g_unix_input_stream_new(fileno(stdin), FALSE);
	GDataInputStream* is = g_data_input_stream_new(base);
	GString* pp_data = g_string_new(NULL);

	int part1 = 0;
	while (read_passport(is, pp_data)) {
		printf("%s\n", pp_data->str);
		g_string_truncate(pp_data, 0);
		++part1;
	}

	int part2 = 0;

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);

	g_object_unref(is);
	g_object_unref(base);
	g_string_free(pp_data, TRUE);

	return EXIT_SUCCESS;
}

