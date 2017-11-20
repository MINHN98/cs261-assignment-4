/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dynarray.h"

#include "pq.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
  struct dynarray* vals;
  struct dynarray* prior;
};

struct node{
  int val;
  int prior;
};


void swap(struct pq* pq, int ind_1, int ind_2);
void percolate_up(struct pq* pq, int curr_ind);
void percolate_down(struct pq* pq, int curr_ind);

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* p_queue = malloc(sizeof(struct pq));
  p_queue->vals = dynarray_create();
  p_queue->prior = dynarray_create();
  return p_queue;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  assert(pq);
  dynarray_free(pq->vals);
  dynarray_free(pq->prior);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);
  return (dynarray_size(pq->vals) == 0);
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  assert(pq);

  //printf("%d\n", priority);

  //get last index where value will be added
  int last_ind = dynarray_size(pq->vals);

  //insert value and its priority to its respective arrays
  dynarray_insert(pq->vals, last_ind, value);
  dynarray_insert(pq->prior, last_ind, &priority);

  //percolate from last index up. 
  percolate_up(pq, last_ind);

  //int test_p = (*((int*) dynarray_get(pq->prior, last_ind)));
  //printf("%d\n", test_p);
  
  //previous code
  /*
  int curr = size;
  while(curr != 0){
    int parent_ind = ((curr-1)/2);
    int parent_prior = (*((int*) dynarray_get(pq->prior, parent_ind)));
    printf("parent: %d\n ", parent_prior);
    printf("parent ind: %d\n ", parent_ind);
    int curr_prior = (*((int*) dynarray_get(pq->prior, curr)));
    printf("curr: %d\n ", curr_prior);
    printf("curr ind: %d\n ", curr);
    if(parent_prior > curr_prior){
      swap(pq, parent_ind, curr);
    }
    curr = parent_ind;
  }
  */
}

void percolate_up(struct pq* pq, int curr_ind){
  if(curr_ind != 0){
    //get parent index
    int parent_ind = ((curr_ind-1)/2);

    //get parent and current priority values
    int parent_prior = (*((int*) dynarray_get(pq->prior, parent_ind)));
    int curr_prior = (*((int*) dynarray_get(pq->prior, curr_ind)));

    //compare and swap if parent is higher priority value (lower priority)
    if(parent_prior > curr_prior){
      swap(pq, parent_ind, curr_ind);
      percolate_up(pq, parent_ind);
    }
  }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq);
  return dynarray_get(pq->vals, 0);
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  assert(pq);
  int p = *((int*) dynarray_get(pq->prior, 0));
  //printf("Returning: %d\n", p);
  return p;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  assert(pq);

  // remember root value
  void* first_val = dynarray_get(pq->vals, 0);

  //get index of last element
  int last_ind = dynarray_size(pq->vals) - 1;

  //swap root and last element in heap
  swap(pq, 0, last_ind);

  // remove last element which is now the root
  dynarray_remove(pq->vals, last_ind);
  dynarray_remove(pq->prior, last_ind);

  //if the array is not empty, percolate down from root node
  if (dynarray_size(pq->vals) > 0){
    percolate_down(pq, 0);
  }

  //printf("Size of vals array: %d\n", dynarray_size(pq->vals));
  //printf("Size of prior array: %d\n", dynarray_size(pq->prior));

  return first_val;
}

void percolate_down(struct pq* pq, int curr_ind){
  //index of right and left children of curr
  int left_c_ind = 2 * curr_ind + 1;
  int right_c_ind = 2 * curr_ind + 2;

  // index of lowest priority value child
  int min_index;

  //if right index is out of bounds
  if(right_c_ind >= dynarray_size(pq->vals)){
    //if left index is out of bounds
    if (left_c_ind >= dynarray_size(pq->vals)){
      return;
    //if right is out of bounds and left is not
    }else{
      min_index = left_c_ind;
    }
  } else {
    //get priority values of left and right children
    int left_c_prior = (*((int*) dynarray_get(pq->prior, left_c_ind)));
    int right_c_prior = (*((int*) dynarray_get(pq->prior, right_c_ind)));
    //determine which child has the lower priority value (higher priority)
    if(left_c_prior <= right_c_prior){
      min_index = left_c_ind;
    }else{
      min_index = right_c_ind;
    }
  }

  //compare priority values of curr and child with the lowest priority value (high priority)
  if ((*((int*) dynarray_get(pq->prior, curr_ind))) > (*((int*) dynarray_get(pq->prior, min_index)))){
    swap(pq, curr_ind, min_index);
    percolate_down(pq, min_index);
  }

}


void swap(struct pq* pq, int ind_1, int ind_2){
  //printf("SWAPPING \n");
  void* temp_val = dynarray_get(pq->vals, ind_1);
  void* temp_prior = dynarray_get(pq->prior, ind_1);
 
  dynarray_set(pq->vals, ind_1, dynarray_get(pq->vals, ind_2));
  dynarray_set(pq->prior, ind_1, dynarray_get(pq->prior, ind_2));

  dynarray_set(pq->vals, ind_2, temp_val);
  dynarray_set(pq->prior, ind_2, temp_prior);
}
