#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include "fsops.h"   // to get access to pixh_file_name

/*
 * Full of bloat half of which I don't really understand in greater detail,
 * ideally there would be a read_png function returning a struct containing
 * height, width and color values for each pixel but I'm afraid it's not
 * that obvious to write one due to C structs' fixed-size nature.
 *
 * As to taking average value itself, the algorithm is based on this article:
 * https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5
 */
void color_avg(char* filename) {
    /* Yet another _that_ type of file type checking, but we need
     * to be sure that what we throw into libpng is actually a png */
    if (! strstr(filename, ".png"))
        return;
    printf("\n%s\n", filename);
    FILE *fp = fopen(filename, "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info  = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);
    int width  = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);
    printf("width: %i\nheight: %i\ncolor_type: %i\nbit_depth: %i\n",
            width, height, color_type, bit_depth);
    //png_set_strip_16(png);
    //png_set_palette_to_rgb(png);
    //png_read_update_info(png, info);
    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (png_byte*) malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    fclose(fp);
    /* Three *really* important and frequently used variables, maybe I could use
     * some magical C keywords on them to speed things up */
    unsigned long long red_accumulator   = 0;
    unsigned long long green_accumulator = 0;
    unsigned long long blue_accumulator  = 0;
    unsigned long pixel_count = 0;
    for (int i = 0; i < height; i++) {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++) {
            /* Not sure why, but it started working properly after changing 3 to 4 */
            png_bytep pixel = &(row[j * 3]);
            //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", i, j, pixel[0], pixel[1], pixel[2], pixel[3]);
            red_accumulator   += pow(pixel[0], 2);
            green_accumulator += pow(pixel[1], 2);
            blue_accumulator  += pow(pixel[2], 2);
            ++pixel_count;
        }
    }
    printf("RED:%llu, GREEN:%llu, BLUE:%llu\n", red_accumulator, green_accumulator, blue_accumulator);
    double red_avg   = sqrt(red_accumulator   / (double)pixel_count);
    double green_avg = sqrt(green_accumulator / (double)pixel_count);
    double blue_avg  = sqrt(blue_accumulator  / (double)pixel_count);
    printf("avg-RED:%f, avg-GREEN:%f, avg-blue:%f\n", red_avg, green_avg, blue_avg);
    printf("%lu\n", pixel_count);
    /* Down there I write the averaged values in R-G-B\n format to .pixh file, line by line */
    red_avg   = round(red_avg);
    green_avg = round(green_avg);
    blue_avg  = round(blue_avg);
    FILE* pixh_file = fopen(pixh_file_name, "a");
    fprintf(pixh_file, "%d-%d-%d\n", (int)red_avg, (int)green_avg, (int)blue_avg);
    fclose(pixh_file);
}
