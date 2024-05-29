#ifndef BUTTON_H
#define BUTTON_H

/* Module to initialize and cleanup button thread.

*/

void initialize_button();
void create_button_thread();
void join_button_thread();
void shutdown_button();

int get_button_state();

#endif
