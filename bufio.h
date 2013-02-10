#ifndef _BUFIO_HEADER
#define _BUFIO_HEADER

#include <ev.h>

/* BUFFER CHAIN */
typedef struct bufio_chain_entry bufio_chain_entry;

struct bufio_chain_entry {
  bufio_chain_entry *next;
  void *buf;
  size_t len;
  off_t used;
};

typedef struct {
  bufio_chain_entry *head, *tail;
} bufio_chain;

/**
 * Enqueues the given buffer for writing. After writing, it will be freed.
 * Returns -1 if calloc failed.
 */
int bufio_chain_append(bufio_chain *bc, void *buf, size_t len);

/**
 * Flush as much data as possible into the given FD.
 * Returns -1 for errors, 0 for ok. errno is left
 * intact on errors, so you might want to check for
 * EAGAIN or so.
 */
int bufio_chain_flush(bufio_chain *bc, int fd);

/**
 * Clear the buffer chain without sending any data.
 */
void bufio_chain_clear(bufio_chain *bc);



/* CONNECTION */
typedef struct bufio_connection bufio_connection;

typedef void (*bufio_connection_error_cb_t)(bufio_connection *con);
typedef void (*bufio_connection_data_cb_t)(bufio_connection *con);

struct bufio_connection {
  ev_io w;
  struct ev_loop *loop;
  void *inbuf;
  size_t inbuf_size;
  size_t inbuf_used;
  bufio_chain outbuf;

  /**
   * Called when an error has occured. You should react to it by destroying
   * the connection. This property can be changed manually at any time.
   */
  bufio_connection_error_cb_t err_cb;

  /**
   * Called when your read buffer has been filled completely. You have to
   * provide a new one immediately. This property can be changed manually
   * at any time.
   */
  bufio_connection_data_cb_t data_cb;

  /**
   * Not touched by this library.
   */
  void *data;
};

/**
 * Creates a bufio connection around an existing file descriptor.
 * This call already activates the watcher without initializing some
 * important stuff, so set it before returning to the eventloop/entering
 * it:
 *
 * - the input buffer – set it using `bufio_connection_set_read_buffer`
 * - the callbacks (`err_cb` and `data_cb`) – set them directly
 */
bufio_connection *bufio_connection_create(struct ev_loop *loop, int fd);
void bufio_connection_set_read_buffer(bufio_connection *con, void *buf, size_t size);
void bufio_connection_destroy(bufio_connection *con);
int bufio_connection_write(bufio_connection *con, void *buf, size_t len);

#endif
