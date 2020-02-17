#include "miner.h"

void main(){
    float counter = 0;
    struct mq_attr mqAttr = {0};
    BLOCK_T *minerBlock = NULL;
    BLOCK_T *newBlock = NULL;
    MSG_T* msg;

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);


    for(;;)
    {
        /* Check if there is place in the Q, if yes increment send, if no print error and try again */
        mq_getattr(mq, &mqAttr);
        if (mqAttr.mq_curmsgs == MQ_MAX_SIZE)
        {
            printf("Queue(%d) reached max number of messages(%ld)\n", mq, mqAttr.mq_maxmsg);
        }
        else
        {
            //printf("Miner %d: generate miner block\n", *thread_id);
            //minerBlock = generateMinerBlock(*thread_id); //get the new block

            printf("Miner %d: generate miner block\n", 1);
            minerBlock = generateMinerBlock(1); //get the new block

            if ((minerBlock->hash & mask) == 0)
            {
                newBlock = minerBlock;
                          
                msg = malloc(sizeof(MSG_T));
                (msg->block) = newBlock;
                mq_send(mq, (char*)msg, MQ_MAX_MSG_SIZE, 0);
                printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by, minerBlock->height, (unsigned int)minerBlock->hash);
            }
            else
            {
                updateMinerBlock(minerBlock);
            }

            free(msg);
        }
    }
}

void updateMinerBlock(BLOCK_T *minerBlock)
{
    minerBlock->nonce = rand();
    minerBlock->timestamp = (int)time(NULL); // current time
    minerBlock->hash = generateHashFromBlock(minerBlock);
}

BLOCK_T *generateMinerBlock(int relayed_by)
{
    BLOCK_T *new_block = malloc(sizeof(BLOCK_T));
    new_block->height = block_chain_head->block->height + 1;
    new_block->timestamp = (int)time(NULL);               // current time in seconds
    new_block->relayed_by = relayed_by;                   // server id
    new_block->nonce = 0;                                 // dummy nonce
    new_block->prev_hash = block_chain_head->block->hash; // dummy prev hash
    new_block->hash = -1;                                 // dummy hash
    new_block->difficulty = NUM_OF_ZERO;

    return new_block;
}


//void *miner(int *thread_id)
//{
//    BLOCK_T *minerBlock = NULL;
//    int policy = -1;
//    struct sched_param sp = {0};
//
//    pthread_getschedparam(pthread_self(), &policy, &sp);
//    printf("Miner #%d with priority %d, policy %s\n", *thread_id, sp.sched_priority, POLICY_STR(policy));
//
//    while (block_chain_head == NULL)
//    {
//    };
//
//    for (;;)
//    {
//        if (minerBlock == NULL || minerBlock->prev_hash != block_chain_head->block->hash)
//        {
//            pthread_mutex_lock(&chain_lock);
//            printf("Miner %d: generate miner block\n", *thread_id);
//            minerBlock = generateMinerBlock(*thread_id); //get the new block
//            pthread_mutex_unlock(&chain_lock);
//        }
//
//        if ((minerBlock->hash & mask) == 0)
//        {
//            pthread_mutex_lock(&chain_lock);
//            if (newBlock == NULL)
//            {
//                newBlock = minerBlock;
//                printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
//                       minerBlock->height,
//                       (unsigned int)minerBlock->hash);
//
//                pthread_cond_signal(&new_block_cond); //sent server signal there is a new block to add
//            }
//            pthread_mutex_unlock(&chain_lock);
//        }
//        else
//        {
//            updateMinerBlock(minerBlock);
//        }
//    }
//}
//
//void *badMiner(int *thread_id)
//{
//    BLOCK_T *minerBlock = NULL;
//    int policy = -1;
//    struct sched_param sp = {0};
//    int bad_mask = 0xff000000;
//
//    pthread_getschedparam(pthread_self(), &policy, &sp);
//    printf("Miner #%d (bad mainer) with priority %d, policy %s\n", *thread_id, sp.sched_priority, POLICY_STR(policy));
//
//    while (block_chain_head == NULL)
//    {
//    };
//
//    for (;;)
//    {
//        if (minerBlock == NULL || minerBlock->prev_hash != block_chain_head->block->hash)
//        {
//            pthread_mutex_lock(&chain_lock);
//            printf("Miner %d(bad mainer): generate miner block\n", *thread_id);
//            minerBlock = generateMinerBlock(*thread_id); //get the new block
//            pthread_mutex_unlock(&chain_lock);
//            sleep(0.1);
//        }
//
//        if ((minerBlock->hash & bad_mask) == 0)
//        {
//            pthread_mutex_lock(&chain_lock);
//            if (newBlock == NULL)
//            {
//                newBlock = minerBlock;
//                printf("Miner #%d(bad mainer): Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
//                       minerBlock->height,
//                       (unsigned int)minerBlock->hash);
//
//                pthread_cond_signal(&new_block_cond); //sent server signal there is a new block to add
//            }
//            pthread_mutex_unlock(&chain_lock);
//        }
//        else
//        {
//            updateMinerBlock(minerBlock);
//        }
//    }
//}
//
