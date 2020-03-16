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
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   
   int arg = 35;
   //printf(1, "Before thread create.\n");
   //int thread_pid = thread_create(worker, &arg);
   //printf(1, "After thread create. Before assertion of thread_pid.\n");
   //assert(thread_pid > 0);
   //printf(1, "After assertion of thread_pid. Before thread_join\n");
   int threadpid1 = thread_create(worker, &arg);
   printf(1, "Thread 1 created with PID: %d\n", threadpid1);

   int threadpid2 = thread_create(worker, &arg);
   printf(1, "Thread 2 created with PID: %d\n", threadpid2);
   
   //int join_pid = thread_join();
   //printf(1, "After thread_join. Before join_pid assertion.\n");
   //assert(join_pid == threadpid1);
   //assert(global == 2);
   
   int join_pid = thread_join();
   printf(1, "Joining %d\n", join_pid);
   join_pid = thread_join();
   printf(1, "Joining %d\n", join_pid);
   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 35);
   assert(global == 1);
   global++;
   exit();
}
