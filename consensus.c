#include "consensus.h"
#include <string.h>

int is_blockchain_valid(Blockchain *bc) {
    if(bc->head == NULL) return 1;
    
    Block *current = bc->head->next;
    Block *previous = bc->head;
    
    while(current != NULL) {
        if(strcmp(current->previous_hash, previous->hash) != 0) {
            return 0;
        }
        
        char calculated_hash[HASH_LENGTH];
        calculate_block_hash(current, calculated_hash);
        if(strcmp(current->hash, calculated_hash) != 0) {
            return 0;
        }
        
        previous = current;
        current = current->next;
    }
    
    return 1;
}

int validate_transactions(Transaction transactions[], int count) {
    for(int i = 0; i < count; i++) {
        if(transactions[i].amount <= 0) {
            return 0;
        }
        if(strlen(transactions[i].sender) == 0 || strlen(transactions[i].receiver) == 0) {
            return 0;
        }
    }
    return 1;
}