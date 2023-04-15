#ifndef ABUF_H 
#define ABUF_H 

#define APPEND_BUF_INIT_SZ 1024
#define APPEND_BUF_PAD 8
#define APPEND_BUF_GROWTH_SZ 2
#include <def.h>
#include <unistd.h>
struct append_buf {
  size_t sz;
  size_t cap;
  char *data;
};


struct append_buf *create_append_buffer();


void free_append_buffer(struct append_buf *buf);

int write_buffer(struct append_buf *buf, char *new_data, size_t sz);

ssize_t write_fd(struct append_buf *buf, int fd);

#endif 
