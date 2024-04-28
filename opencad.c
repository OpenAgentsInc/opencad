/**
 * Initial OpenCAD library: provides functions for drawing and saving pixel buffers.
 * This library provides basic drawing functions and a function to save the pixel buffer to a PPM file.
 */

#ifndef OPENCAD_C_
#define OPENCAD_C_

/**
 * Swaps the values of two integers.
 * @param a The first integer.
 * @param b The second integer.
 */
void swap_int(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/**
 * Fills the pixel buffer with a solid color.
 * @param pixels The pixel buffer.
 * @param width The width of the pixel buffer.
 * @param height The height of the pixel buffer.
 * @param color The color to fill with.
 */
void opencad_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color)
{
    for (size_t i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }
}

typedef int Errno;

#define return_defer(value) do { result = (value); goto defer; } while (0)

/**
 * Saves the pixel buffer to a PPM file.
 * @param pixels The pixel buffer.
 * @param width The width of the pixel buffer.
 * @param height The height of the pixel buffer.
 * @param file_path The path to the file to save to.
 * @return An error code indicating the result of the operation.
 */
Errno opencad_save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path)
{
    int result = 0;
    FILE *f = NULL;

    {
        f = fopen(file_path, "wb");
        if (f == NULL) return_defer(errno);

        fprintf(f, "P6\n%zu %zu 255\n", width, height);
        if (ferror(f)) return_defer(errno);

        for (size_t i = 0; i < width*height; ++i) {
            uint32_t pixel = pixels[i];
            uint8_t bytes[3] = {
                (pixel>>(8*0))&0xFF,
                (pixel>>(8*1))&0xFF,
                (pixel>>(8*2))&0xFF,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            if (ferror(f)) return_defer(errno);
        }
    }

defer:
    if (f) fclose(f);
    return result;
}

/**
 * Fills a rectangle in the pixel buffer with a solid color.
 * @param pixels The pixel buffer.
 * @param pixels_width The width of the pixel buffer.
 * @param pixels_height The height of the pixel buffer.
 * @param x0 The x-coordinate of the top-left corner of the rectangle.
 * @param y0 The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color to fill with.
 */
void opencad_fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                      int x0, int y0, size_t w, size_t h,
                      uint32_t color)
{
    for (int dy = 0; dy < (int) h; ++dy) {
        int y = y0 + dy;
        if (0 <= y && y < (int) pixels_height) {
            for (int dx = 0; dx < (int) w; ++dx) {
                int x = x0 + dx;
                if (0 <= x && x < (int) pixels_width) {
                    pixels[y*pixels_width + x] = color;
                }
            }
        }
    }
}

/**
 * Fills a circle in the pixel buffer with a solid color.
 * @param pixels The pixel buffer.
 * @param pixels_width The width of the pixel buffer.
 * @param pixels_height The height of the pixel buffer.
 * @param cx The x-coordinate of the center of the circle.
 * @param cy The y-coordinate of the center of the circle.
 * @param r The radius of the circle.
 * @param color The color to fill with.
 */
void opencad_fill_circle(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                        int cx, int cy, int r,
                        uint32_t color)
{
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;
    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < (int) pixels_height) {
            for (int x = x1; x <= x2; ++x) {
                if (0 <= x && x < (int) pixels_width) {
                    int dx = x - cx;
                    int dy = y - cy;
                    if (dx*dx + dy*dy <= r*r) {
                        pixels[y*pixels_width + x] = color;
                    }
                }
            }
        }
    }
}

/**
 * Draws a line on the pixel buffer.
 * @param pixels The pixel buffer.
 * @param pixels_width The width of the pixel buffer.
 * @param pixels_height The height of the pixel buffer.
 * @param x1 The x-coordinate of the start point.
 * @param y1 The y-coordinate of the start point.
 * @param x2 The x-coordinate of the end point.
 * @param y2 The y-coordinate of the end point.
 * @param color The color of the line.
 */
void opencad_draw_line(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                      int x1, int y1, int x2, int y2,
                      uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx != 0) {
        int c = y1 - dy*x1/dx;

        if (x1 > x2) swap_int(&x1, &x2);
        for (int x = x1; x <= x2; ++x) {
            if (0 <= x && x < (int) pixels_width) {
                int sy1 = dy*x/dx + c;
                int sy2 = dy*(x + 1)/dx + c;
                if (sy1 > sy2) swap_int(&sy1, &sy2);
                for (int y = sy1; y <= sy2; ++y) {
                    if (0 <= y && y < (int) pixels_height) {
                        pixels[y*pixels_width + x] = color;
                    }
                }
            }
        }
    } else {
        int x = x1;
        if (0 <= x && x < (int) pixels_width) {
            if (y1 > y2) swap_int(&y1, &y2);
            for (int y = y1; y <= y2; ++y) {
                if (0 <= y && y < (int) pixels_height) {
                    pixels[y*pixels_width + x] = color;
                }
            }
        }
    }
}

#endif // OPENCAD_C_
