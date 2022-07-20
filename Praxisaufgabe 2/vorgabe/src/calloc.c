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

    mem_block *current = last_allocation;
    mem_block *newblock;
    unsigned long actualSize = nmemb * size;
    unsigned long roundedSize = actualSize - actualSize%8 + 8;
    unsigned long sizeNewBlock;

    while (current != NULL) {
        sizeNewBlock = current->size - roundedSize - sizeof(mem_block);
        if (current->size % 2 == 1) {
            current = current->next;
        } else if (current->size == roundedSize) {
            current->size = current->size|1;
            memset(current+1, c, roundedSize);
            last_allocation = current;
            return (current + 1);
        } else if (current->size > roundedSize && current->size < roundedSize+sizeof(mem_block)+8) {
            current->size = roundedSize;
            current->size = current->size|1;
            memset(current+1, c, roundedSize);
            last_allocation = current;
            return (current + 1);
        } else if (current->size > roundedSize) {
            newblock = current + sizeof(mem_block) + roundedSize;
            newblock->prev = current;
            newblock->next = current->next;
            newblock->size = sizeNewBlock;
            current->size = roundedSize;
            current->size = current->size|1;
            memset(current+1, c, roundedSize);
            current->next = newblock;
            last_allocation = current;
            return (current + 1);
        }
    }

    last_allocation = current;
    return (current + 1);
}

void my_free(void *ptr){
	// TODO

    mem_block *current = ptr-sizeof(mem_block);
    mem_block *next = current->next;
    mem_block *prev = current->prev;

    if (current->size % 2 == 1) {
        current->size--;
        if (last_allocation == current) {
            last_allocation = prev;
        }
    }

    if (next != NULL && next->size % 2 == 0 && next->size != 0) {
        current->size += next->size + sizeof(mem_block);
        current->next = next->next;
        if (last_allocation == next) {
            last_allocation = current;
        }
        if (next->next != NULL) {
            if (next->next->prev != NULL) {
                next->next->prev = current;
            }
        }
        next->size = 0;
        next->prev = NULL;
        next->next = NULL;
    }
    if (prev != NULL && prev->size % 2 == 0 && prev->size != 0) {
        prev->size += current->size + sizeof(mem_block);
        prev->next = current->next;
        if (last_allocation == current) {
            last_allocation = prev;
        }
        if (current->next != NULL) {
            if (current->next->prev != NULL) {
                current->next->prev = prev;
            }
        }
        current->size = 0;
        current->prev = NULL;
        current->next = NULL;
    }
}
