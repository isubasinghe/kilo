#include <stdio.h>
#include <stdlib.h>
#include <ropes.h>

struct rope_t *rope_create() {
  struct rope_t *rope = calloc(1, sizeof(struct rope_t));
  if(rope == NULL) {
    return NULL;
  }

  return rope;
}
