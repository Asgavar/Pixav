#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

/* 
 * Does roughly the same thing as basename(char*) from libgen.h,
 * which means it returns pure file name with preceding path cut.
 * <Turned out it was present in <string.h>,
 * but I'll leave it as it is>
 */
char* basename(char* full_path) {
    /* The path might be needed later, so let's create a copy of it! */
    char* full_path_copy = malloc(strlen(full_path) + 1);
    strcpy(full_path_copy, full_path);
    char* filename = strstr(full_path_copy, "/"); 
    while (filename) {
        strncpy(full_path_copy, full_path_copy, strlen(filename));
        filename = strstr(full_path_copy, "/");
        puts(filename);
    }
    return full_path_copy;
}

/*
 * Returns a temporary folder name which resembles the file name.
 */
char* temp_folder_name(char* filename) {
    char* dot_position = strchr(filename, '.');
    /* Remove the extension part first (wo_ext == without extension) */
    char* wo_ext = malloc(sizeof(filename));
    strncpy(wo_ext, filename,
            strlen(filename) - strlen(dot_position));
    char* temp_prefix = "temp_";
    char* folder_name = malloc(sizeof(wo_ext) + sizeof(temp_prefix));
    /* Funny things happen when you try to concatenate empty string */
    strcpy(folder_name, temp_prefix);
    strcat(folder_name, wo_ext);
    return folder_name;
}

int strcompare(const void* a, const void* b) {
    return strcmp(* (char * const *) a, * (char * const *) b);
}

/*
 * Applies the given function to every file (image) in temp folder.
 * TODO: filtering already processed files.
 */
/* void tmpdirloop(void (*func)(char* filename)) { */
void tmpdirloop() {
    DIR* dir;
    struct dirent* de;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    dir = opendir(cwd);
    /* File counting could be put in a separate function,
     * but imo it's less messy this way */
    int file_count = 0;
    while (de = readdir(dir)) {
        ++file_count;
    }
    rewinddir(dir);
    char*  filenames[file_count];
    int    file_no = 0;
    while (de = readdir(dir)) {
        /* printf("%d\t%s\n", (int) de->d_ino, de->d_name); */
        /* Only . and .. have such length */
        if (strlen(de->d_name) < 3) {
            continue;
        }
        filenames[file_no] = de->d_name;
        ++file_no;
    }
    /* Names follow the frameXXXXXX pattern so their length is fixed */
    qsort(filenames, file_no, sizeof(filenames[0]), &strcompare);
    for (int i=0; i<file_no; i++) {
        printf("%s\n", filenames[i]);
    }
}
