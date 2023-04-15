#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <errors.h>

static struct termios orig_termios;

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
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
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
