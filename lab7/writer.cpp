#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <signal.h>

#define FIFO_NAME "./fifo_pipe"

bool exitFlag;
int fifo;
int message;
socklen_t messageLength;
int sock;

bool checkFifoExists () {
  if(mkfifo(FIFO_NAME, 0777) == -1) {
    return errno == EEXIST;
  }
  unlink(FIFO_NAME);
  return false;
}

void sig_handler (int signo) {
  printf("get SIGPIPE\n");
  fifo = -1;
}

int main (int argc, char *argv[]) {
  signal(SIGPIPE, sig_handler);
  std::cout << "программа начала работу" << std::endl;

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  exitFlag = false;
  fifo = -1;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  while(!exitFlag) {
    if (!checkFifoExists()) {
      fifo = -1;
      if (getchar() != -1) {
        exitFlag = true;
      }
      continue;
    }

    if (fifo == -1) {
      fifo = open(FIFO_NAME, O_WRONLY | O_NONBLOCK);
    }

    getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &message, &messageLength);

    if(write(fifo, &message, sizeof(int)) != -1) {
      std::cout << "Записанно сообщение: " << message << std::endl;
    }

    if (getchar() != -1) {
      exitFlag = true;
    }

    sleep(1);
  }

  close(fifo);
  close(sock);

  std::cout << "программа завершила работу" << std::endl;
}