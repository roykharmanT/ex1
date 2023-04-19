#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct item{
    int friends_in_queue;
    int rivals_in_queue;
    struct item* next;
}*Item;

typedef struct IsraeliQueue_t{
        FriendshipFunction *friend_function;
        ComparisonFunction comparison_function;
        int rivalry_threshold;
        int friendship_threshold;
        Item head;
}*IsraeliQueue;


IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    if(q==NULL)
        return NULL;
    IsraeliQueue new_queue = IsraeliQueueCreate(q->friend_function, q->comparison_function,
    q->rivalry_threshold, q->friendship_threshold);
    return new_queue;
}


