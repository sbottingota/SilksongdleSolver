#include <stdio.h>

#include "gui.h"

int main() {
    printf("Hello, world!\n(App init)\n");

    gui_init();

    while (gui_is_running()) {
        gui_frame();
    }

    gui_shutdown();
}

