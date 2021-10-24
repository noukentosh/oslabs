#include <iostream>
#include <fstream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <signal.h>

#define WRITERSEM "/labsem.writter"
#define READERSEM "/labsem.reader"

bool exitFlag;
sem_t *writerSem;
sem_t *readerSem;
int shmid;
int *sharedMem;

void exitAction () {
  shmdt(sharedMem);
  shmctl(shmid, IPC_RMID, NULL);
  sem_close(writerSem);
  sem_close(readerSem);
  sem_unlink(WRITERSEM);
  sem_unlink(READERSEM);
  std::cout << "программа завершила работу" << std::endl;
  exit(0);
}

void SIGINT_handler (int sig) {
  if(sig == 2) exitAction();
}

int main (int argc, char *argv[]) {
  if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
    printf("SIGINT install error\n");
    exit(1);
  }

  std::cout << "программа начала работу" << std::endl;

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  exitFlag = false;
  writerSem = sem_open(WRITERSEM, O_CREAT, 0777, 0);
  readerSem = sem_open(READERSEM, O_CREAT, 0777, 1);
  key_t sharedMemKey = ftok("./keylock", 1);
  shmid = shmget(sharedMemKey, sizeof(int), IPC_CREAT | 0777);
  if (shmid < 0) {
		perror("shmget");
	}

  sharedMem = (int *)shmat(shmid, NULL, 0);
  if (sharedMem == (void *)-1) {
    perror("shmat");
  }
  int localVar;

  while(!exitFlag) {
    sem_wait(writerSem);

    localVar = *sharedMem;
    std::cout << "Результат работы функции: " << localVar << std::endl;

    sem_post(readerSem);

    if (getchar() != -1) {
      exitFlag = true;
    }

    sleep(1);
  }

  shmdt(sharedMem);
  shmctl(shmid, IPC_RMID, NULL);
  sem_close(writerSem);
  sem_close(readerSem);
  sem_unlink(WRITERSEM);
  sem_unlink(READERSEM);

  std::cout << "программа завершила работу" << std::endl;
}