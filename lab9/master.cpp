#include <stdio.h>
#include <sys/capability.h>

#define SLAVE_EXE "./slave"

int main(void) {
  char caps[] = "cap_net_admin=+eip";
  
  cap_t caps_new = cap_from_text(caps);
  if(cap_set_file(SLAVE_EXE, caps_new) == -1) {
    perror("Ошибка записи возможностей");
    return 1;
  }
}