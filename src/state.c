#include <stdio.h>
#include <stdlib.h>
#include <state.h>
#include <def.h>


struct state_t* state_create() {
  struct state_t *state = calloc(1, sizeof(struct state_t));
  if(state == NULL) {
    return NULL;
  }
  state->mode = NORMAL;
  state->row = 0;
  state->col = 0;
  return state;
}


int state_handle_input(struct state_t *state, int chr, struct state_action_t *action) {
  action->action = NOOP;
  if (chr < 0) {
    return 0;
  }

  if(state->mode == INSERT) {
    if(chr == 27) {
      action->action = ENTER_NORMAL;
      return 0;
    }
    action->chr = chr;
    action->action = INSERT_CHAR;
  }else if (state->mode == NORMAL) {
    switch (chr) {
      case 'q':
        action->action = QUIT;
        break;
      case 'i':
        action->action = ENTER_INSERT;
        break;
    }
  }

  return 0;
}


