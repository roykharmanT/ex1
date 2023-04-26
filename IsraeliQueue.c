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


int get_rivalry_measure(IsraeliQueue q, Item item_1, Item item_2){
    int number_of_friendship_measures = sizeof(q->friendship_measures)/sizeof(FriendshipFunction);
    int sum = 0;
    for(int i = 0; i < number_of_friendship_measures; ++i){
        sum += q->friendship_measures[i](item_1, item_2);
    }
    return sum/number_of_friendship_measures;
}

Item get_rivals_behind(IsraeliQueue q, Item friend, Item item_to_insert){
    Item potential_rival = friend->next;
    while(potential_rival){
        int rivalry_measure = get_rivalry_measure(q, potential_rival, item_to_insert);
        if(rivalry_measure < q->rivalry_threshold & potential_rival->rivals_in_queue < RIVAL_QUOTA){
            return potential_rival;
        }
    }
    return NULL;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *ptr){
    Item potential_friend = q->head;
    Item item_to_insert = (Item)ptr;
    int number_of_friendship_measures = sizeof(q->friendship_measures)/sizeof(FriendshipFunction);

    while(potential_friend){
        if(potential_friend->friends_in_queue > FRIEND_QUOTA){
            //If friend quota is full continue to the next potential friend
            potential_friend = potential_friend->next;
            continue;
        }
        for(int i = 0; i < number_of_friendship_measures; ++i){
            //For each friendship measures check friendship_threshold
            if(q->friendship_measures[i](item_to_insert, potential_friend) > q->friendship_threshold){

                Item rival_behind = get_rivals_behind(q, potential_friend, item_to_insert);
                if(rival_behind){
                    //If a rival is blocking, increase his enemies in queue and skip to behind the rival
                    rival_behind->rivals_in_queue += 1;
                    potential_friend = rival_behind;
                    break;
                }
                // Insert item behind friend
                potential_friend->friends_in_queue += 1;
                item_to_insert->next = potential_friend->next;
                potential_friend->next = item_to_insert;
                return ISRAELIQUEUE_SUCCESS;
            }
        }
        //Try the next friend
        potential_friend = potential_friend->next;
    }
    //If Item can't cut in queue place it at the end of the queue
    item_to_insert->next = NULL;
    potential_friend->next = item_to_insert;
    return ISRAELIQUEUE_SUCCESS;
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

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int new_threshold){
    q->friendship_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int new_threshold){
    q->rivalry_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}






