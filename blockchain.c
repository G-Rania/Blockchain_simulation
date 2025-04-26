#include "blockchain.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_blockchain(Blockchain *bc, int difficulty) {
    bc->head = NULL;
    bc->tail = NULL;
    bc->difficulty = difficulty;
    pthread_mutex_init(&bc->lock, NULL);
    
    Block *genesis = (Block *)malloc(sizeof(Block));
    genesis->index = 0;
    genesis->timestamp = time(NULL);
    genesis->transaction_count = 0;
    strcpy(genesis->previous_hash, "0");
    genesis->nonce = 0;
    calculate_block_hash(genesis, genesis->hash);
    genesis->next = NULL;
    
    bc->head = genesis;
    bc->tail = genesis;
}

void calculate_hash(const char *str, char output[HASH_LENGTH]) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, str, strlen(str));
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    for(unsigned int i = 0; i < hash_len; i++) { // Changé int en unsigned int
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[HASH_LENGTH-1] = '\0';
}

char* calculate_merkle_root(Transaction transactions[], int transaction_count) {
    if (transaction_count == 0) return NULL;

    char hashes[transaction_count][HASH_LENGTH];
    for (int i = 0; i < transaction_count; i++) {
        char tx_data[256];
        sprintf(tx_data, "%s%s%.2f%ld", transactions[i].sender, transactions[i].receiver, transactions[i].amount, transactions[i].timestamp);
        calculate_hash(tx_data, hashes[i]);
    }

    while (transaction_count > 1) {
        int new_count = (transaction_count + 1) / 2;
        for (int i = 0; i < new_count; i++) {
            char combined[2 * HASH_LENGTH];
            strcpy(combined, hashes[2 * i]);
            if (2 * i + 1 < transaction_count) {
                strcat(combined, hashes[2 * i + 1]);
            }
            calculate_hash(combined, hashes[i]);
        }
        transaction_count = new_count;
    }

    char* merkle_root = (char*)malloc(HASH_LENGTH);
    strcpy(merkle_root, hashes[0]);
    return merkle_root;
}

void calculate_block_hash(Block *block, char output[HASH_LENGTH]) {
    if (!block || !output) {
        fprintf(stderr, "Erreur: pointeur nul dans calculate_block_hash\n");
        return;
    }

    // Calcul du Merkle Root pour les transactions
    char *merkle_root = calculate_merkle_root(block->transactions, block->transaction_count);
    if (!merkle_root) {
        fprintf(stderr, "Erreur: Merkle Root non calculé\n");
        return;
    }

    // Préparation des données pour le hash du bloc
    char data[4096];
    sprintf(data, "%d%ld%s%d%s", block->index, block->timestamp, merkle_root, block->nonce, block->previous_hash);

    // Calcul du hash du bloc
    calculate_hash(data, output);

    // Libération de la mémoire allouée pour le Merkle Root
    free(merkle_root);
}