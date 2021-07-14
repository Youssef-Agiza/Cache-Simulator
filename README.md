# Cache-Simulator

## Implementation Details

1. Replacement Policies

   - **Least Recently Used(LRU) policy idea:**

     Every set in the cache will have a member "leastUsed" which will increase every time a set is accessed but the memory line is not used. That is, "least used" indicates how much recently the memory line was **not** accessed. th For example, assume we have 4-way sets and we accessed the second block in set number 5. Now, all the blocks except the second have not been "recently used," thus, we increase the leastUsed value for all the block except the second block. Then, to find the memory line to be replaced (least recently used), we loop over all the block in the set and choose the one with the _largest_ value. Bottom line is: leastUsed increases when the line was accessed longer time ago and decreases the more recently it was accessed.

   - **Least Frequenctly Used(LFU) policy idea:**

     Every set block stores a counter(frequency). Every time a memory line is accessed, its frequency increases. Then, to find the line to be replaced(least frequently used), we loop over all the blocks in the set and choose the one with the _smallest_ value.

## Limitations

1. Code optimization

   In this project we didn't focus mainly on optimizing the code but rather on simulating the caching proccess correctly. Thus, some code parts are not optimized, some variables are allocated even if they are not needed, some loops are written explicitly more than ones instead of encapsulating them in a function. This can be further optimized but it would an overkill for this project so we prefered to keep it simple.
