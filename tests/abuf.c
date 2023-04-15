#include <abuf.h>
#include "utest.h"
char *hello_world_str = "hello world\0";

UTEST(ABUF, Write) {
  struct append_buf *buf = create_append_buffer();
  ASSERT_NE(buf, NULL);
  char *old_data = buf->data;

  int data = write_buffer(buf, hello_world_str, 12);
  ASSERT_EQ(0, data);
  ASSERT_EQ(12, buf->sz);
  ASSERT_STREQ(hello_world_str, buf->data);
  write_fd(buf, STDOUT_FILENO);
  ASSERT_EQ(old_data, buf->data);
  ASSERT_EQ(0, buf->sz);
}
