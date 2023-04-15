#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <abuf.h>
#include <def.h>
#include <unistd.h>

struct append_buf *create_append_buffer() {
  struct append_buf *buf = malloc(sizeof(struct append_buf));
  buf->sz = 0;
  buf->data = malloc(sizeof(char)*APPEND_BUF_INIT_SZ);
  buf->cap = APPEND_BUF_INIT_SZ;
  return buf;
}

int write_buffer(struct append_buf *buf, char *new_data, size_t sz) {
  if(buf->cap - sz - buf->sz  - APPEND_BUF_PAD <= 0) {
    size_t new_cap = buf->cap * 2;
    char *new_data = realloc(buf->data, new_cap) ;
    if(new_data == NULL) {
      return -1;
    }

    buf->cap = buf->cap * 2;
  }

  buf->sz = buf->sz + sz;
  memcpy(&buf->data[buf->sz], new_data, sz);

  return 0;
}

ssize_t write_fd(struct append_buf *buf, int fd) {
  if(buf->sz <= 0) {
    return 0;
  }

  ssize_t written_amount = write(fd, buf->data, buf->sz);
  if(written_amount < 0) {
    return -1;
  }

  buf->sz = buf->sz - written_amount;
  memmove(buf->data, &buf->data[written_amount], written_amount); 
  return written_amount;
}

void free_append_buffer(struct append_buf *buf) {
  free(buf->data);
  free(buf);
}
