#include <thread>
#include <future>
#include <unistd.h>
#include <semaphore.h>
#include <deque>
#include <chrono>
#include <sys/socket.h>

#define MAX_CAPACITY 4

void proc1 (bool &exitFlag, std::deque<int> &pipeline, int &sock) {
  printf("поток 1 начал работу\n");

  while(!exitFlag) {
    if(pipeline.size() >= MAX_CAPACITY) {
      printf("Pipeline переполнен\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    /*pipeline.push_back(([&](int sock) {
      int result;
      socklen_t resultLength;
      getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &result, &resultLength);
      return result;
    })(sock));*/

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  printf("поток 1 закончил работу\n");
}

void proc2 (bool &exitFlag, std::deque<int> &pipeline) {
  printf("поток 2 начал работу\n");

  while(!exitFlag) {
    if(pipeline.empty()) {
      printf("Pipeline пуст\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      continue;
    }
    printf("Результат работы потока 1: %d\n", pipeline.at(0));
    pipeline.pop_front();
  }

  printf("поток 2 закончил работу\n");
}

int main (int argc, char ** argv) {
  printf("программа начала работу\n");

  bool threadExitFlag = false;
  std::deque<int> pipeline;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  std::thread tA(&proc1, std::ref(threadExitFlag), std::ref(pipeline), std::ref(sock));
  std::thread tB(&proc2, std::ref(threadExitFlag), std::ref(pipeline));

  printf("программа ждет нажатия клавиши\n");
  getchar();
  printf("\nклавиша нажата\n");

  threadExitFlag = true;

  tA.join();
  tB.join();

  close(sock);

  printf("программа завершила работу\n");
}