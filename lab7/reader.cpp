#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "./fifo_pipe"

bool exitFlag;
int fifo;
int buf;

bool checkFifoExists () {
  if(mkfifo(FIFO_NAME, 0777) == -1) {
    return errno == EEXIST;
  }
  unlink(FIFO_NAME);
  return false;
}

int main (int argc, char *argv[]) {
  std::cout << "программа начала работу" << std::endl;

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  exitFlag = false;
  fifo = -1;

  if(checkFifoExists()) {
    unlink(FIFO_NAME);
  }

  while(!exitFlag) {
    if (!checkFifoExists()) {
      mkfifo(FIFO_NAME, 0777);
    }

    if (fifo == -1) {
      fifo = open(FIFO_NAME, O_CREAT | O_RDONLY | O_NONBLOCK);
    }

    if(read(fifo, &buf, sizeof(int)) != 0) {
      std::cout << "Прочитано сообщение: " << buf << std::endl;
    }

    if (getchar() != -1) {
      exitFlag = true;
    }

    sleep(1);
  }

  close(fifo);
  unlink(FIFO_NAME);

  std::cout << "программа завершила работу" << std::endl;
}