# ass3_dsa_202
Cache Implementation
In the previous assignment, a cache is implemented using AVL tree for searching
and a queue for FIFO replacement policy. Actually, there are many different ap-
proaches for searching and replacement policies. Every approach has advantages
and disvantages. In this assignment, you are required to implement different
searching and replacement policies. In addition, these approaches must be com-
bined in a system so that the cache can be applied with different combinations.
For searching, this assignment requires two different approaches: AVL tree
and hashing while for replacement, there are four different policies: Least re-
cently used (LRU), Most recently used (MRU), Least frequently used (LFU)
and First In First Out (FIFO). These policies must be implemented as different
classes. When creating a cache, it must be passed an object for searching and
another object for replacement policy. The details will be described in next
section.
