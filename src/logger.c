#include <stdbool.h>
#include <logger.h>

static struct logger_t *__global_logger = NULL;

struct logger_t *create_file_logger(char *filename) {
  FILE *fptr = fopen(filename, "wb");
  if(fptr == NULL) { 
    return NULL;
  }
  struct logger_t *l = malloc(sizeof(struct logger_t));
  if(l == NULL) {
    return NULL;
  }
  l->fptr = fptr;
  return l;
}


bool setup_global_file_logger(char *filename) {
  struct logger_t *l = create_file_logger(filename);
  if(l == NULL) {
    return false;
  }
  __global_logger = l;
  return true;
}

int logger_printf(struct logger_t * l, const char *format, ...) {
  int ret = 0;
  va_list myargs;
  va_start(myargs, format);
  ret = vfprintf(l->fptr, format, myargs);
  va_end(myargs);
  return ret;
}

int logger_gprintf(const char *format, ...) {
  if(__global_logger == NULL) {
    return 0;
  }
  int ret = 0;
  va_list myargs;
  va_start(myargs, format);
  ret = vfprintf(__global_logger->fptr, format, myargs);
  va_end(myargs);
  return ret;
}


void free_logger(struct logger_t * logger) {
  int skip = logger->fptr == stderr || logger->fptr == stdin || logger->fptr == stdout;
  if(skip) {
    return; 
  }
  fclose(logger->fptr);
  free(logger);
}
