#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <win.h>
#include <abuf.h>
#include <string.h>
#include <state.h>
#include <editor.h>
#include <logger.h>


struct editor_context *create_editor_ctx() {
  struct editor_context *ctx = malloc(sizeof(struct editor_context));
  get_window_sz(&ctx->rows, &ctx->cols);
  struct state_t *state = state_create();
  ctx->state = state;
  struct append_buf *buf = create_append_buffer();
  ctx->abuf = buf;
  ctx->cx = 0;
  ctx->cy = 0;
  ctx->row = malloc(128*sizeof(struct erow *));
  ctx->numrows = 0;
  ctx->rowcap = 128;

  ctx->rowoff = 0;
  ctx->coloff = 0;
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
    int filerow = y + ctx->rowoff;
    if(filerow >= ctx->numrows) {
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
      int len = ctx->row[filerow].sz;
      if(len > ctx->cols) len = ctx->cols;
      awrite(ctx, ctx->row[filerow].chars, len);
    }

    awrite(ctx, "\x1b[K", 3);
    if (y < ctx->rows - 1) {
      awrite(ctx, "\r\n", 2);
    }

  }
}

void editor_scroll(struct editor_context *ctx) {
  if(ctx->cy < ctx->rowoff) {
    ctx->rowoff = ctx->cy;
  }
  if(ctx->cy >= ctx->rowoff + ctx->rows) {
    ctx->rowoff = ctx->cy - ctx->rows + 1;
  }
}
void refresh_screen(struct editor_context *ctx) {
  editor_scroll(ctx);
  awrite(ctx, "\x1b[?25l", 6);
  awrite(ctx, "\x1b[H", 3);
  draw_rows(ctx);
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", ctx->cy + 1, ctx->cx + 1);
  awrite(ctx, buf, strlen(buf));
  awrite(ctx, "\x1b[?25h", 6);
  write_fd(ctx->abuf, STDOUT_FILENO); 
}

void editor_append_row(struct editor_context *ctx, char *s, size_t len) { 
  // 64 is PADDING
  if(ctx->numrows + 64 >= ctx->rowcap) {
    size_t sz = sizeof(struct erow) * (ctx->rowcap * 2);
    logger_gprintf("GOT SIZE %d\n", sz);
    ctx->row = realloc(ctx->row, sz);
    ctx->rowcap = ctx->rowcap * 2;
  }

  size_t at = ctx->numrows;
  ctx->row[at].sz = len;
  ctx->row[at].chars = malloc(len + 1);
  memcpy(ctx->row[at].chars, s, len);
  ctx->row[at].chars[len] = '\0';
  ctx->numrows = ctx->numrows + 1;
}

int editor_open(struct editor_context *ctx, char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    return -1;
  }

  while(1) {
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;
    linelen = getline(&line, &linecap, fp);
    if(linelen == -1) {
      break;
    }
    while(linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r')) {
      linelen--;
    }
    editor_append_row(ctx, line, linelen);
    linecap = 0;
    linelen = 0;
    free(line);
  }
  fclose(fp);
  return 0;
}
