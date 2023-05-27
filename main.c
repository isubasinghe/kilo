#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <state.h>
#include <win.h>
#include <abuf.h>
#include <string.h>
#include <editor.h>
#include <logger.h>

int main(int argc, char *argv[]) {
  if(setup_global_file_logger("logs.txt") == 0) {
    printf("unable to open logs.txt\n");
    return 1;
  }  
  logger_gprintf("logger started\n");
  struct editor_context *ctx = create_editor_ctx();
  if(argc == 2) {
    int res = editor_open(ctx, argv[1]);
    if(res != 0) {
      perror("couldn't open file\n");
      goto exit_loop;
    }
  }else {
    if(editor_open(ctx, "main.c") != 0) {
      perror("Couldn't open file\n");
      goto exit_loop;
    }
  }
  if(termio_init() < 0) {
    return 1;
  }

  if(termio_enable_raw_mode() < 0) {
    return 1;
  }

  logger_gprintf("termio and raw mode enabled successfully\n");  

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
