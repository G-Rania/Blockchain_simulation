#ifndef CONSENSUS_H
#define CONSENSUS_H

#include "blockchain.h"

int is_blockchain_valid(Blockchain *bc);
int validate_transactions(Transaction transactions[], int count);

#endif