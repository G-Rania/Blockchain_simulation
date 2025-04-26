#include <stdio.h>
#include <time.h>
#include "blockchain.h"
#include "replication.h"
#include "consensus.h"
#include "interaction.h"

int main() {
    Blockchain bc;
    init_blockchain(&bc, 4); // Difficulty of 4 leading zeros
    
    // Create some transactions
    Transaction txs1[2] = {
        {"Alice", "Bob", 10.5, time(NULL)},
        {"Bob", "Charlie", 5.2, time(NULL)}
    };
    
    Transaction txs2[1] = {
        {"Charlie", "Alice", 3.7, time(NULL)}
    };
    
    // Add blocks to the blockchain
    add_block(&bc, txs1, 2);
    add_block(&bc, txs2, 1);
    
    // Print the blockchain
    print_blockchain(&bc);
    
    // Validate the blockchain
    printf("\nBlockchain valid? %s\n", is_blockchain_valid(&bc) ? "Yes" : "No");
    
    // Try to tamper with the blockchain
    tamper_blockchain(&bc, 1);
    printf("After tampering, blockchain valid? %s\n", is_blockchain_valid(&bc) ? "Yes" : "No");
    
    return 0;
}