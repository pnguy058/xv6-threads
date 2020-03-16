// Tests creating a single thread, then joining the thread.
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   global = 1;
   int arg = 35;

   if(argc != 2){
    printf(1, "ERROR!!!\nUsage: %s ThreadCount\n", argv[0]);
    exit();
   }
   int n = atoi(argv[1]);

   if (n < 1) {
    printf(1, "ThreadCount must between 1 and 100\n");
    exit();
   }

   int thread_pid[100] = {-1};
   int join_pid = -1;

   for(int i = 0; i < n; i++) {
     thread_pid[i] = thread_create(worker, &arg);
     printf(1, "Created thread %d. PID : %d\n", (i+1), thread_pid[i]);
     assert(thread_pid[i] > 0);
    }
   for(int i = 0; i < n; i++) {
     join_pid = thread_join();
     printf(1, "Joined : %d\n", join_pid);
    }

   printf(1, "Global = %d.\n", global);
   assert(global == (n + 1));
   
   printf(1, "TEST PASSED\n\n");
   exit();
}

void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 35);
   global++;
   exit();
}
