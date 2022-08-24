# Min Heap Implementation

For the coding part of the assignment, you’ll be implementing min-heaps using arrays as discussed
in  class. The  relevant  files  in  the  starter  code  for  this  section  are  minheap.h,  minheap.c  and
test_minheap.c. First look  at minheap.h to see how the data structures are stored for the min-heap
implementation and understand what’s going on.  In particular, we are having to modify our structure
to  support  fast  decrease_priority()  operations.  The  header  file  explains  what  these  changes
are. Once you are familiar with the data layout, open minheap.c and complete the functions there
to  manipulate  the  heap.  Be  thoughtful  about  how  you  write  a  code,  writing  some  common  helper
functions can greatly reduce the amount of work you need to do. Lastly, the test minheap.c file provides some basic tests for your implementation. These are
not comprehensive, and you should test for correctness yourself by adding more thorough test cases
and  tracing  portions  of  your  algorithm  by  hand  to  compare.  You  will  need  to  use  your  min-heap
implementation  for  the  next  section  of  the  assignment,  so  make  sure  that  your  implementation  is
correct before you begin. You don’t want to be debugging multiple different parts of your code when
something goes wrong.