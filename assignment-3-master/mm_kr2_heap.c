/*
 * mm_kr2_heap.c
 *
 * Based on C dynamic memory manager code from
 * Brian Kernighan and Dennis Richie (K&R)
 *
 *  @since Mar 10, 2020
 *  @author Hongming Zhang & Junming Zhao
 */


#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include "memlib.h"
#include "mm_heap.h"


/** Allocation unit for header of memory blocks */
typedef union Header {          /* block header/footer */
    struct {
    	union Header *next;  /** next block if on free list */
    	union Header *prev;  /** prev block if on free list */
        size_t size;         /** size of this block including header */
                             /** measured in multiple of header size */
        bool isFree;      /** indicate if the block is free or not */
    } s;
    max_align_t _align;              		  // force alignment to max align boundary
} Header;

// forward declarations
static Header *morecore(size_t);
void visualize(const char*);

/** Start of free memory list */
static Header *freep = NULL;

/**
 * Initialize memory allocator
 */
void mm_init() {
	mem_init();

    if (mem_sbrk(sizeof(Header) * 3) == NULL) {
        return;
    }

    freep = mem_heap_lo();
    freep[0].s.size = freep[1].s.size = 2;
    freep[0].s.isFree = freep[1].s.isFree = 1;
    freep[2].s.isFree = 1;
    freep[0].s.prev = freep[0].s.next = freep;
}

/**
 * Reset memory allocator.
 */
void mm_reset(void) {
	mem_reset_brk();

    if (mem_sbrk(sizeof(Header) * 3) == NULL) {
        return;
    }

    freep = mem_heap_lo();
    freep[0].s.size = freep[1].s.size = 2;
    freep[0].s.isFree = freep[1].s.isFree = 1;
    freep[2].s.isFree = 1;
    freep[0].s.prev = freep[0].s.next = freep;
}

/**
 * De-initialize memory allocator.
 */
void mm_deinit(void) {
    mem_deinit();

    freep = NULL;
}

/**
 * Allocation units for nbytes bytes.
 *
 * @param nbytes number of bytes
 * @return number of units for nbytes
 */
inline static size_t mm_units(size_t nbytes) {
    /* smallest count of Header-sized memory chunks */
    /*  (+2 additional chunk for the Header itself) needed to hold nbytes */
    return (nbytes + sizeof(Header) - 1) / sizeof(Header) + 2;
}

/**
 * Allocation bytes for nunits allocation units.
 *
 * @param nunits number of units
 * @return number of bytes for nunits
 */
inline static size_t mm_bytes(size_t nunits) {
    return nunits * sizeof(Header);
}

/**
 * Get pointer to block payload.
 *
 * @param bp the block
 * @return pointer to allocated payload
 */
inline static void *mm_payload(Header *bp) {
	return bp + 1;
}

/**
 * Get pointer to block for payload.
 *
 * @param ap the allocated payload pointer
 */
inline static Header *mm_block(void *ap) {
	return (Header*)ap - 1;
}



/**
 * Allocates size bytes of memory and returns a pointer to the
 * allocated memory, or NULL if request storage cannot be allocated.
 *
 * @param nbytes the number of bytes to allocate
 * @return pointer to allocated memory or NULL if not available.
 */
void *mm_malloc(size_t nbytes) {
    if (freep == NULL) {
    	mm_init();
    }

    Header * bp = freep;

    // smallest count of Header-sized memory chunks
    //  (+2 additional chunk for the Header itself) needed to hold nbytes
    size_t nunits = mm_units(nbytes);

    // traverse the circular list to find a block
    while (freep != NULL) {
     	if ((bp[0].s.isFree == 0) && (bp[0].s.size >= nunits)) {
            if (bp->s.size < (nunits + 2)) {          /* found block large enough */
               	// free block exact size
               	// move the head

                // no longer on free list
               	bp[0].s.isFree = bp[bp[0].s.size - 1].s.isFree = 1;
                bp[0].s.prev->s.next = bp[0].s.next;
                bp[0].s.next->s.prev = bp[0].s.prev;
            } else {
            	// split and allocate tail end
               	// Calculate the remaining size on the free list
              	size_t remainSize = bp[0].s.size - nunits;
                bp[remainSize - 1].s.size = bp[0].s.size -=	nunits;
                bp[remainSize].s.size = nunits;
                bp[remainSize + nunits - 1].s.size = nunits;

                // No longer on the free list
                bp[remainSize].s.isFree = bp[remainSize + nunits - 1].s.isFree = 1;
                // Return this part
                bp = bp + remainSize;
            }
            // move the head
            if (freep == bp) {
                freep = bp[0].s.prev;
            }

            return mm_payload(bp);
        }
        bp = bp[0].s.next;

        /* back where we started and nothing found - we need to allocate */
        if (bp == freep) {                    /* wrapped around free list */
            bp = morecore(nunits);
            if (bp == NULL) {
            	errno = ENOMEM;
                return NULL;                /* none left */
            }
        }
    }

    assert(false);  // shouldn't happen
    return NULL;
}


/**
 * Deallocates the memory allocation pointed to by ap.
 * If ap is a NULL pointer, no operation is performed.
 *
 * @param ap the memory to free
 */
