#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char *argv[]) {
  int shm_id;
  key_t mem_key;
  char *shm_ptr, *s;

  /* Creating */
  mem_key = 1201;
  shm_id = shmget(mem_key, SIZE, IPC_CREAT | 0666);
  if (shm_id < 0) {
    perror("shmget error (server)\n");
    exit(1);
  }

  /* Attaching */
  shm_ptr = shmat(shm_id, NULL, 0);
  if (shm_ptr == (char *)-1) {
    perror("shmat error (server)\n");
    exit(1);
  }

  /* Use the memory */
  s = shm_ptr;
  for (char c = 'a'; c <= 'z'; c++) {
    *s = c;
    s++;
  }
  *s = 0;
  // Wait for client/other process
  fprintf(stdout, "Waiting for client");
  while (*shm_ptr != '*') {
    fprintf(stdout, ".");
    sleep(1);
  }
  printf("\n");

  /*  Detaching */
  shmdt(shm_ptr);

  /*  Removing */
  shmctl(shm_id, IPC_RMID, NULL);

  return 0;
}
