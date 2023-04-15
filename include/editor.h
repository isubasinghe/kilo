#ifndef EDITOR_H 
#define EDITOR_H 

#include <def.h>

struct erow {
  size_t sz;
  char *chars;
};

struct editor_context {
  int cx;
  int cy;
  int rows;
  int cols;
  struct state_t *state;
  struct append_buf *abuf;
  struct erow row;
  size_t numrows;
};

struct editor_context *create_editor_ctx();
void awrite(struct editor_context *ctx, char *data, size_t sz);
void clear_screen(__attribute__((unused)) struct editor_context *ctx);
void draw_rows(struct editor_context *ctx);
void refresh_screen(struct editor_context *ctx);
#endif 
