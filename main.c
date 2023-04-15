#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <state.h>
#include <win.h>

struct editor_context {
  int rows;
  int cols;
  struct state_t *state;
};

struct editor_context *create_editor_ctx() {
  struct editor_context *ctx = malloc(sizeof(struct editor_context));
  get_window_sz(&ctx->rows, &ctx->cols);
  struct state_t *state = state_create();
  ctx->state = state;
  return ctx;
}

void clear_screen(__attribute__((unused)) struct editor_context *ctx) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_rows(struct editor_context *ctx) {
  int y;
  for (y = 0; y < ctx->rows; y++) {
    write(STDOUT_FILENO, "~", 1);
    if (y < ctx->rows - 1) {
      write(STDOUT_FILENO, "\r\n", 2);
    }
  }
}

void refresh_screen(struct editor_context *ctx) {
  clear_screen(ctx); 
  draw_rows(ctx);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

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
        printf("%d ('%c') \r\n", action.chr, action.chr);
        break;
      case NOOP:
        break;
      case ENTER_INSERT:
        ctx->state->mode = INSERT;
        printf("SWITCHING TO INSERT\r\n");
        break;
      case ENTER_NORMAL:
        ctx->state->mode = NORMAL;
        printf("SWITCHING TO NORMAL\r\n");
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
