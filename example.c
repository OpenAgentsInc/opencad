/**
 * Generates and saves three example images using the OpenCAD library.
 * A checkerboard pattern, circle pattern, and line pattern, each saved to a PPM file.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "opencad.c"

#define WIDTH 800
#define HEIGHT 600

#define COLS (8*2)
#define ROWS (6*2)
#define CELL_WIDTH  (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF

static uint32_t pixels[WIDTH*HEIGHT];

/**
 * Linearly interpolates between two values.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The interpolated value.
 */
float lerpf(float a, float b, float t)
{
    return a + (b - a)*t;
}

/**
 * Generates a checkerboard pattern and saves it to a PPM file.
 * @return True if the operation was successful, false otherwise.
 */
bool checker_example(void)
{
    opencad_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y)%2 == 0) {
                color = 0xFF2020FF;
            }
            opencad_fill_rect(pixels, WIDTH, HEIGHT, x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
        }
    }

    const char *file_path = "checker.ppm";
    Errno err = opencad_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }

    return true;
}

/**
 * Generates a circle pattern and saves it to a PPM file.
 * @return True if the operation was successful, false otherwise.
 */
bool circle_example(void)
{
    opencad_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            float u = (float)x/COLS;
            float v = (float)y/ROWS;
            float t = (u + v)/2;

            size_t radius = CELL_WIDTH;
            if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;

            opencad_fill_circle(pixels, WIDTH, HEIGHT,
                               x*CELL_WIDTH + CELL_WIDTH/2, y*CELL_HEIGHT + CELL_HEIGHT/2,
                               (size_t) lerpf(radius/8, radius/2, t),
                               FOREGROUND_COLOR);
        }
    }

    const char *file_path = "circle.ppm";
    Errno err = opencad_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

/**
 * Generates a line pattern and saves it to a PPM file.
 * @return True if the operation was successful, false otherwise.
 */
bool lines_example(void)
{
    opencad_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     0, 0, WIDTH, HEIGHT,
                     FOREGROUND_COLOR);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH, 0, 0, HEIGHT,
                     FOREGROUND_COLOR);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     0, 0, WIDTH/4, HEIGHT,
                     0xFF20FF20);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/4, 0, 0, HEIGHT,
                     0xFF20FF20);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH, 0, WIDTH/4*3, HEIGHT,
                     0xFF20FF20);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/4*3, 0, WIDTH, HEIGHT,
                     0xFF20FF20);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     0, HEIGHT/2, WIDTH, HEIGHT/2,
                     0xFFFF3030);

    opencad_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/2, 0, WIDTH/2, HEIGHT,
                     0xFFFF3030);

    const char *file_path = "lines.ppm";
    Errno err = opencad_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

/**
 * Generates a 3D brick pattern and saves it to a PPM file.
 * @return True if the operation was successful, false otherwise.
 */
bool brick_example(void)
{
    opencad_fill(pixels, WIDTH, HEIGHT, 0xFF000000); // Black background

    // Front face
    opencad_draw_line(pixels, WIDTH, HEIGHT, 200, 400, 400, 400, 0xFFFFFFFF); // White lines
    opencad_draw_line(pixels, WIDTH, HEIGHT, 400, 400, 400, 300, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 400, 300, 200, 300, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 200, 300, 200, 400, 0xFFFFFFFF);

    // Top face
    opencad_draw_line(pixels, WIDTH, HEIGHT, 200, 300, 250, 250, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 250, 250, 450, 250, 0xFFFFFFFF);

    // Right face
    opencad_draw_line(pixels, WIDTH, HEIGHT, 400, 400, 450, 350, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 450, 350, 450, 250, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 450, 250, 400, 300, 0xFFFFFFFF);
    opencad_draw_line(pixels, WIDTH, HEIGHT, 400, 300, 400, 400, 0xFFFFFFFF);

    const char *file_path = "brick.ppm";
    Errno err = opencad_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }

    return true;
}


/**
 * The main entry point of the program.
 * @return 0 if the program executed successfully, -1 otherwise.
 */
int main(void)
{
    if (!checker_example()) return -1;
    if (!circle_example()) return -1;
    if (!lines_example()) return -1;
    if (!brick_example()) return -1;
    return 0;
}
