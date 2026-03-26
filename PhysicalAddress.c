#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <VAS in MB> <Page Size in KB> <Virtual Address>\n", argv[0]);
        return 1;
    }

    int vas_mb = atoi(argv[1]);
    int page_kb = atoi(argv[2]);
    int virtual_address = atoi(argv[3]);

    // Convert to bytes
    int vas = vas_mb * 1024 * 1024;
    int page_size = page_kb * 1024;

    int num_pages = vas / page_size;

    // Calculate page number and offset
    int page_number = virtual_address / page_size;
    int offset = virtual_address % page_size;

    // Simulated page table (identity mapping)
    int page_table[num_pages];
    for (int i = 0; i < num_pages; i++) {
        page_table[i] = i; // simple mapping
    }

    // Check for page table miss
    if (page_number >= num_pages) {
        printf("Page table miss!\n");
        return 1;
    }

    int frame_number = page_table[page_number];

    printf("Physical Address = <%d, %d>\n", frame_number, offset);

    return 0;
}