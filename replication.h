#ifndef REPLICATION_H
#define REPLICATION_H

#include "blockchain.h"

void* replicate_block(void *arg);
void add_block(Blockchain *bc, Transaction transactions[], int count);

#endif