void mm_free(void *ap) {
	// ignore null pointer
	if (ap == NULL){
		return;
	}

	Header * bp = mm_block(ap);   /* point to block header */

    // validate size field of header block
    assert(bp->s.size > 0 && mm_bytes(bp->s.size) <= mem_heapsize());

    // find where to insert the free space
    size_t nunits = bp->s.size;

    if (bp[-1].s.isFree != 0) {
    	// link in before upper block
        bp[0].s.prev = freep;
        bp[0].s.next =  freep[0].s.next;
        freep[0].s.next = freep[0].s.next->s.prev = bp;
    } else  {
    	// coalesce if adjacent to upper neighbor
        bp = bp - bp[-1].s.size;
        nunits = nunits + bp[0].s.size;
        bp[0].s.size = bp[nunits-1].s.size = nunits;
    }

    if (bp[nunits].s.isFree == 0) {
        // coalesce if adjacent to lower block
        bp[nunits].s.prev->s.next = bp[nunits].s.next;
        bp[nunits].s.next->s.prev = bp[nunits].s.prev;

        nunits += bp[nunits].s.size;
        bp[0].s.size = bp[nunits-1].s.size = nunits;
    }

    bp[0].s.isFree = bp[nunits-1].s.isFree = 0;

    /* reset the start of the free list */
    freep = bp;
}

/**
 * Tries to change the size of the allocation pointed to by ap
 * to size, and returns ap.
 *
 * If there is not enough room to enlarge the memory allocation
 * pointed to by ap, realloc() creates a new allocation, copies
 * as much of the old data pointed to by ptr as will fit to the
 * new allocation, frees the old allocation, and returns a pointer
 * to the allocated memory.
 *
 * If ap is NULL, realloc() is identical to a call to malloc()
 * for size bytes.  If size is zero and ptr is not NULL, a minimum
 * sized object is allocated and the original object is freed.
 *
 * @param ap pointer to allocated memory
 * @param newsize required new memory size in bytes
 * @return pointer to allocated memory at least required size
 *	with original content
 */
void* mm_realloc(void *ap, size_t newsize) {
	// NULL ap acts as malloc for size newsize bytes
	if (ap == NULL) {
		return mm_malloc(newsize);
	}

	Header * bp = mm_block(ap);    // point to block header
	if (newsize > 0) {
		// return this ap if allocated block large enough
		if (bp->s.size >= mm_units(newsize)) {
			return ap;
		}
	}

    // allocate new block
    void *newap = mm_malloc(newsize);
    if (newap == NULL) {
        return NULL;
    }

    // copy old block to new block
    size_t oldsize = mm_bytes(bp->s.size - 2);
    memcpy(newap, ap, (oldsize < newsize) ? oldsize : newsize);
    mm_free(ap);
    return newap;
}

/**
 * Request additional memory to be added to this process.
 *
 * @param nu the number of Header units to be added
 * @return pointer to start additional memory added
 */
static Header *morecore(size_t nu) {
	// nalloc based on page size
	size_t nalloc = mem_pagesize()/sizeof(Header);

	/* get at least NALLOC Header-chunks from the OS */
    if (nu < nalloc) {
    	nu = nalloc;
    }

    size_t nbytes = mm_bytes(nu); // number of bytes
    void *p = mem_sbrk(nbytes);
    if (p == (char *) -1) {	// no space
        return NULL;
    }

    Header *bp = (Header*)p;
    bp->s.size = bp[nu - 1].s.size = nu;
    bp->s.isFree = bp[nu - 1].s.isFree = 0;
	bp[nu].s.isFree = 1;

	// add new space to the circular list
	mm_free(bp+1);

    return freep;
}

/**
 * Print the free list (debugging only)
 *
 * @msg the initial message to print
 */
void visualize(const char* msg) {
    fprintf(stderr, "\n--- Free list after \"%s\":\n", msg);

    if (freep == NULL) {                   /* does not exist */
        fprintf(stderr, "    List does not exist\n\n");
        return;
    }

    if (freep == freep[0].s.prev) {          /* self-pointing list = empty */
        fprintf(stderr, "    List is empty\n\n");
        return;
    }

    Header *tmp = freep;                           // find the start of the list
    char* str = "    ";
    do {           // traverse the list
        fprintf(stderr, "%sptr: %10p size: %-3lu blks - %-5lu bytes\n",
            str, (void *)tmp, tmp->s.size, mm_bytes(tmp->s.size));
        str = " -> ";
        tmp = tmp[0].s.next;
    }  while (tmp != freep);
/*
    char* str = "    ";
    for (Header *p = base.s.ptr; p != &base; p = p->s.ptr) {
        fprintf(stderr, "%sptr: %10p size: %3lu blks - %5lu bytes\n",
            str, (void *)p, p->s.size, mm_bytes(p->s.size));
        str = " -> ";
    }
 */
    fprintf(stderr, "--- end\n\n");
}

/**
 * Calculate the total amount of available free memory.
 *
 * @return the amount of free memory in bytes
 */
size_t mm_getfree(void) {
    if (freep == NULL) {
        return 0;
    }
    
    // point to head of free list
    Header *tmp = freep;
    size_t res = tmp->s.size;
    
    // scan free list and count available memory
    while (tmp[0].s.next > tmp) {
        tmp = tmp->s.next;
        if(tmp->s.isFree == 0){
        	res += tmp->s.size;
        }
    }
    
    // convert header units to bytes
    return mm_bytes(res);
}
