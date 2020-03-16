# Kernel-level threads in xv6

Our project will consist of implementing new `clone()` and `join()` system calls to the xv6 which will allow us to create kernel level threads. 

## Clone

We implemented the clone function call in the form of `clone(void(*func)(void*), void *arg, void *stack)`. `func` is the address passed in in which the thread will start executing at. The `arg` that is passed in will be put into our user stack. The `stack` is what the process uses as its new user stack which is passed in with a fake return PC and our `arg`. This function is a heavily modified version of `fork()` which handles processes.

## Join
We then implemented a new system call which is `int join(void** stack)`. This call waits for a child thread of a process that it shares the address space with to exit. It returns the PID of the waited-for child or -1 if there is no child. The location of the thread's user stack is then copied into the argument `stack` which can then be freed later on inside of our user level `thread_join()` function. The variable `tstack_address` holds this variable. This function follows that of `wait()`.

## Umalloc.c file
We placed two major user level function calls in here that calls `clone()` and `join()`. `thread_create()` and `thread_join` were placed inside of the umalloc.c file in order to access the `malloc()` function (allocating space in the heap for each new thread created) and the `free()` function (to free the stack allocated for the thread after its finished with it). The `thread_create()` function made sure that the page allocated by `malloc()` is page aligned. It also made sure to change the stack address of the allocated stack in case a change is needed.
