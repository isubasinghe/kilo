#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <state.h>

int main() {
  if(termio_init() < 0) {
    return 1;
  }

  if(termio_enable_raw_mode() < 0) {
    return 1;
  }
  
  struct state_t *state = state_create();
  struct state_action_t action;
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    state_handle_input(state, c, &action);
    switch (action.action) {
      case QUIT:
        goto exit_loop;
      case INSERT_CHAR:
        break;
      case NOOP:
        break;
    }
  }
exit_loop:;

  termio_end();
  return 0;
}
