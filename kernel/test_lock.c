// Tests creating n threads, then joining n threads.
#include "types.h"
#include "user.h"

//Global variables used by all threads
int ppid;
int sharedInt = 0;
lock_t lck;

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
   for (int i = 0; i < 10000; i++) {
     sharedInt += 100;
   }
   exit();
}
void
worker2(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 45);
   //while (sharedInt < 10000);
   sharedInt *= 2;
   sleep(35);
   sharedInt /= 2;
   exit();
}

int
main(int argc, char *argv[])
{
   ppid = getpid();
   sharedInt = 0;
   lock_init(&lck);

   int arg = 45;

   if(argc != 2){
    printf(1, "ERROR!!!\nUsage: %s ThreadCount\n", argv[0]);
    exit();
   }
   int n = atoi(argv[1]);

   if (n < 1 || (n % 2) != 0) {
    printf(1, "ThreadCount must even and >= 2\n");
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
     if (i % 2 == 0) {
       thread_pid[i] = thread_create(worker, &arg);
     } else {
       thread_pid[i] = thread_create(worker2, &arg);
     }
     printf(1, "Created Thread #%d with PID: %d\n", (i+1), thread_pid[i]);
     assert(thread_pid[i] > 0);
    }
   // Join n threads
   for(i = 0; i < n; i++) {
     join_pid = thread_join();
     printf(1, "Thread Joined with PID: %d\n", join_pid);
     //assert(thread_pid[i] == join_pid);
    }

   printf(1, "sharedInt = %d\n", sharedInt);
   assert(sharedInt == n / 2 * 1000000);

   printf(1, "TEST PASSED\n\n");
   exit();
}
