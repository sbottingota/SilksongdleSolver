#ifndef GUI_H
#define GUI_H

// include 'bool' as a type, if it does not already exist (as it does in C++)
#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void gui_init(void);
void gui_frame(void);
void gui_shutdown(void);

bool gui_is_running(void);

#ifdef __cplusplus
}
#endif

#endif
