#ifndef LOGGER_H 
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

struct logger_t {
  FILE *fptr;
};


struct logger_t *create_file_logger(char *filename);

bool setup_global_file_logger(char *filename);

int logger_printf(struct logger_t *, const char *format, ...);

int logger_gprintf(const char *format, ...);

void free_logger(struct logger_t *);



#endif // LOGGER_H
