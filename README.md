# Kernel-level threads in xv6

Our project consisted of implementing new system calls, `clone()` and `join()`, in xv6 which allowed us to create and wait for kernel level threads. We then build a thread libary by using `clone()` and implementing a simple ticket lock with `lock()` and `unlock()`.
To due so we followed the instrunctions from CS153's Textbook author Remzi H. Arpaci-Dusseau in his Kernel Threads project repo. (https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-xv6-threads). 

## Building with Docker
REQ: Docker Desktop installed and the `Dockerfile` in this repo downloaded. Skip to #5 to run on local cloned repo.
1. `docker build -t xv6 .` Installs the requirements and clones the repo to create a docker image
2. (To run auto test script) `docker run --rm -it xv6` Will execute the `entrypoint.sh` script which makes, and runs qemu-nox and auto tests `test_lock2`. This script was created for travis and isn't reccomended to test manually. Skip to #3
3. `docker run --rm -it xv6 bash` Opens docker with workdir set to the cloned repo
4. `cd kernel` -> `make` -> `make qemu-nox` Opens qemu in terminal. From there you can execute `ls` to view avaible commands or run the test scripts mentioned below.
5. If you wish to run docker using the --volume tag, clone this repo -  `git clone https://github.com/pnguy058/xv6-threads.git`, and change terminal directory to it, copy the contents of `localDockerfile` to `Dockerfile`, then in console type `docker build -t xv6 .` and `docker run --rm -it xv6`. This will create image based on local /kernel files that you can make changes to.

## Clone

We implemented the clone function call in the form of `clone(void(*func)(void*), void *arg, void *stack)`. `func` is the address passed in in which the thread will start executing at. The `arg` that is passed in will be put into our user stack. The `stack` is what the process uses as its new user stack which is passed in with a fake return PC and our `arg`. This function is a heavily modified version of `fork()` which handles processes.

## Join
We then implemented a new system call which is `int join(void** stack)`. This call waits for a child thread of a process that it shares the address space with to exit. It returns the PID of the waited-for child or -1 if there is no child. The location of the thread's user stack is then copied into the argument `stack` which can then be freed later on inside of our user level `thread_join()` function. The variable `tstack_address` holds this variable. This function follows that of `wait()`.

## Umalloc.c file
We placed two major user level function calls in here that calls `clone()` and `join()`. `thread_create()` and `thread_join` were placed inside of the umalloc.c file in order to access the `malloc()` function (allocating space in the heap for each new thread created) and the `free()` function (to free the stack allocated for the thread after its finished with it). The `thread_create()` function made sure that the page allocated by `malloc()` is page aligned. It also made sure to change the stack address of the allocated stack in case a change is needed.

## Implementing Spinlocks
We implemented two types of spinlocks. The first a ticket lock using FetchAndAdd , and the second a simple spinlock using TestAndSet.
To begin we added a `lock_t` struct in `types.h`. The struct held three `uint` data fields: `ticket`, `turn`, and `flag`. The first two used by FetchAndAdd and the last by TestAndSet. In `ulib.c` we added three lock functions, `lock_init()` which initilizes the stuct data fields, `lock()` where the atomic instructions in `x86.h` are called, and `unlock()` which releases the lock as the name implies.

## Test Files
There are four test scripts in total. 
`test_thread.c` simply creates a signle thread using `thread_create`, has the thread execute a simple increment of a global variable, exits, and then calls `thread_join` to cleanup.

`thread_many.c` tests the creation/joining of multiple threads. This function takes a single pararamater as input that tells the program how may threads to create. Note - creating over 61 threads will cause the program to fault

`test_lock` and `test_lock2` are nearly identical. The former test doesn't have spinlocks around the global variable each thread edits, causing the final value to be inncorrect and the test to fail. The latter adds spinlocks and the test passes.
