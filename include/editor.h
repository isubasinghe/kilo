#ifndef EDITOR_H 
#define EDITOR_H 

#include <def.h>

struct erow {
  size_t sz;
  char *chars;
};

struct editor_context {
  // current position
  int cx;
  int cy;
  // window size data
  int rows;
  int cols;
  // state
  struct state_t *state;
  // buff writer
  struct append_buf *abuf;
  // data
  struct erow *row;
  size_t numrows;
  size_t rowcap;
  // Scrolling
  int rowoff;
  int coloff;
};

struct editor_context *create_editor_ctx();
void awrite(struct editor_context *ctx, char *data, size_t sz);
void clear_screen(__attribute__((unused)) struct editor_context *ctx);
void editor_scroll(struct editor_context *ctx);
void draw_rows(struct editor_context *ctx);
void refresh_screen(struct editor_context *ctx);

void editor_append_row(struct editor_context *ctx, char *s, size_t len);
int editor_open(struct editor_context *ctx, char *fname);
#endif 
