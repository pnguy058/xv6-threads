#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
//We are putting these functions in umalloc because
//the malloc and free functions are here.
//User level call for clone()
int
thread_create(void (*instruction)(void*), void *arg)
{
  int stack_address;
  void *stack = malloc(4096); //allocating a page size
  
  //page aligning
  if((uint)stack % 4096 != 0) //if not page aligned free and page align
  {
    free(stack);
    stack = malloc(2 * 4096);
    stack_address = (uint)stack;
    stack += (4096 - (uint)stack %  4096);
  }
  else
  {
    stack_address = (uint)stack;
  }
  ((uint*)stack)[0] = stack_address; //setting new stack address in case not page aligned
  //printf(1, "2nd malloc: %p = %d\n", stack, (uint)stack);
  
  //printf(1, "After initalization of page size in heap.\n");
  int pid = clone(instruction, arg, stack); // clone returns the PID
  //printf(1, "After pid is returned from clone.\n");
  return pid;
}
//user level call for join()
int
thread_join()
{
  void *stack = (void*)0; //null stack
  int pid = join(&stack);
  free(stack);
  return(pid);
}
