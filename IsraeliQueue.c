#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct item{
    int friends_in_queue;
    int rivals_in_queue;
    struct item* next;
}*Item;

typedef struct IsraeliQueue_t{
        FriendshipFunction *friendship_measures;
        ComparisonFunction comparison_function;
        int rivalry_threshold;
        int friendship_threshold;
        Item head;
}*IsraeliQueue;


IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friend_function,
 ComparisonFunction comparison_function, int friendship_threshold, int rivalry_threshold){
    IsraeliQueue new_queue;
    if(new_queue){
        new_queue -> friendship_measures = friend_function;
        new_queue -> comparison_function = comparison_function;
        new_queue -> rivalry_threshold = rivalry_threshold;
        new_queue -> friendship_threshold = friendship_threshold;
        new_queue -> head = NULL;
    }
    return new_queue;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    if(q==NULL)
        return NULL;
    IsraeliQueue new_queue = IsraeliQueueCreate(q->friendship_measures, q->comparison_function,
    q->rivalry_threshold, q->friendship_threshold);
    return new_queue;
}

void IsraeliQueueDestroy(IsraeliQueue q){
    Item head = q->head;
    while(head){
        Item to_destroy = head;
        head = head->next;
        free(to_destroy);
    }
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friend_func){
    int array_size = sizeof(q->friendship_measures) / sizeof(FriendshipFunction);
    FriendshipFunction *to_free = q->friendship_measures;
    FriendshipFunction *new_friendship_measures = malloc(sizeof(FriendshipFunction)*(1+array_size));

    if(new_friendship_measures == NULL){
        //If failed to allocate memory return error
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int index = 0;
    for(;index < array_size; ++index){
        // Copy freindship measured to new array
        new_friendship_measures[index] = q->friendship_measures[index];
    }
    //Assign the new friendship measures array to the israeli queue
    new_friendship_measures[index] = friend_func;
    q->friendship_measures = new_friendship_measures;

    if(array_size > 0){
        // Free old array
        free(to_free);
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue, FriendshipFunction){

}





