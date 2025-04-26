#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <pthread.h>
#include <openssl/evp.h>
#include <time.h>

#define MAX_TRANSACTIONS 10
#define HASH_LENGTH 65

typedef struct {
    char sender[50];
    char receiver[50];
    double amount;
    time_t timestamp;
} Transaction;

typedef struct Block {
    int index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    char previous_hash[HASH_LENGTH];
    char hash[HASH_LENGTH];
    int nonce;
    struct Block *next;
} Block;

typedef struct {
    Block *head;
    Block *tail;
    int difficulty;
    pthread_mutex_t lock;
} Blockchain;

// Structure pour passer des données aux threads
typedef struct {
    Blockchain *bc;
    Block *new_block;
} ThreadData;

void init_blockchain(Blockchain *bc, int difficulty);
void calculate_hash(const char *str, char output[HASH_LENGTH]);
char* calculate_merkle_root(Transaction transactions[], int transaction_count);
void calculate_block_hash(Block *block, char output[HASH_LENGTH]);

#endif