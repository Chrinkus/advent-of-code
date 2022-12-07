#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>

enum Type { TYPE_DIR, TYPE_FILE };
enum Chars {
        COMMAND = '$',
        DIRECTORY = 'd',
        DELIM = ' ',
};

const char* HOME = "/";

struct File {
        struct File* parent;
        char* name;
        enum Type type;
        union {
                int size;
                struct cgs_vector files;
        } data;
};

struct File
file_new(struct File* parent, char* name, int size)
{
        return (struct File){
                .parent = parent,
                .name = name,
                .type = TYPE_FILE,
                .data.size = size,
        };
}

struct File
dir_new(struct File* parent, char* name)
{
        return (struct File){
                .parent = parent,
                .name = name,
                .type = TYPE_DIR,
                .data.files = cgs_vector_new(sizeof(struct File)),
        };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
dir_pred(const void* a, const void* b)
{
        const struct File* file = a;
        const char* name = b;

        return strcmp(file->name, name) == 0;
}

void*
change_directory(const struct cgs_strsub* ss, struct File* dir)
{
        if (dir->type != TYPE_DIR)
                return NULL;
        char* s = cgs_strsub_to_str(ss);
        struct File* new_dir = cgs_vector_find(&dir->data->files, dir_pred, s); 
        if (!new_dir)
                return NULL;

        free(s);
        return new_dir;
}

void*
parse_command(const struct cgs_vector* subs, struct File* dir)
{
        const struct cgs_strsub* ss1 = cgs_vector_get(subs, 1);
        if (cgs_strsub_eq_str(ss1, "cd")) {
                const struct cgs_strsub* ss2 = cgs_vector_get(subs, 2);
                return change_directory(dir, ss2);
        }
        if (cgs_strsub_eq_str(ss1, "ls")) {
                return dir;
        }
        return NULL;
}

void*
parse_line(const struct cgs_vector* subs, struct File* dir)
{
        const struct cgs_strsub* ss = cgs_vector_get(subs, 0);
        const char ch = ss->data[0];
        if (ch == COMMAND)
                return parse_command(subs, dir);
        if (ch == DIRECTORY)
                return parse_dir(subs, dir);
        if (isdigit(ch))
                return parse_file(subs, dir);
        return NULL;
}

void*
build_tree(struct File* curr)
{
        struct cgs_string buff = cgs_string_new();
        struct cgs_vector subs = cgs_vector_new(sizeof(struct cgs_strsub));
        while (cgs_io_getline(stdin, &buff) > 0) {
                if (!cgs_string_split(&buff, DELIM, &subs))
                        return NULL;
                curr = parse_line(&subs, curr);
                cgs_string_clear(&buff);
                cgs_vector_clear(&subs);
        }
        cgs_string_free(&buff);
        cgs_vector_free(&subs);
        return home;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
        struct File home = dir_new(NULL, strdup(HOME));
        if (!build_tree(&home))
                return EXIT_FAILURE;

        return EXIT_SUCCESS;
}
