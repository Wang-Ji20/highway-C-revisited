#pragma once

// The Rio Package - Robust I/O functions
// from CSAPP

// purpose:
//  In TCP, it's common that the receive bytes is less
//  than we requested because some bytes may be transporting
//  in the Internet.
//  So we need a package of I/O functions to ensure we receive
//  correct amount of data.

#include "../hcr.h"

// APUE 3e p.522
ssize_t rio_readn(int fd, void* usrbuf, size_t n);

ssize_t rio_writen(int fd, void* usrbuf, size_t n);

enum {
  RIO_BUFSIZE = 8192,
};

typedef struct {
  int rio_fd;
  size_t rio_cnt;
  char* rio_bufptr;
  char rio_buf[RIO_BUFSIZE];
} rio_t;

void rio_readinitb(rio_t *rp, int fd); 
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
