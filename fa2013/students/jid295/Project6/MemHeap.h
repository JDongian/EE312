#ifndef _MemHeap_h
#define _MemHeap_h 1

void initializeHeap(void);
void* allocateMemory(unsigned size);
void deallocateMemory(void* ptr);
void displayChunks(void);
bool isEmptyHeap(void);
bool isSaneHeap(void);

inline void* operator new(size_t sz) { return allocateMemory(sz); }
inline void* operator new[](size_t sz) { return allocateMemory(sz); }
inline void operator delete(void* p) { deallocateMemory(p); }
inline void operator delete[](void* p) { deallocateMemory(p); }

#endif /* _MemHeap_h */
