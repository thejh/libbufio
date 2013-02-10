#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <ev.h>

#include "bufio.h"

static void connection_waiting_cb(struct ev_loop *loop, ev_io *w, int revents) {
  bufio_connection *con = (bufio_connection *)w;
  if (revents & EV_READ) {
    assert(con->inbuf_used != con->inbuf_size);
    int res = read(w->fd, con->inbuf+con->inbuf_used, con->inbuf_size-con->inbuf_used);
    if (res == -1) {
      if (errno != EAGAIN && errno != EWOULDBLOCK) {
        con->err_cb(con);
      }
      return;
    }
    assert(res != 0);
    con->inbuf_used += res;
    if (con->inbuf_size == con->inbuf_used) {
      con->data_cb(con);
    }
  }

  if (revents & EV_WRITE) {
    if (bufio_chain_flush(&con->outbuf, w->fd) == 0) {
      ev_io_stop(loop, w);
      ev_io_set(w, w->fd, EV_READ);
      ev_io_start(loop, w);
    }
  }
}

bufio_connection *bufio_connection_create(struct ev_loop *loop, int fd) {
  bufio_connection *con = calloc(1, sizeof(*con));
  if (con == NULL) return NULL;
  con->loop = loop;
  ev_io_init(&con->w, connection_waiting_cb, fd, EV_READ);
  ev_io_start(loop, &con->w);
  return con;
}

void bufio_connection_set_read_buffer(bufio_connection *con, void *buf, size_t size) {
  assert(size > 0);
  con->inbuf = buf;
  con->inbuf_size = size;
  con->inbuf_used = 0;
}

void bufio_connection_destroy(bufio_connection *con) {
  bufio_chain_clear(&con->outbuf);
  ev_io_stop(con->loop, &con->w);
  free(con);
}

int bufio_connection_write(bufio_connection *con, void *buf, size_t len) {
  int res;
  bool chain_was_empty = (con->outbuf.head == NULL);
  res = bufio_chain_append(&con->outbuf, buf, len);
  if (res == -1) return -1;
  if (chain_was_empty) {
    if (bufio_chain_flush(&con->outbuf, con->w.fd)) {
      ev_io_stop(con->loop, &con->w);
      ev_io_set(&con->w, con->w.fd, EV_READ|EV_WRITE);
    }
  }
  return 0;
}
