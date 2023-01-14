#include <stdio.h>
#include <stdlib.h>
#include <state.h>


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
  if(state->mode == INSERT) {

  }else if (state->mode == NORMAL) {
    switch (chr) {
      case 'q':
        action->action = QUIT;
    }
  }

  return 0;
}


