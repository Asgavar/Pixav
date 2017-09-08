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
    /* What kind of abomination is that? */
    if (! strstr(filename, "frame"))
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
    fprintf(pixh_file, "%03d-%03d-%03d\n", (int)red_avg, (int)green_avg, (int)blue_avg);
    fclose(pixh_file);
}

/*
 * Reads .pixh file and draws a svg image based on its contents.
 */
void draw_svg(void) {
    FILE* pixh_file = fopen(pixh_file_name, "r");
    /* FIXME: SVG filename */
    FILE* svg_file  = fopen("sample.svg", "a");
    int line_count = 0;
    /* Every line is 11 chars long in fact */
    char rgb_buf[16];
    char* val_red = malloc(3+1);
    char* val_green = malloc(3+1);
    char* val_blue = malloc(3+1);
    while (fgets(rgb_buf, 16, pixh_file)) {
        ++line_count;
    }
    printf("%d", line_count);
    rewind(pixh_file);
    /* TODO: Shouldn't be a square */
    fprintf(svg_file, "<svg height=\"%d\" width=\"%d\">", line_count, line_count);
    int line_no = 0;
    while (fgets(rgb_buf, 16, pixh_file)) {
        puts(rgb_buf);
        /* The format is: RRR-GGG-BBB */
        strncpy(val_red, rgb_buf, 3);
        strncpy(val_green, rgb_buf+4, 3);
        strncpy(val_blue, rgb_buf+8, 3);
        /* NUL-termination time! Turned out pointers work just like arrays */
        val_red[3]   = '\0';
        val_green[3] = '\0';
        val_blue[3]  = '\0';
        printf("\n%s\n", val_red);
        printf("\n%s\n", val_green);
        printf("\n%s\n", val_blue);
        fprintf(svg_file, "<line x1=\"0\" x2=\"%d\" y1=\"%d\" y2=\"%d\" style=\"stroke:rgb(%s, %s, %s);stroke-width:5\" />", line_count, line_no*5, line_no*5, val_red, val_green, val_blue);
        ++line_no;
    }
    fprintf(svg_file, "</svg>");
    fclose(svg_file);
}
