#include "../drivers/screen.h"
#include "util.h"

void idle() {}

void init() {
    clear_screen();
    DEBUG("Succesfully loaded kernel into memory\n");
    DEBUG("Transferred control from bootSector to kernel\n");
}