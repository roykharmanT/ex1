#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef struct item{
    int friends_in_queue;
    int rivals_in_queue;
    void* ptr;
    struct item* next;
}*Item;

struct IsraeliQueue_t{
        FriendshipFunction *friendship_measures;
        ComparisonFunction comparison_function;
        int rivalry_threshold;
        int friendship_threshold;
        Item head;
};


int getFriendshipMeasureSize(FriendshipFunction* friendship_measure_array)
{
    // Return the amount of friendship measure in a FriendshipFunction array.
    int size = 0;
    while(*friendship_measure_array[size]){
        ++size;
    }
    return size;
}

IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friend_function,
 ComparisonFunction comparison_function, int friendship_threshold, int rivalry_threshold)
 {
    IsraeliQueue new_queue = (IsraeliQueue)malloc(sizeof(*new_queue));
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

void IsraeliQueueDestroy(IsraeliQueue q)
{
    if(q){
        Item head = q->head;
        while(head){
            Item to_destroy = head;
            head = head->next;
            free(to_destroy);
        }
        if(q->friendship_measures){
            free(q->friendship_measures);
        }
        free(q);
    }
}


int getRivalryMeasure(IsraeliQueue q, Item item_1, Item item_2)
{
    int number_of_friendship_measures = getFriendshipMeasureSize(q->friendship_measures);
    int sum = 0;
    for(int i = 0; i < number_of_friendship_measures; ++i){
        sum += q->friendship_measures[i](item_1->ptr, item_2->ptr);
    }
    return sum/number_of_friendship_measures;
}

Item getRivalsBehind(IsraeliQueue q, Item friend, Item item_to_insert)
{
    Item potential_rival = friend->next;
    while(potential_rival){
        int rivalry_measure = getRivalryMeasure(q, potential_rival, item_to_insert);
        if(rivalry_measure < q->rivalry_threshold && potential_rival->rivals_in_queue < RIVAL_QUOTA){
            return potential_rival;
        }
    }
    return NULL;
}

IsraeliQueueError enqueueItem(IsraeliQueue q, Item item_to_insert)
{
    if(!q->head){
        q->head = item_to_insert;
        return ISRAELIQUEUE_SUCCESS;
    }

    Item potential_friend = q->head;
    int number_of_friendship_measures = getFriendshipMeasureSize(q->friendship_measures);
    while(potential_friend)
    {
        if(potential_friend->friends_in_queue > FRIEND_QUOTA || potential_friend == item_to_insert)
        {
            //If friend quota is full continue to the next potential friend
            potential_friend = potential_friend->next;
            continue;
        }
        for(int i = 0; i < number_of_friendship_measures; ++i){
            //For each friendship measures check friendship_threshold
            if(q->friendship_measures[i](item_to_insert->ptr, potential_friend->ptr) > q->friendship_threshold)
            {

                Item rival_behind = getRivalsBehind(q, potential_friend, item_to_insert);
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

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *ptr)
{
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    
    Item item_to_insert = (Item)malloc(sizeof(*item_to_insert));
    item_to_insert ->friends_in_queue = 0;
    item_to_insert ->rivals_in_queue = 0;
    item_to_insert -> next = NULL;
    item_to_insert ->ptr = ptr;
    return enqueueItem(q, item_to_insert);
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction friend_func)
{
    if(q == NULL || *friend_func == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }

    int array_size = getFriendshipMeasureSize(q->friendship_measures);
    q->friendship_measures = (FriendshipFunction*)realloc(q->friendship_measures, (array_size + 2) * sizeof(FriendshipFunction));
    if(q->friendship_measures == NULL){
        //If failed to allocate memory return error
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    //Assign the new friendship measures array to the israeli queue
    q->friendship_measures[array_size] = friend_func;
    q->friendship_measures[array_size + 1] = NULL;

    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int new_threshold)
{
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->friendship_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int new_threshold)
{
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->rivalry_threshold = new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue q)
{
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


void* IsraeliQueueDequeue(IsraeliQueue q)
{
    void* to_return = NULL;
    if(q->head){
         to_return = q->head->ptr;
        q->head = q->head->next;
    }
    return to_return;
}


bool IsraeliQueueContains(IsraeliQueue q, void *ptr)
{
    Item current = q->head;
    while(current){
        void* compare_ptr = current->ptr;
        if(q->comparison_function(compare_ptr, ptr)){
            return true;
        }
    }
    return false;
}


IsraeliQueueError improvePosition(IsraeliQueue q, Item item)
{
    if(item->next){
        improvePosition(q, item->next);
    }
    enqueueItem(q, item);
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q)
{
    if(q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    return improvePosition(q, q->head);
}

void MergeThreshold(IsraeliQueue* qarr, int *avg_friend_threshold, int* avg_rivalry_threshold, int* sum_friend_array)
{
    int index = 0, sum_friendship = 0, sum_rivalry = 1, sum_queues = 0;
    *sum_friend_array = 0;
    while(qarr[index])
    {
        sum_friendship += qarr[index]->friendship_threshold;
        sum_rivalry *= qarr[index]->rivalry_threshold;
        *sum_friend_array += getFriendshipMeasureSize(qarr[index]->friendship_measures);
        sum_queues += IsraeliQueueSize(qarr[index]);
        index++;
    }
    *avg_friend_threshold = ceil((double)(sum_friendship)/(double)(index));
    *avg_rivalry_threshold = ceil(pow((double)(sum_rivalry), 1.0/(double)(index)));
}

void MergeFunction(IsraeliQueue* qarr, FriendshipFunction *friendship_func_to_queue)
{
    int index = 0, current = 0;
    FriendshipFunction* temp_func;
    while(qarr[index])
    {
        temp_func = qarr[index]->friendship_measures;
        int i = 0;
        while(temp_func[i]){
            friendship_func_to_queue[current] = temp_func[i];
            current++;
        }
        index++;
    }
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qarr ,ComparisonFunction compare_func)
{
    int avg_friend_threshold, avg_rivalry_threshold, sum_friend_array;
    MergeThreshold(qarr, &avg_friend_threshold, &avg_rivalry_threshold, &sum_friend_array);
    FriendshipFunction *friendship_func = (FriendshipFunction*)malloc(sum_friend_array*sizeof(FriendshipFunction));
    MergeFunction(qarr, friendship_func);
    IsraeliQueue new_queue = IsraeliQueueCreate(friendship_func, compare_func, avg_friend_threshold, avg_rivalry_threshold);
    void *ptr;
    bool remaining = true;
    while(remaining){
        int index = 0;
        remaining = false;
        while(qarr[index]){
            if(qarr[index]->head){
                ptr = qarr[index]->head->ptr;
                IsraeliQueueEnqueue(new_queue, ptr);
                qarr[index]->head = qarr[index]->head->next;
                remaining = true;
            }
            index++;
        }
    }
    return new_queue;
}