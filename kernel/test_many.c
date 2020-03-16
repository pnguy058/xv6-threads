// Tests creating n threads, then joining n threads.
#include "types.h"
#include "user.h"

//Global variables used by all threads
int ppid;
int global = 0;

// If assert is True then continue, if False then test failed so kill process
#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n\n"); \
   kill(ppid); \
   exit(); \
}

// The instruction that thread_create will point to
void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 45);
   global++;
   exit();
}

int
main(int argc, char *argv[])
{
   ppid = getpid();
   global = 0;
   int arg = 45;

   if(argc != 2){
    printf(1, "ERROR!!!\nUsage: %s ThreadCount\n", argv[0]);
    exit();
   }
   int n = atoi(argv[1]);

   if (n < 1 || n > 100) {
    printf(1, "ThreadCount must between 1 and 100\n");
    exit();
   }
   if (n > 61) {
    printf(1, "Note: Program will crash when ThreadCount > 61\n");
   }

   int thread_pid[100] = {-1};
   int join_pid = -1;
   int i = 0;
   // Create n threads
   for(i = 0; i < n; i++) {
     thread_pid[i] = thread_create(worker, &arg);
     printf(1, "Created thread %d. PID : %d\n", (i+1), thread_pid[i]);
     assert(thread_pid[i] > 0);
    }
   // Join n threads
   for(i = 0; i < n; i++) {
     join_pid = thread_join();
     printf(1, "Joined : %d\n", join_pid);
     assert(thread_pid[i] == join_pid);
    }
   // Checks worker data race for global
   printf(1, "Global = %d.\n", global);
   assert(global == n);

   printf(1, "TEST PASSED\n\n");
   exit();
}
