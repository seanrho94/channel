#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "channel.h"

/*
###My Approach###
1. Note that we have to use same fd (file descriptor) for sender and receiver to communicate data through pipe.
2. To achieve this, I have created fd array (receiver_fd[2]) in receiver struct.
3. Then I've created int pointer (sender_fd) in sender struct.
4. When channel_init is called, int pointer in sender struct points to the address of fd array in receiver struct.
5. Now sender and receiver struct shares the same fd (file descriptor) and they can communicate through pipe.
6. When sender is duplicated by sender_dup, the duplicated sender still points to the same address of fd array in receiver struct
as we copied it by using memcpy().
*/

void channel_init(struct receiver* recv, struct sender* sender, size_t msg_sz) {
  // If recv OR sender is NULL, do nothing.
  if (recv == NULL || sender == NULL) {
    return;
  }
  // Create pipe by using fd array from receiver struct.
  int p = pipe(recv->receiver_fd);
  // Receiver and sender must agree on a size of messages during initialisation. 
  recv->msg_sz = msg_sz;
  sender->msg_sz = msg_sz;
  // Make sure sender_fd points to the fd array in the receiver.
  sender->sender_fd = recv->receiver_fd;
}

void channel_get(struct receiver* channel, void* data) {
  // Do nothing if channel OR data is NULL.
  if (channel == NULL || data == NULL){
    return;
  }
  // Read from the pipe and store it into the given buffer (void* data in this case).
  read(channel->receiver_fd[0], data, channel->msg_sz);
}

void channel_send(struct sender* channel, void* data) {
  // Do nothing if channel OR data is NULL.
  if (channel == NULL || data == NULL){
    return;
  }
  // Write the given data to the pipe. (Send it through pipe).
  write(channel->sender_fd[1], data, channel->msg_sz);
}

void sender_dup(struct sender* dest, struct sender* src) {
  // Do nothing if dest sender OR src sender is NULL.
  if (dest == NULL || src == NULL){
    return;
  }
  // Copy every fields in src sender to dest sender.
	memcpy(dest, src, sizeof(struct sender));
}

// Do not need this function as I did not malloc any objects.
void channel_destroy(struct receiver* recv, struct sender* sender) {

}

int main(){
  struct sender* send = (struct sender*)malloc(sizeof(struct sender));
  struct receiver* receive = (struct receiver*)malloc(sizeof(struct receiver));
  struct sender* send1 = (struct sender*)malloc(sizeof(struct sender));

  channel_init(receive, send, 10);
  printf("sender: %d\n", send->msg_sz);

  char* msg1 = "0123456789"; 
  char inbuf[10];

  channel_send(send, msg1);
  //printf("%s\n", send->buf);
  channel_get(receive, inbuf);
  printf("%s\n", inbuf);

  return 0;
}
