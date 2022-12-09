#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cgs/cgs.h>

enum Magic { DELIM = ' ', BIG_SIZE = 100000, };

const char* ROOT = "/";
const char* PARENT = "..";
const char* COMMAND = "$";
const char* CHANGE = "cd";
const char* LIST = "ls";
const char* DIRECTORY = "dir";

enum Type { TYPE_DIR, TYPE_FILE };

struct DirData {
        int size;
        struct cgs_vector files;
};

struct FileData {
        int size;
};

struct File {
        struct File* parent;
        char* name;
        enum Type type;
        union {
                struct DirData dir;
                struct FileData file;
        } data;
};

static struct File
file_new(struct File* parent, char* name, int size)
{
        return (struct File){
                .parent = parent,
                .name = name,
                .type = TYPE_FILE,
                .data.file = { .size = size },
        };
}

static struct File
dir_new(struct File* parent, char* name)
{
        return (struct File){
                .parent = parent,
                .name = name,
                .type = TYPE_DIR,
                .data.dir = {
                        .size = 0,
                        .files = cgs_vector_new(sizeof(struct File)),
                },
        };
}

static struct File*
dir_add_file(struct File* dir, const struct cgs_vector* subs)
{
        const struct cgs_strsub* ss0 = cgs_vector_get(subs, 0);
        const struct cgs_strsub* ss1 = cgs_vector_get(subs, 1);

        char* name = cgs_strsub_to_str(ss1);
        if (!name)
                return NULL;

        struct File file;
        int size;
        if (cgs_strsub_eq_str(ss0, DIRECTORY)) {
                file = dir_new(dir, name);
        } else if (cgs_strsub_to_int(ss0, &size)) {
                file = file_new(dir, name, size);
        } else {
                goto error_cleanup;
        }

        if (!cgs_vector_push(&dir->data.dir.files, &file))
                goto error_cleanup;
        return dir;
error_cleanup:
        free(name);
        return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static int
dir_pred(const void* a, const void* b)
{
        const struct File* file = a;
        const struct cgs_strsub* ss = b;

        return cgs_strsub_eq_str(ss, file->name);
}

static struct File*
change_directory(const struct cgs_strsub* ss, struct File* dir)
{
        if (dir->type != TYPE_DIR)
                return NULL;
        if (cgs_strsub_eq_str(ss, PARENT))
                return dir->parent;

        if (cgs_strsub_eq_str(ss, ROOT)) {
                while (dir->parent)
                        dir = dir->parent;
                return dir;
        }

        struct File* new_dir = cgs_vector_find(&dir->data.dir.files,
                        dir_pred, ss); 
        if (!new_dir)
                return NULL;

        return new_dir;
}

static struct File*
parse_command(const struct cgs_vector* subs, struct File* dir)
{
        const struct cgs_strsub* ss1 = cgs_vector_get(subs, 1);
        if (cgs_strsub_eq_str(ss1, CHANGE)) {
                const struct cgs_strsub* ss2 = cgs_vector_get(subs, 2);
                return change_directory(ss2, dir);
        }
        if (cgs_strsub_eq_str(ss1, LIST)) {
                return dir;
        }
        return NULL;
}

static struct File*
parse_line(const struct cgs_vector* subs, struct File* dir)
{
        const struct cgs_strsub* ss = cgs_vector_get(subs, 0);
        if (cgs_strsub_eq_str(ss, "$"))
                return parse_command(subs, dir);
        else
                return dir_add_file(dir, subs);
}

static void*
read_and_build_tree(struct File* curr)
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
        return curr;
}

static int
set_dir_sizes(struct File* dir)
{
        int sum = 0;
        struct cgs_vector* files = &dir->data.dir.files;
        for (size_t i = 0; i < cgs_vector_length(files); ++i) {
                struct File* file = cgs_vector_get_mut(files, i);
                if (file->type == TYPE_FILE)
                        sum += file->data.file.size;
                else if (file->type == TYPE_DIR)
                        sum += set_dir_sizes(file);
                else
                        printf("File type error\n");
        }
        dir->data.dir.size = sum;
        return sum;
}

static int
get_sum_of_small_dirs(const struct File* dir)
{
        int sum = 0, size = dir->data.dir.size;
        if (size <= BIG_SIZE)
                sum += size;

        const struct cgs_vector* files = &dir->data.dir.files;
        for (size_t i = 0; i < cgs_vector_length(files); ++i) {
                const struct File* file = cgs_vector_get(files, i);
                if (file->type == TYPE_FILE)
                        continue;
                sum += get_sum_of_small_dirs(file);
        }

        return sum;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
        struct File root = dir_new(NULL, strdup(ROOT));
        if (!read_and_build_tree(&root) || set_dir_sizes(&root) == 0)
                return EXIT_FAILURE;

        int part1 = get_sum_of_small_dirs(&root);
        printf("%d\n", part1);

        return EXIT_SUCCESS;
}
