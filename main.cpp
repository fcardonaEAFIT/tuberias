#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

using namespace std;

void testStatus(int status);

int
main(void) {

  int tub1[2];
  int tub2[2];
  int tub3[2];
  pid_t pidCat;

  pipe(tub1);

  if ((pidCat = fork()) == 0) { // Hijo
    dup2(tub1[1], 1);
    close(tub1[0]);
    close(tub1[1]);
    execlp("cat", "cat", "datos.txt", NULL);
    exit(EXIT_FAILURE);
  }

  pipe(tub2);
  pid_t pidSort;

  if ((pidSort = fork()) == 0) { // Hijo
    dup2(tub1[0], 0);
    dup2(tub2[1], 1);
    close(tub1[0]);
    close(tub1[1]);
    close(tub2[0]);
    close(tub2[1]);
    execlp("sort", "sort", NULL);
    exit(EXIT_FAILURE);
  }

  close(tub1[0]);
  close(tub1[1]);
  pid_t pidEgrep;

  if ((pidEgrep = fork()) == 0) { // Hijo
    dup2(tub2[0], 0);
    // dup2(tub3[1], 1);
    close(tub2[0]);
    close(tub2[1]);
    // close(tub3[0]);
    // close(tub3[1]);
    execlp("egrep", "egrep", "o$", NULL);
    exit(EXIT_FAILURE);
  }

  close(tub2[0]);
  close(tub2[1]);
  
  int status;
  waitpid(pidCat, &status, 0);
  testStatus(status);
  waitpid(pidSort, &status, 0);
  testStatus(status);
  waitpid(pidEgrep, &status, 0);
  testStatus(status);
  
  return EXIT_SUCCESS;
}

void
testStatus(int status) {
  if (WIFEXITED(status)) {
    cout << "Hijo: termino " << WEXITSTATUS(status) << endl;
  }
  else if (WIFSIGNALED(status)) {
    cout << "Hijo lo mataron " << WTERMSIG(status) << endl;
  }
}
