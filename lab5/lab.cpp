#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SHAREDSEM "/labsem"
#define CHAROUT "1"

int main (int argc, char *argv[]) {
  std::cout << "программа начала работу" << std::endl;

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  sem_t *sem = sem_open(SHAREDSEM, O_CREAT, 0777, 1);
  std::ofstream outputFile("output.txt", std::ios::app);
  bool exitFlag = false;

  if (outputFile.is_open()) {
    std::cout << "Файл успешно открыт" << std::endl;
    std::cout << "Программа ждет нажатия клавиши" << std::endl;
    while (!exitFlag) {
      sem_wait(sem);
      
      for(int i = 0; i < 5; i++) {
        std::cout << CHAROUT << std::flush;
        outputFile << CHAROUT << std::flush;
        sleep(1);
      }

      sem_post(sem);

      if (getchar() != -1) {
        exitFlag = true;
      }
            
      sleep(1);
    }
  }

  outputFile.close();
  sem_close(sem);
  sem_unlink(SHAREDSEM);

  std::cout << "программа завершила работу" << std::endl;
}