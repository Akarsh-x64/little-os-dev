#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"


/* private kernel apis declerations*/
// cannot call

int get_cursor_offset();
void set_cursor_offset(int offset);
int putc(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/* PUBLIC KERNEL APIS */
// free to call

void atDEBUG(char *message, int col, int row) {
	int offset;
	if (col >= 0 && row >= 0) {
		offset = get_offset(col, row);
	} else {
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	int charCounter = 0;

	while (message[charCounter] != 0) {
		offset = putc(message[charCounter++], col, row, WHITE_ON_BLACK);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void DEBUG(char *message) {
    atDEBUG("[DEBUG] ", -1, -1);
	atDEBUG(message, -1, -1);
}

void vgaPrint(char *message) {
    atDEBUG(message, -1, -1);
}

/* Private kernel apis */
int putc(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            memory_copy((char *)get_offset(0, i) + VIDEO_ADDRESS,
                        (char *)get_offset(0, i-1) + VIDEO_ADDRESS,
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = (char *)get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = (char *)VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }
