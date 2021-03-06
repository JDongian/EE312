/*
 * Project4.c 
 *
 * My Name
 * My Section Time
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */



/* Free the elements of the given set. */
void destroySet(Set* self) {
    free(self->elements);
}

/* Set elements to the null pointer and len to 0. */
void createEmptySet(Set* self) {
    self->len = 0;
    self->elements = 0;
    self->capacity = 0;
}

/* Initialize a set to be filled with one element only. */
void createSingletonSet(Set* self, int x) {
    self->elements = (int*) malloc(sizeof(int));
    self->elements[0] = x;
    self->len = 1;
    self->capacity = 1;
}

/* Copy the elements from the second set to the first set.
 * WARNING: Does no checking for capacity. */
void createCopySet(Set* self, const Set* other) {
    self->elements = (int*) malloc(other->len * sizeof(int));
    for (int k = 0; k < other->len; k += 1) {
        self->elements[k] = other->elements[k];
    }
    self->len = other->len;
    self->capacity = other->capacity;
}

/* Deallocates the set and copies the second set to the first. */
void assignSet(Set* self, const Set* other) {
    if (self == other) { return; }
    destroySet(self);
    createCopySet(self, other);
}

/* Return true if x is an element of self.
 * Otherwise, return false.
 */
bool isMemberSet(const Set* self, int x) {
    if (self->len == 0) {
        return false;
    }
    int min=0, mid, max=self->len;
    while (min < max) {
        mid = (min+max)/2;
        if (self->elements[mid] < x)
            min = mid+1;
        else
            max = mid;
    }
    return ((max == min) && (self->elements[min] == x));
}

/* Add x as a new member to this set. */
void insertSet(Set* self, int x) {
    if (self->len == 0) {
        createSingletonSet(self, x);
        return;
    }
    /* If x is already a member of the set,
     * then self should not be changed.
     */
    if (!isMemberSet(self, x)) {
        /* If there is no room left in the elements,
         * allocate double the length.
         */
        if (self->len+1 >= self->capacity) {
            self->capacity *= 2;
            int* new_elements = (int*) malloc(sizeof(int)*(2+(self->capacity)));
            int i = 0; //new_elements iterator
            int j = 0; //self->elements iterator
            /* Cleverly copy the elements */
            while (i <= (self->len)) {
                if ((i == j) && (self->len == 1) && (i == 0)) {
                    if (x < self->elements[j]) {
                        new_elements[i] = x;
                        new_elements[i+1] = self->elements[j];
                        i += 2; ++j;
                    } else {
                        new_elements[i] = self->elements[j];
                        ++i; ++j;
                    }
                    continue;
                }
                /* If we are at the end and we have not inserted x yet. */
                if ((i == j) && (i == self->len-1)) {
                    /* Copy the last element. */
                    new_elements[i] = self->elements[j];
                    /* Set the element after the last to x. */
                    new_elements[self->len] = x;
                    break;
                }
                /* If we are in the right spot to insert x. */
                if ((i == j) && (x < self->elements[j])) {
                    new_elements[i] = x;
                    new_elements[i+1] = self->elements[j];
                    /* Hop an extra time for the new element. */
                    i += 2; ++j;
                } else {
                    new_elements[i] = self->elements[j];
                    /* Iterate both arrays. */
                    ++i; ++j;
                }
            }
            printf("%d %d %d\n",
                    self->capacity, self->len, (self->elements)[0]);
            destroySet(self);
            /* Set the pointer of the copied array to the current array. */
            self->elements = new_elements;
            ++(self->len);
        } else {
            int min=0, mid, max=self->len;
            while (min < max) {
                mid = (min+max)/2;
                if (self->elements[mid] < x)
                    min = mid+1;
                else
                    max = mid;
            }
            int temp;
            int swap = self->elements[min];
            self->elements[min] = x;
            /* Rotate the remaining elements down. */
            for (int i = min+1; i < (self->len)+1; ++i) {
                temp = self->elements[i];
                self->elements[i] = swap;
                swap = temp;
            }
            ++(self->len);
        }
    }
}


/* Remove an element from the set. */
void removeSet(Set* self, int x) {
    if (self->len == 0) {
        return;
    }
    int min=0, mid, max=self->len;
    while (min < max) {
        mid = (min+max)/2;
        //assert(mid < max);
        if (self->elements[mid] < x)
            min = mid+1;
        else
            max = mid;
    }
    /* If the value was in the array, */
    if ((max == min) && (self->elements[min] == x)) {
        /* Rotate the elements down. */
        for (int i = min; i < (self->len)-1; ++i) {
            self->elements[i] = self->elements[i+1];
        }
        --(self->len);
    }
}

/* done for you already */
void displaySet(const Set* self) {
    int k;
    printf("{");
    if (self->len == 0) { 
        printf("}"); 
    } else {
        for (k = 0; k < self->len; k += 1) {
            if (k < self->len - 1) {
                printf("%d,", self->elements[k]);
            } else {
                printf("%d}", self->elements[k]);
            }
        }
    }
}

/* Return true if self and other have exactly the same elements.
 * Complexity: O(n)
 */
bool isEqualToSet(const Set* self, const Set* other) {
    if (self->len != other->len){ return false; }
    for (int i = 0; i < self->len; ++i) {
        if (self->elements[i] != other->elements[i]){
            return false;
        }
    }
    return true;
}

/* Return true if every element of self is also an element of other
 * Complexity: O(n)
 */
bool isSubsetOf(const Set* self, const Set* other) {
    if (self->len > other->len){ return false; }
    for (int i = 0; i < self->len; ++i) {
        if (isMemberSet(other, self->elements[i])) {
            return false;
        }
    }
    return true;
}

/* Check if the set is empty.
 * Complexity: O(1)
 */
bool isEmptySet(const Set* self) {
    return self->len == 0;
}

/* Remove all elements from self that are not also elements of other */
void intersectFromSet(Set* self, const Set* other) {
    for (int i = 0; i < other->len; ++i) {
        if (!isMemberSet(self, other->elements[i])) {
            removeSet(self, other->elements[i]);
        }
    }
}

/* Remove all elements from self that are also elements of other */
void subtractFromSet(Set* self, const Set* other) {
    for (int i = 0; i < other->len; ++i) {
        removeSet(self, other->elements[i]);
    }
}

/* Add all elements of other to self
 * (obviously, without creating duplicate elements) */
void unionInSet(Set* self, const Set* other) {
    for (int i = 0; i < other->len; ++i) {
        insertSet(self, other->elements[i]);
    }
}
