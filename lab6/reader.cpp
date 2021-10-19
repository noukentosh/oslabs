#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SHAREDSEM "/labsem"
#define CHAROUT "2"

int main (int argc, char *argv[]) {
  std::cout << "программа начала работу" << std::endl;

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  sem_t *sem = sem_open(SHAREDSEM, O_CREAT, 0777, 10);
  std::ofstream outputFile("output.txt", std::ios::app);
  bool exitFlag = false;

  if (outputFile.is_open()) {
    std::cout << "Файл успешно открыт" << std::endl;
    std::cout << "Программа ждет нажатия клавиши" << std::endl;
    while (!exitFlag) {
      sem_wait(sem);
      
      for(int i = 0; i < 5; i++) {
        std::cout << CHAROUT;
        outputFile << CHAROUT;
        sleep(1);
      }

      if (getchar() != -1) {
        exitFlag = true;
      }

      sem_post(sem);
      sleep(1);
    }
  }

  outputFile.close();
  sem_close(sem);
  sem_unlink(SHAREDSEM);

  std::cout << "программа завершила работу" << std::endl;
}