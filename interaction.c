#include "interaction.h"
#include <stdio.h>

void print_blockchain(Blockchain *bc) {
    printf("\nBlockchain:\n");
    Block *current = bc->head;
    while(current != NULL) {
        printf("Block %d [%s]\n", current->index, current->hash);
        printf("  Previous: %s\n", current->previous_hash);
        printf("  Transactions: %d\n", current->transaction_count);
        for(int i = 0; i < current->transaction_count; i++) {
            printf("    %s -> %s: %.2f\n", 
                   current->transactions[i].sender,
                   current->transactions[i].receiver,
                   current->transactions[i].amount);
        }
        current = current->next;
    }
}

void tamper_blockchain(Blockchain *bc, int block_index) {
    Block *current = bc->head;
    while(current != NULL && current->index != block_index) {
        current = current->next;
    }
    
    if(current != NULL) {
        printf("\nTampering with block %d...\n", block_index);
        current->transactions[0].amount *= 2;
        calculate_block_hash(current, current->hash);
    }
}