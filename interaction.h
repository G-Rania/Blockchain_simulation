#ifndef INTERACTION_H
#define INTERACTION_H

#include "blockchain.h"

void print_blockchain(Blockchain *bc);
void tamper_blockchain(Blockchain *bc, int block_index);

#endif