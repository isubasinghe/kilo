#ifndef TERMIO_H 
#define TERMIO_H 

#include <termios.h>


int termio_disable_raw_mode();

int termio_enable_raw_mode();

int termio_init();
int termio_end();

#endif ///TERMIO_H 
