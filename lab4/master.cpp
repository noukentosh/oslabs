#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  printf("мастер программа начала работу\n");

  const char *arguments[5] = { "Param1", "Param2", "Param3", "Param4", NULL };
  const char *env[5] = { "env1", "env2", "env3", "env4", NULL };

  pid_t	pid	=	fork();
  int slaveStatus;

  if (pid == 0) {
    if(execve("./slave", const_cast<char* const *>(arguments), const_cast<char* const *>(env)) == -1){
      perror("execve");
    }
  } else if (pid > 0) {
    printf("PID = %d\n", getpid());
    printf("Parent PID = %d\n", getppid());
    printf("Slave PID = %d\n", pid);

    while (waitpid(pid, &slaveStatus, WNOHANG) == 0) {
      printf("мастер программа ждет завершения подпроцесса\n");
      sleep(1);
    }

    printf("подпроцесс вернул код: %d\n", WEXITSTATUS(slaveStatus));
  } else {
    perror("fork");
  }

  printf("мастер программа ждет нажатия клавиши\n");
  getchar();
  printf("\nклавиша нажата\n");

  printf("мастер программа завершила работу\n");
}