#ifndef BITCOIN_SERVER_H
#define BITCOIN_SERVER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "lib/entities.h"
#include "lib/utils.h"

int numberOfConnections = 0;

void generateInitBlock();
int isLegalBlock(BLOCK_T *serverBlock);
void checkAndUpdateBlockChainHead(mqd_t *newBlock_mq);
int reallocMinersMQ(mqd_t ** minersMQ, int newSize);

#endif //BITCOIN_SERVER_H
