#include <thread>
#include <future>
#include <unistd.h>
#include <semaphore.h>

void proc1 (bool &exitFlag, std::promise<int> &&returnValue, sem_t &sem) {
  printf("поток 1 начал работу\n");

  while (!exitFlag) {
    timespec current_time;
    do {
      clock_gettime(CLOCK_REALTIME, &current_time);
      current_time.tv_sec += 1;
    } while(sem_timedwait(&sem, &current_time) == -1);

    for(uint i = 0; i < 4; i++) {
      putchar('1');
      fflush(stdout);
      usleep(1000000);
    }

    sem_post(&sem);
    usleep(1000000);
  }

  returnValue.set_value(111);

  printf("поток 1 закончил работу\n");
}

void proc2 (bool &exitFlag, std::promise<int> &&returnValue, sem_t &sem) {
  printf("поток 2 начал работу\n");

  while (!exitFlag) {
    timespec current_time;
    do {
      clock_gettime(CLOCK_REALTIME, &current_time);
      current_time.tv_sec += 1;
    } while(sem_timedwait(&sem, &current_time) == -1);
    
    for(uint i = 0; i < 4; i++) {
      putchar('2');
      fflush(stdout);
      usleep(1000000);
    }

    sem_post(&sem);
    usleep(1000000);
  }

  returnValue.set_value(222);

  printf("поток 2 закончил работу\n");
}

int main (int argc, char ** argv) {
  printf("программа начала работу\n");

  std::promise<int> threadReturnValuePromise1,
                    threadReturnValuePromise2;

  auto  threadReturnValue1 = threadReturnValuePromise1.get_future(),
        threadReturnValue2 = threadReturnValuePromise2.get_future();

  bool  threadExitFlag1 = false,
        threadExitFlag2 = false;

  sem_t sem;
  sem_init(&sem, 1, 1);

  std::thread tA(proc1, std::ref(threadExitFlag1), std::move(threadReturnValuePromise1), std::ref(sem));
  std::thread tB(proc2, std::ref(threadExitFlag2), std::move(threadReturnValuePromise2), std::ref(sem));

  printf("программа ждет нажатия клавиши\n");
  getchar();
  printf("\nклавиша нажата\n");

  threadExitFlag1 = true;
  threadExitFlag2 = true;

  tA.join();
  tB.join();

  sem_destroy(&sem);

  printf("Thread return value 1: %d\n", threadReturnValue1.get());
  printf("Thread return value 2: %d\n", threadReturnValue2.get());

  printf("программа завершила работу\n");
}