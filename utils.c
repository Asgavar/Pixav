#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

/* 
 * Does roughly the same thing as basename(char*) from libgen.h,
 * which means it returns pure file name with preceding path cut.
 * <Turned out it was present even on Gentoo,
 * but I'll leave it as it is>
 */
char* basename(char* full_path) {
    /* The path might be needed later, so let's create a copy of it! */
    char* full_path_copy = malloc(sizeof(full_path));
    strcpy(full_path_copy, full_path);
    while (strstr(full_path_copy, "/")) {
        strncpy(full_path_copy, full_path_copy, strlen(full_path_copy));
    }
    return full_path_copy;
}

/*
 * Creates a temporary folder with a name that resembles file name.
 */
void create_temp_folder(char* filename) {
    char* dot_position = strchr(filename, '.');
    /* Remove the extension part first (wo_ext == without extension) */
    char* wo_ext = malloc(sizeof(filename));
    strncpy(wo_ext, filename,
            strlen(filename) - strlen(dot_position));
    puts(wo_ext);
    char* temp_prefix = "temp_";
    char* folder_name = malloc(sizeof(wo_ext) + sizeof(temp_prefix));
    /* Funny things happen when you try to concatenate empty string */
    strcpy(folder_name, temp_prefix);
    strcat(folder_name, wo_ext);
    puts(folder_name);
    /* S_IRWXU is rwx equivalent in mode_t */
    mkdir(folder_name, S_IRWXU);
}
