#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <ev.h>
#include <unistd.h>

#include "bufio.h"

int bufio_chain_append(bufio_chain *bc, void *buf, size_t len) {
  bufio_chain_entry *e = calloc(1, sizeof(*e));
  if (e != NULL) return -1;
  e->next = NULL;
  e->buf = buf;
  e->len = len;
  if (bc->head != NULL) {
    bc->tail->next = e;
  } else {
    bc->head = e;
  }
  bc->tail = e;
  return 0;
}

int bufio_chain_flush(bufio_chain *bc, int fd) {
  while (bc->head != NULL) {
    bufio_chain_entry *e = bc->head;
    int res = write(fd, e->buf+e->used, e->len-e->used);
    if (res < 0) return res;
    assert(e->used == e->len || res != 0);
    e->used += res;
    if (e->used == e->len) {
      bc->head = e->next;
      free(e->buf);
      free(e);
    }
  }
  bc->tail = NULL;
  return 0;
}

void bufio_chain_clear(bufio_chain *bc) {
  while (bc->head != NULL) {
    bufio_chain_entry *e = bc->head;
    free(e->buf);
    bc->head = e->next;
    free(e);
  }
  bc->tail = NULL;
}
