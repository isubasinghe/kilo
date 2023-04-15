#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <state.h>
#include <win.h>
#include <abuf.h>
#include <string.h>
#include <editor.h>

int main() {
  if(termio_init() < 0) {
    return 1;
  }

  if(termio_enable_raw_mode() < 0) {
    return 1;
  }
  
  struct editor_context *ctx = create_editor_ctx();
  struct state_action_t action;
  char c;
  while (1) {
    refresh_screen(ctx);
    if(read(STDIN_FILENO, &c, 1) < 0) {
      continue;
    }
    
    // GET INPUT 
    state_handle_input(ctx->state, c, &action);
    // PROCESS ACTION
    switch (action.action) {
      case QUIT:
        goto exit_loop;
      case INSERT_CHAR:
        break;
      case NOOP:
        break;
      case ENTER_INSERT:
        ctx->state->mode = INSERT;
        break;
      case ENTER_NORMAL:
        ctx->state->mode = NORMAL;
        break;
      case MOVE_UP:
        ctx->cy = ctx->cy - 1;
        break;
      case MOVE_DOWN:
        ctx->cy = ctx->cy + 1;
        break;
      case MOVE_LEFT:
        ctx->cx = ctx->cx - 1;
        break;
      case MOVE_RIGHT:
        ctx->cx = ctx->cx + 1;
        break;
    } 
    // reset state
    c = -1;
  }
exit_loop:;
  clear_screen(ctx);
  termio_end();
  return 0;
}
