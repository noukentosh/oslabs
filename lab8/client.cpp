#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>

#define SERVER "127.0.0.1"
#define PORT 8888

struct sockaddr_in si_server;
socklen_t slen;

void workSocket (bool &exitFlag, int &sock) {
  int buf;
	int message = 1;
	bool noError = true;
  
  while (!exitFlag) {
		noError = true;
		if (sendto(sock, &message, sizeof(int), 0, (struct sockaddr *) &si_server, slen) == -1) {
			perror("sendto");
	    exit(1);
		}

		printf("Клиент запросил данные\n");
		
		if (recvfrom(sock, &buf, sizeof(int), 0, (struct sockaddr *) &si_server, &slen) == -1) {
			noError = false;
		}

		if (!noError) {
			continue;
		}

		printf("Клиент принял данные\n");
		
		printf("Данные от сервера: %d\n", buf);
		sleep(1);
  }
}

int main(void) {
	bool exitFlag = false;
	int sock;
	slen = sizeof(si_server);

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket");
	  exit(1);
	}

	struct timeval read_timeout;
	read_timeout.tv_sec = 1;
	read_timeout.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	
	if (inet_aton(SERVER , &si_server.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	std::thread processSocket (workSocket, std::ref(exitFlag), std::ref(sock));

	printf("программа ждет нажатия клавиши\n");
  getchar();
  printf("\nклавиша нажата\n");

  exitFlag = true;

  processSocket.join();

	close(sock);
}