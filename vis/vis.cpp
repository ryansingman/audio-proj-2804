#include "vis.h"

void ascii_vis(float *look_dir) {
    /*
     * Creates ascii art representation of signal direction
     * Inputs:
     *      look_dir -- look direction of highest power beam
     */
    // convert look dir to radians
    float look_dir_rad = ((*look_dir) * M_PI / 180.) + 0.000000001;     // pad to adjust for undefined behavior at 0, 180

    // get terminal size
    int num_row, num_col;
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    num_row = size.ws_row - 2;      // reserve final row for next terminal line and top row for test status
    num_col = size.ws_col - 1;      // reserve final column for newline char

    // create bool matrix
    bool bool_mat[num_row][num_col] = { {0} };        // initialize bool matrix with zeros
    int origin_x = num_col / 2, origin_y = num_row;
    bool_mat[origin_y][origin_x] = 1;

    // fill out character matrix
    int row_idx = origin_y, col_idx = origin_x;
    int r = 1;  // radius
    while (((num_row >= row_idx) && (row_idx > 0)) && ((num_col > col_idx) && (col_idx > 0))) {
        row_idx = origin_y - (r * sin(look_dir_rad));
        col_idx = origin_x + (r * cos(look_dir_rad));
        bool_mat[row_idx][col_idx] = 1;
        r++;
    }

    // print out ascii art 
    for (int ii = 0; ii < num_row; ii++) {
        for (int jj = 0; jj < num_col; jj++) {
            if (bool_mat[ii][jj]) {
                printf("%s", BOLDBLUE);
                printf("*");
                printf("%s", RESET);
            }
            else printf(" ");
        }
        printf("\n");
    }
}
