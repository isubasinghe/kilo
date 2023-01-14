#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <errors.h>

int termio_disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) < 0) {
    return ERROR_TERM_ATTR_SET;
  }
  return 0;
}

int termio_enable_raw_mode() {
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
    return ERROR_TERM_ATTR_SET;
  }
  return 0;
}

int termio_init() {
  if(tcgetattr(STDIN_FILENO, &orig_termios) < 0) {
    return ERROR_TERM_ATTR_GET;
  }
  return 0;
}

int termio_end() {
  return termio_disable_raw_mode();
}
