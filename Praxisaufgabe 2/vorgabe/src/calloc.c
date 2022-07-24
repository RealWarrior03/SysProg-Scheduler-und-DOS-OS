#include "../include/calloc.h"
// Für memset
#include <string.h>

static void * MEM;
static size_t MEM_SIZE;

/* Die aktuelle Position für das Next Fit Verfahren */
static mem_block * last_allocation = NULL;

void my_calloc_init(void * mem, size_t mem_size){
	MEM = mem;
	MEM_SIZE = mem_size;

	/* Initial existiert genau ein mem_block direkt am Anfang des Speichers */
	mem_block * beginning = MEM;

	beginning->next = NULL;
	beginning->prev = NULL;

	/* Der verwaltete Speicher des initialen mem_blocks ist die des
	 * gesamten Speichers minus die Größe seiner eigenen Verwaltungsstruktur
	 * Da sowohl MEM_SIZE ein Vielfaches von 8 ist und sizeof(mem_block) auch
	 * mindestens ein vielfaches von 8 oder mehr ist, ist das LSB
	 * auch direkt 0 -> free.
	 */
	beginning->size = MEM_SIZE - sizeof(mem_block);

	/* Unser Zeiger für das Next Fit Verfahren */
	last_allocation = beginning;
}

/* +------------------------------------+ *
 * | Hier ist Platz für Hilfsfunktionen | *
 * +------------------------------------+ */



/* -------------------------------------- */

void * my_calloc(size_t nmemb, size_t size, int c) {
	// TODO

    mem_block *current = last_allocation; //position in the chain
    unsigned long actualSize = nmemb * size; //size the new calloc requests
    unsigned long roundedSize = actualSize; //set this as the rounded value
    if (actualSize % 8 != 0) { //and if the "rounded" value isn't %8=0
        roundedSize = actualSize - actualSize%8 + 8; //than round it up to the next number that is %8=0
    }
    int loop = 0; //indicator for the second round

    while (current != last_allocation || loop == 0) {
        if (current->size % 2 == 0) { //only check the capacity, if the current block is free
            if (current->size == roundedSize) { //capacity fits perfectly
                current->size = current->size|1; //mark as used
                memset(current+1, c, roundedSize); //fill storage
                last_allocation = current; //mark last_allocation
                return (current + 1); //return
            } else if (current->size > roundedSize && current->size < roundedSize+sizeof(mem_block)+8) { //capacity is just a little bit too big
                current->size = roundedSize; //set
                current->size = current->size|1; //mark as used
                memset(current+1, c, roundedSize); //fill storage
                last_allocation = current; //mark last_allocation
                return (current + 1); //return
            } else if (current->size >= roundedSize+sizeof(mem_block)+8) { //capacity is larger; block needs to be split into two
                unsigned long sizeNewBlock = current->size - roundedSize - sizeof(mem_block); //get the size for the new block
                char * blockPointer = (char *) current + sizeof(mem_block) + roundedSize; //create the new block
                mem_block *newblock = (mem_block *) blockPointer; //mark its pointer as a mem_block
                newblock->prev = current; //put it behind current
                newblock->next = current->next; //and mark the next block
                if (newblock->next != NULL) { //if there is a following block
                    newblock->next->prev = newblock; //set its previous pointer to the new block
                }
                newblock->size = sizeNewBlock; //set the size to the calculated one
                if ((newblock->size & 1) == 1) { //if the new block for some reason is used
                    newblock->size--; //mark it as free
                }
                current->size = roundedSize; //change the size of the current block
                current->size = current->size|1; //and mark it as used
                memset(current+1, c, roundedSize); //fill storage
                current->next = newblock; //and set the new block as the next after current
                last_allocation = current; //mark last_allocation
                return (current + 1); //return
            }
        }

        if (current->next != NULL) { //if there is another block coming
            current = current->next; //move there
        } else { //otherwise
            current = MEM; //move to the beginning
            loop = 1; //and mark it for the loop
        }
    }

    return NULL;
}

void my_free(void *ptr){
	// TODO

    mem_block *current = ptr-sizeof(mem_block);
    mem_block *next = current->next;
    mem_block *prev = current->prev;

    if ((current->size & 1) == 1) { //if the block is marked as used, set it as free
        current->size--;
    }
    if (next != NULL) { //if the next block exists
        if ((next->size & 1) == 0) { //and is free
            if (last_allocation == next) { //if its the last_allocation
                last_allocation = current; //move it to current
            }
            current->size += next->size + sizeof(mem_block); //expand the current size
            if (next->next != NULL) { //if the next of next exists
                next->next->prev = current; //mark current as its prev
            }
            current->next = next->next; //and mark next->next as the next of current
        }
    }
    if (prev != NULL) { //if prev exists
        if ((prev->size & 1) == 0) { //and is free
            if (last_allocation == current) { //if current is the last_allocation
                last_allocation = prev; //set it to prev
            }
            prev->size += current->size + sizeof(mem_block); //extend prev size
            prev->next = current->next; //move the next pointer
            if (current->next != NULL) { //if there is a next after current
                current->next->prev = prev; //set its prev to prev
            }
        }
    }
}
