#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <win.h>
#include <abuf.h>
#include <string.h>
#include <state.h>
#include <editor.h>


struct editor_context *create_editor_ctx() {
  struct editor_context *ctx = malloc(sizeof(struct editor_context));
  get_window_sz(&ctx->rows, &ctx->cols);
  struct state_t *state = state_create();
  ctx->state = state;
  struct append_buf *buf = create_append_buffer();
  ctx->abuf = buf;
  ctx->cx = 0;
  ctx->cy = 0;
  ctx->row = NULL;
  ctx->numrows = 0;
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
    if(y >= ctx->numrows) {
      if (ctx->numrows == 0 && y == ctx->rows/ 3) {
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
    } else {
      int len = ctx->row[y].sz;
      if(len > ctx->cols) len = ctx->cols;
      awrite(ctx, ctx->row[y].chars, len);
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

int editor_open(struct editor_context *ctx, char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    return -1;
  }
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  linelen = getline(&line, &linecap, fp);
  if (linelen != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    ctx->row.sz= linelen;
    ctx->row.chars = malloc(linelen + 1);
    memcpy(ctx->row.chars, line, linelen);
    ctx->row.chars[linelen] = '\0';
    ctx->numrows = 1;
  }
  free(line);
  fclose(fp);
  return 0;
}
