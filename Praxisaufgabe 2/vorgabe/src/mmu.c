#include "../include/mmu.h"

/* Memory start address: Startadresse unseres 12 bit Adressraumes */
static void *mem_start_addr;

/* page table base register = Seitentabellenbasisregister */
static addr_t *ptbr;

/* +------------------------------------+ *
 * | Hier ist Platz für Hilfsfunktionen | *
 * +------------------------------------+ */

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            if (j % 4 == 3) {
                printf(".");
            }
            printf("%u", byte);
        }
    }
    puts("");
}


/* -------------------------------------- */

void mmu_init(void* mem)
{
	mem_start_addr = mem;
	ptbr = mem;
}

int switch_process(int proc_id)
{
    if (proc_id < PROC_NUM && proc_id >= 0) {
        ptbr = (addr_t *) mem_start_addr + (PT_AMOUNT * proc_id);
        return 0;
    }
    return 1;
}

addr_t mmu_translate(addr_t va, req_type req)
{
    addr_t physaddr = MY_NULL;
    addr_t pagenum = (va >> 8); //get page num from va
    addr_t offset = va & 255; //get offset from va

    pte* page = (pte *) ptbr + pagenum; //get page from pagetable
    addr_t actions = page->offset & 7; //get read-write rights for page

    if ((req & actions) == req) { //check if requested rights are given
        if ((page->info & 8) == 8) { //check if page is present
            page->info = (page->info | 4); //mark page as accessed
            if ((req & 2) == 2) { //check if the process is writing
                page->info = page->info | 2; //mark the page as modified
            }
            physaddr = (page->page_num << 8) | offset; //get the physical address by merging the offset behind the pagenumber
        }
    }

    return physaddr;
}

addr_t mmu_check_request(request r)
{
    switch_process(r.p_num);
    return mmu_translate(r.addr, r.type);
}
