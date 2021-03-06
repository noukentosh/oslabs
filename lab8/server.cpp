#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>
#include <queue>

#define PORT 8888

struct sockaddr_in si_server, si_client;
socklen_t slen, recv_len;

struct clientRequest {
	struct sockaddr_in client;
	socklen_t slen;
};

std::queue<clientRequest> clientRequests;

void writeSocket (bool &exitFlag, int &sock) {
	int message;
	socklen_t messageLength;

	while (!exitFlag) {
		if(clientRequests.empty()) {
			sleep(1);
			continue;
		}

		struct clientRequest req = clientRequests.front();

		getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &message, &messageLength);

		if (sendto(sock, &message, messageLength, 0, (struct sockaddr *) &req.client, req.slen) == -1) {
			perror("sendto");
			exit(1);
		}

		printf("Сервер отправил данные\n");

		clientRequests.pop();
	}
}

void readSocket (bool &exitFlag, int &sock) {
  int buf;
	bool noError = true;
  
  while (!exitFlag) {
		noError = true;

		if ((recv_len = recvfrom(sock, &buf, sizeof(int), 0, (struct sockaddr *) &si_client, &slen)) == -1) {
	  	noError = false;
		}

		if (!noError) {
			continue;
		}

		printf("Клиент запросил данные\n");

		clientRequests.push({ si_client, slen });
  }
}

int main(void) {
  bool exitFlag = false;
	int optval = 1;
  int sock;
	slen = sizeof(si_client);

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket");
	  exit(1);
	}

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 500000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	memset((char *) &si_server, 0, sizeof(si_server));
	
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	si_server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sock, (struct sockaddr*) &si_server, sizeof(si_server)) == -1) {
		perror("bind");
	  exit(1);
	}
	
	std::thread readSocketThread (readSocket, std::ref(exitFlag), std::ref(sock));
	std::thread writeSocketThread (writeSocket, std::ref(exitFlag), std::ref(sock));

  printf("программа ждет нажатия клавиши\n");
  getchar();
  printf("\nклавиша нажата\n");

  exitFlag = true;

  readSocketThread.join();
	writeSocketThread.join();

	close(sock);
}