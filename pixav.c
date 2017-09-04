#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/stat.h>
#include <editline/readline.h>
#include "fsops.h"
#include "vidops.h"
#include "imgops.h"

/*
 * Pixav is a tool that transforms a video file into an image made of
 * average/dominating pixel colors of its every frame.
 *
 * * (c) Artur "asgavar" Juraszek, 2017
 */

int main(int argc, char** argv) {
    /* Should check for ffmpeg existence first */
    char* video_file_path;
    if (argc == 2)
        video_file_path = argv[1];
    else {
        /* Check whether specified file (or folder!) exists */
        while (access(video_file_path, R_OK)) {
            video_file_path = readline("Video file location: ");
            /* A dirty hack to ensure that the path points to a file */
            if (! strstr(video_file_path, ".")) {
                puts("It must be a file!");
            }
        }
    }
    char* video_file_name = basename(video_file_path);
    char* dirname = temp_folder_name(video_file_name);
    /* S_IRWXU is user's rwx equivalent in mode_t */
    mkdir(dirname, S_IRWXU);
    split_to_frames(video_file_path, dirname);
    chdir(dirname);
    tmpdirloop(&color_avg);
    //color_avg("/home/asgavar/Pixav/samples/photo3.png");
}
