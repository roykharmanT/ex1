#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct item{
    int friends_in_queue;
    int rivals_in_queue;
    void* ptr;
    struct item* next;
}*Item;

typedef struct IsraeliQueue_t{
        FriendshipFunction *friendship_measures;
        ComparisonFunction comparison_function;
        int rivalry_threshold;
        int friendship_threshold;
        Item head;
}*IsraeliQueue;


int get_friendship_measure_size(FriendshipFunction* friendship_measure_array){
    // Return the amount of friendship measure in a FriendshipFunction array.
    int size = 0;
    while(*friendship_measure_array[size]){
        ++size;
    }
    return size;
}

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
    int number_of_friendship_measures = get_friendship_measure_size(q->friendship_measures);
    int sum = 0;
    for(int i = 0; i < number_of_friendship_measures; ++i){
        sum += q->friendship_measures[i](item_1->ptr, item_2->ptr);
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

IsraeliQueueError enqueue_item(IsraeliQueue q, Item item_to_insert){
    Item potential_friend = q->head;
    int number_of_friendship_measures = get_friendship_measure_size(q->friendship_measures);
    while(potential_friend){
        if(potential_friend->friends_in_queue > FRIEND_QUOTA){
            //If friend quota is full continue to the next potential friend
            potential_friend = potential_friend->next;
            continue;
        }
        for(int i = 0; i < number_of_friendship_measures; ++i){
            //For each friendship measures check friendship_threshold
            if(q->friendship_measures[i](item_to_insert->ptr, potential_friend->ptr) > q->friendship_threshold){

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
    potential_friend = item_to_insert;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *ptr){
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    
    Item item_to_insert = {0, 0, ptr, NULL};
    return enqueue_item(q, item_to_insert);
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friend_func){
    if(q == NULL || *friend_func == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }

    FriendshipFunction *to_free = q->friendship_measures;
    int array_size = get_friendship_measure_size(q->friendship_measures);
    FriendshipFunction *new_friendship_measures = (FriendshipFunction*)malloc((array_size + 2) * sizeof(FriendshipFunction));
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
    new_friendship_measures[array_size] = friend_func;
    new_friendship_measures[array_size + 1] = NULL;
    q->friendship_measures = new_friendship_measures;

    if(to_free){
        // Free old array
        free(to_free);
    }
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int new_threshold){
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->friendship_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int new_threshold){
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->rivalry_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue q){
    int size = 0;
    if(q == NULL){
        return 0;
    }
    Item current = q->head;
    while(current){
        size += 1;
        current = current->next;
    }
    return size;
}


void* IsraeliQueueDequeue(IsraeliQueue q){
    Item to_return = q->head;
    q->head = q->head->next;
    return to_return;
}


bool IsraeliQueueContains(IsraeliQueue q, void *ptr){
    Item current = q->head;
    while(current){
        void* compare_ptr = current->ptr;
        if(q->comparison_function(compare_ptr, ptr)){
            return true;
        }
    }
    return false;
}


IsraeliQueueError improve_position(IsraeliQueue q, Item item){
    if(item->next){
        improve_position(q, item->next);
    }
    enqueue_item(q, item);
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }

    return improve_position(q, q->head);
}

