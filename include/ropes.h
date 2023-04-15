#ifndef ROPES_H 
#define ROPES_H

#include <stdint.h>

struct rope_t {
  struct rope_node_t *root;
};

struct rope_node_t {
  int node;
};


typedef char* rope_string;

struct rope_t *rope_create();

rope_string rope_index(struct rope_node_t *node, uint64_t i);

struct rope_node_t* rope_concat(struct rope_node_t *left, struct rope_node_t *right);

void split(struct rope_node_t *node);


#endif // ROPES_H
