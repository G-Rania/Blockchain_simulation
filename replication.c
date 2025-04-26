#include "blockchain.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* replicate_block(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    Blockchain *bc = data->bc;
    
    Block *thread_block = (Block *)malloc(sizeof(Block));
    memcpy(thread_block, data->new_block, sizeof(Block));
    
    char target[bc->difficulty + 1];
    memset(target, '0', bc->difficulty);
    target[bc->difficulty] = '\0';
    
    while(strncmp(thread_block->hash, target, bc->difficulty) != 0) {
        thread_block->nonce++;
        calculate_block_hash(thread_block, thread_block->hash);
    }
    
    pthread_mutex_lock(&bc->lock);
    if(bc->tail->index < thread_block->index) {
        bc->tail->next = thread_block;
        bc->tail = thread_block;
        printf("Thread %lu mined block %d\n", pthread_self(), thread_block->index);
        pthread_mutex_unlock(&bc->lock);
    } else {
        pthread_mutex_unlock(&bc->lock);
        free(thread_block);
    }
    
    free(data);
    return NULL;
}

void add_block(Blockchain *bc, Transaction transactions[], int count) {
    Block *new_block_template = (Block *)malloc(sizeof(Block));
    // Initialisation du template
    new_block_template->index = bc->tail->index + 1;
    new_block_template->timestamp = time(NULL);
    memcpy(new_block_template->transactions, transactions, sizeof(Transaction) * count);
    new_block_template->transaction_count = count;
    strcpy(new_block_template->previous_hash, bc->tail->hash);
    new_block_template->nonce = 0;
    calculate_block_hash(new_block_template, new_block_template->hash);
    new_block_template->next = NULL;
    
    pthread_t threads[3]; 
    
    for(int i = 0; i < 3; i++) {
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
        data->bc = bc;
        data->new_block = new_block_template;
        pthread_create(&threads[i], NULL, replicate_block, data);
    }
    
    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    free(new_block_template);
}