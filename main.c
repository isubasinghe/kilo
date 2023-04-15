#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <state.h>
#include <win.h>
#include <abuf.h>
#include <string.h>

struct editor_context {
  int cx;
  int cy;
  int rows;
  int cols;
  struct state_t *state;
  struct append_buf *abuf;
};

struct editor_context *create_editor_ctx() {
  struct editor_context *ctx = malloc(sizeof(struct editor_context));
  get_window_sz(&ctx->rows, &ctx->cols);
  struct state_t *state = state_create();
  ctx->state = state;
  struct append_buf *buf = create_append_buffer();
  ctx->abuf = buf;
  ctx->cx = 0;
  ctx->cy = 0;
  return ctx;
}

void awrite(struct editor_context *ctx, char *data, size_t sz) {
  int res = write_buffer(ctx->abuf, data, sz);
  if(res < 0) {
    perror("unable to realloc, dying\n");
    exit(1);
  }
}

void clear_screen(__attribute__((unused)) struct editor_context *ctx) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_rows(struct editor_context *ctx) {
  int y;
  for (y = 0; y < ctx->rows; y++) {
  if (y == ctx->rows/ 3) {
      char welcome[80];
      int welcomelen = snprintf(welcome, sizeof(welcome),
        "Kilo editor -- version %s","0.0.1");
      if (welcomelen > ctx->cols) welcomelen = ctx->cols;
      int padding = (ctx->cols - welcomelen) / 2;
      if (padding) {
        awrite(ctx, "~", 1);
        padding--;
      }
      while (padding--) awrite(ctx, " ", 1);
      awrite(ctx, welcome, welcomelen);
    } else {
      awrite(ctx, "~", 1);
    }
    awrite(ctx, "\x1b[K", 3);
    if (y < ctx->rows - 1) {
      awrite(ctx, "\r\n", 2);
    }
  }
}

void refresh_screen(struct editor_context *ctx) {
  awrite(ctx, "\x1b[?25l", 6);
  awrite(ctx, "\x1b[H", 3);
  draw_rows(ctx);
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", ctx->cy + 1, ctx->cx + 1);
  awrite(ctx, buf, strlen(buf));
  awrite(ctx, "\x1b[?25h", 6);
  write_fd(ctx->abuf, STDOUT_FILENO); 
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
