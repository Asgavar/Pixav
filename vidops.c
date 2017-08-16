#include <stdlib.h>
#include <stdio.h>

/*
 * Simply calls ffmpeg (which is assumed to be installed) with variable arguments
 */
void split_to_frames(char* path_to_file, char* temp_folder) {
    char* ffmpeg_command = malloc(2048);
    /* -i stands for input, output is provided afterwards */
    sprintf(ffmpeg_command, "ffmpeg -i %s -qscale:v 0 %s/frame%%06d.jpg", path_to_file, temp_folder);
    system(ffmpeg_command);
}
