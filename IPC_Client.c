#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 100

int main(int argc, char *argv[]) {
  int shm_id;
  key_t key;
  char *shm_ptr, *s;

  /* Creating */
  key = 1201;
  shm_id = shmget(key, SIZE, 0666);

  if (shm_id < 0) {
    perror("shmget error (client)\n");
    exit(1);
  }

  /* Attaching */
  shm_ptr = shmat(shm_id, NULL, 0);
  if (shm_ptr == (char *)-1) {
    perror("shmat error (client)\n");
    exit(1);
  }

  /*  Use the memory */
  for (s = shm_ptr; *s != 0; s++) {
    printf("%c ", *s);
  }

  /* Put '*' after reading to show server we are done */
  *shm_ptr = '*';
  printf("\nIt's done from client\n");

  /* Detaching */
  shmdt(shm_ptr);

  return 0;
}
