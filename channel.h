#ifndef SRC_CHANNEL_H
#define SRC_CHANNEL_H

#include <stdlib.h>

struct sender{
  size_t msg_sz;
  int* sender_fd; // A pointer that points to the fd array in receiver.
};

struct receiver{
  size_t msg_sz;
  int receiver_fd[2]; // File descriptor for pipe.
};

void channel_init(struct receiver* recv, struct sender* sender, 
  size_t msg_sz);

void channel_get(struct receiver* channel, void* data);

void channel_send(struct sender* channel, void* data);

void sender_dup(struct sender* dest, struct sender* src);

void channel_destroy(struct receiver* recv, struct sender* sender);

#endif

