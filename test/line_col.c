// test chpeg_line_col()

#include <stdio.h>
#include "chpeg/util.h"

#define test(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: %s: test '%s' failed\n", __FILE__, __LINE__, __func__, #x); \
            return 1; \
        } \
    } while(0)

int main(void)
{
    const unsigned char *input = (const unsigned char *)
        "line 1\nfoo\nline 3";
    //              1 111111    offset
    //   0123456 7890 123456
    //
    //   1     2    3           line
    //
    //   1234560 1230 123456    col

    size_t line, col;

    chpeg_line_col(input, 0, &line, &col);
    test(line == 1 && col == 1);

    chpeg_line_col(input, 1, &line, &col);
    test(line == 1 && col == 2);

    chpeg_line_col(input, 3, &line, &col);
    test(line == 1 && col == 4);

    chpeg_line_col(input, 5, &line, &col);
    test(line == 1 && col == 6);

    // newline itself is col 0 of that line
    chpeg_line_col(input, 6, &line, &col);
    test(line == 2 && col == 0);

    chpeg_line_col(input, 7, &line, &col);
    test(line == 2 && col == 1);

    chpeg_line_col(input, 8, &line, &col);
    test(line == 2 && col == 2);

    chpeg_line_col(input, 9, &line, &col);
    test(line == 2 && col == 3);

    // newline itself is col 0 of that line
    chpeg_line_col(input, 10, &line, &col);
    test(line == 3 && col == 0);

    chpeg_line_col(input, 11, &line, &col);
    test(line == 3 && col == 1);

    chpeg_line_col(input, 13, &line, &col);
    test(line == 3 && col == 3);

    chpeg_line_col(input, 16, &line, &col);
    test(line == 3 && col == 6);

    return 0;
}

