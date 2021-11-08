#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/capability.h>

#define SLAVE_EXE "./slave"

int main(void) {
  int optval = 1;
  int sock;

  cap_t	caps_old = cap_get_file(SLAVE_EXE);
  ssize_t length_p;
  char *cap_text = cap_to_text(caps_old, &length_p);
  printf("Текущие возможности slave: %s\n", cap_text);
  cap_free(cap_text);

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket");
	}

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
    perror("socket");
  }

	close(sock);
}