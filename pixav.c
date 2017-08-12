#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/stat.h>
#include <editline/readline.h>
#include "fsops.h"
#include "vidops.h"

/*
 * Pixav is a tool that transforms a video file into an image made of average
 * pixel colors of its every frame.
 *
 * * (c) Artur "asgavar" Juraszek, 2017
 */

int main(int argc, char** argv) {
    /* ffmpeg existence checking should be more user-friendly */
    system("ffmpeg");
    char* video_file_path;
    /* Check whether specified file (or folder!) exists */
    while (access(video_file_path, R_OK)) {
        video_file_path = readline("Video file location: ");
        /* A dirty hack to ensure that the path points to a file */
        if (! strstr(video_file_path, ".")) {
            puts("It must be a file!");
            free(video_file_path);
        }
    }
    char* video_file_name = basename(video_file_path);
    puts(video_file_name);
    char* dirname = temp_folder_name(video_file_name);
    /* S_IRWXU is user's rwx equivalent in mode_t */
    mkdir(dirname, S_IRWXU);
    split_to_frames(video_file_path, dirname);
}
