/* list.c                  <<-- A template to be updated and 
 * Christian Walker           <<-- changed
 * ECE 223 Spring 2014
 * MP2
 *
 * Propose: A template for list.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.  
 *
 * Bugs: Many detail have not been implemented.
 */
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>
#include "datatypes.h"   /* defines data_t */
#include "list.h"        /* defines public functions for list ADT */
/* definitions for private constants used in list.c only */
#define LIST_SORTED   -1234
#define LIST_UNSORTED -4132
/* prototypes for private functions used in list.c only */
void list_debug_print(list_t *list_ptr);
void list_validate(list_t *L);

void list_sort(pList list_ptr, int sort_type);
void IterativeSort(pList list_ptr, llist_node_t *head, llist_node_t *tail);
void RecursiveSort(pList list_ptr, llist_node_t *head, llist_node_t *tail);
llist_node_t *FindMin(list_t *list_ptr, llist_node_t *head, llist_node_t *tail);
void MergeSort(pList list_ptr);
/* ----- below are the functions  ----- */
/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED or LIST_UNSORTED
 *
 * The inital empty list must have
 * 1.  a dummy head llist_node_t
 * 2.  a dummy tail llist_node_t
 * 3.  llist_size = 0
 * 4.  llist_sort = LIST_SORTED
 *
 * Use list_decontruct to remove and deallocate all elements on a list,
 * the dummy head and tail, and the header block.
 */
list_t * list_construct(void)
{
    list_t *L = NULL;
    llist_node_t *H, *T;
    L = (list_t *) malloc(sizeof(list_t));
    H = (llist_node_t *) malloc(sizeof(llist_node_t));
    T = (llist_node_t *) malloc(sizeof(llist_node_t));
    L->llist_head = H;
    L->llist_tail = T;
    L->llist_size = 0;
    L->llist_sort = LIST_SORTED;
    
    H->data_ptr = NULL;
    T->data_ptr = NULL;
    H->prev = NULL;
    T->next = NULL;
    H->next = T;
    T->prev = H;
    
    /* the last line of this function must call validate */
 //   list_validate(L);
    return L;
}
/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, the dummy head and tail, and the header 
 * block.
 */
void list_destruct(list_t *list_ptr)
{
    /* the first line must validate the list */
    list_validate(list_ptr);
    llist_node_t *temp;
    while(list_ptr->llist_size > 0){
        temp = list_ptr->llist_head->next;
        list_ptr->llist_head->next = temp->next;
        temp->next->prev = list_ptr->llist_head;
        temp->next = NULL;
        temp->prev = NULL;
        free(temp->data_ptr);
        free(temp);
        list_ptr->llist_size--;
    }
    //free head and tail dummy block
    free(list_ptr->llist_tail);
    free(list_ptr->llist_head);
    free(list_ptr);
}
/* Return an Iterator that points to the first element in the list.  If the
 * list is empty the pointer that is returned is equal to the dummy tail
 * llist_node_t.
 */
llist_node_t * list_iter_first(list_t *list_ptr)
{
    assert(list_ptr != NULL);
  //  list_validate(list_ptr);
    return list_ptr->llist_head->next;
}
/* Return an Iterator that always points to the dummy tail llist_node_t. 
 */
llist_node_t * list_iter_tail(list_t *list_ptr)
{
    assert(list_ptr != NULL);
  //  list_validate(list_ptr);
    return list_ptr->llist_tail;
}
/* Advance the Iterator to the next item in the list.  The Iterator must not be
 * pointing to the dummy tail llist_node_t.
 *
 * To determine if an Iterator is past the last element and pointing to the
 * tail comper the Iterator to list_iter_tail.  Only if they are equal has
 * the Iterator moved past the last element.
 *
 * It is only valid to compare the Iterator to list_iter_first or 
 * list_iter_tail
 *
 * It is a catastrophic error to call this function if the
 * iterator, idx_ptr, is null or points to the tail.
 */
llist_node_t * list_iter_next(llist_node_t * idx_ptr)
{
    assert(idx_ptr != NULL && idx_ptr->next != NULL);
    return idx_ptr->next;
}
/* Obtains a pointer to an element stored in the specified list, at the
 * specified Iterator position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * idx_ptr: pointer to position of the element to be accessed.  This pointer
 *          must be obtained from list_elem_find, list_iter_first, or
 *          list_iter_next.  
 *
 * return value: pointer to the data_t element found in the list at the 
 * iterator position. A value NULL is returned if 
 *     1.  the list is empty 
 *     2.  the idx_ptr points to the dummy head or dummy tail
 */
data_t * list_access(list_t *list_ptr, llist_node_t * idx_ptr)
{
    assert(list_ptr != NULL);
    assert(idx_ptr != NULL);
    
    if(list_ptr->llist_size != 0){
        return(idx_ptr->data_ptr);
    }
    /* debugging function to verify that the structure of the list is valid */
//    list_validate(list_ptr);
    return NULL;   // you have to fix the return value
}
/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: uses the compare function defined in datatypes.h
 *
 * The function returns an Iterator pointer to the llist_node_t that contains 
 * the first matching element if a match if found.  If a match is not found 
 * the return value is NULL.
 *
 * Note: to get a pointer to the matching data_t memory block pass the return
 *       value from this function to the list_access function.
 */
llist_node_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr)
{
    int equal;
//    list_validate(list_ptr);
    
    llist_node_t *rover = list_ptr->llist_head->next;
    while(rover->data_ptr != NULL){
        equal = comp_proc(elem_ptr , rover->data_ptr);
        if(equal == 0)
            return(rover);
        else{
            rover = rover->next;
        }
    }
    /* fix the return value */
    return NULL;
}
/* Inserts the data element into the list in front of the llist_node_t 
 * iterator position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * idx_ptr: pointer to a llist_node_t.  The element is to be inserted as a 
 *          member in the list at the position that is immediately in front 
 *          of the position of the provided Iterator pointer.
 *
 * If idx_ptr is set using 
 *    -- list_iter_first, then the new element becomes the first item in 
 *       the list.
 *    -- list_iter_tail, then the new element becomes the last item in 
 *       the list.
 *    -- for any other idx_ptr, the new element is insert before the 
 *       Iterator
 *
 * For example, to insert at the tail of the list do
 *      list_insert(mylist, myelem, list_iter_tail(mylist))
 * to insert at the front of the list do
 *      list_insert(mylist, myelem, list_iter_first(mylist))
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->llist_sort must be equal 
 * to LIST_UNSORTED.
 */
void list_insert(list_t *list_ptr, data_t *elem_ptr, llist_node_t * idx_ptr)
{
    assert(list_ptr != NULL);
    /* insert your code here */
    llist_node_t *new_node;
    new_node = malloc(sizeof(llist_node_t));
    list_ptr->llist_size++;
    list_ptr->llist_sort = LIST_UNSORTED;
    
    // Front of the list
 
    new_node->prev = idx_ptr->prev;
    idx_ptr->prev->next = new_node;
    idx_ptr->prev = new_node;
    new_node->next = idx_ptr;
    new_node->data_ptr = elem_ptr;
    
    
    /* the last two lines of this function must be the following */
    if (list_ptr->llist_sort == LIST_SORTED) 
        list_ptr->llist_sort = LIST_UNSORTED;
   // list_validate(list_ptr);
}
/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the compare_proc.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * must generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->llist_sort == LIST_SORTED);
    /* insert your code here */
    int compare;
    int end_of_list = 1;
    
    llist_node_t *rover = list_ptr->llist_head->next;
    llist_node_t *new_node;
    new_node = malloc(sizeof(llist_node_t));
    new_node->data_ptr = elem_ptr;
  
    while(rover->data_ptr != NULL){
        compare = comp_proc(rover->data_ptr, elem_ptr);
        if(compare == -1){
            new_node->next = rover;
            new_node->prev = rover->prev;
            rover->prev->next = new_node;
            rover->prev = new_node;
            list_ptr->llist_size++;
            end_of_list = 0;
            break;
        }
        else if(compare == 0){
            rover = rover->next;
            while(rover->data_ptr != NULL){
                compare = comp_proc(rover->data_ptr, elem_ptr);
                if(compare == 0){
                    rover = rover->next;
                }
                else {
                    end_of_list = 0;
                    break;
                }               
            }
            if(end_of_list == 1){
                break;
            }
            else{
                new_node->prev = rover->prev;
                new_node->next = rover;
                rover->prev->next = new_node;
                rover->prev = new_node;
                list_ptr->llist_size++;
                break;
            }
        }
        else{
            rover = rover->next;
        }        
    }
    
    if(end_of_list == 1){
        new_node->prev = rover->prev;
        new_node->next = rover;
        rover->prev->next = new_node;
        rover->prev = new_node;
        list_ptr->llist_size++;
    }
    /* the last line of this function must be the following */
 //   list_validate(list_ptr);
}
/* Removes the element from the specified list that is found at the 
 * Iterator llist_node_t index.  A pointer to the data element is returned.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * idx_ptr: pointer to position of the element to be accessed.  This pointer
 *          must be obtained from list_elem_find, list_iter_first, 
 *          list_iter_next, or list_iter_tail.  
 *
 * If the list is empty, then the function should return NULL.
 *
 * Note: it is easy to remove the element at the front of the list using
 *     list_remove(mylist, list_iter_first(mylist))
 *
 * However, removing the element at the end of the list is not easy since
 * there is no way to get the pointer to the last element in the list.  
 * Notice that list_iter_tail returns a pointer to the dummy tail, not the 
 * last element in the list.
 *
 * Note: a significant danger with this function is that once
 * an element is removed from the list, the idx_ptr is dangling.  That
 * is, the idx_ptr now points to memory that is no longer valid.
 * You should not call list_iter_next on an iterator after there
 * has been a call to list_remove with the same iterator.
 *
 * When you remove the llist_node_t in this function, you should null the next
 * and prev pointers before you free the memory block to avoid accidental use
 * of these now invalid pointers. 
 */
data_t * list_remove(list_t *list_ptr, llist_node_t * idx_ptr)
{
    if (idx_ptr == NULL)
        return NULL;
    if (list_ptr->llist_size == 0) return NULL;
    assert(idx_ptr != list_ptr->llist_head && idx_ptr != list_ptr->llist_tail);
    /* insert your code here */
    data_t *temp = idx_ptr->data_ptr;
    idx_ptr->next->prev = idx_ptr->prev;
    idx_ptr->prev->next = idx_ptr->next;
    idx_ptr->next = NULL;
    idx_ptr->prev = NULL;
    list_ptr->llist_size--;
    
    free(idx_ptr);
    
    /* the last line should verify the list is valid after the remove */
//    list_validate(list_ptr);
    return temp;  // you must fix the return value
}
/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int list_size(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->llist_size >= 0);
    return list_ptr->llist_size;
}
/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void list_validate(list_t *L)
{
    llist_node_t *N;
    int count = 0;
    assert(L != NULL && L->llist_head != NULL && L->llist_tail != NULL);
    assert(L->llist_head != L->llist_tail);
    assert(L->llist_head->prev == NULL && L->llist_tail->next == NULL);
    assert(L->llist_head->data_ptr == NULL && L->llist_tail->data_ptr == NULL);
    assert(L->llist_head->next != NULL && L->llist_tail->prev != NULL);
    assert(L->llist_sort == LIST_SORTED || L->llist_sort == LIST_UNSORTED);
    N = L->llist_head->next;
    assert(N->prev == L->llist_head);
    while (N != L->llist_tail) {
        assert(N->data_ptr != NULL);
        assert(N->next->prev == N);
        count++;
        N = N->next;
    }
    assert(count == L->llist_size);
    if (L->llist_sort == LIST_SORTED && L->llist_size > 0) {
        N = L->llist_head->next;
        while (N->next != L->llist_tail) {
            assert(comp_proc(N->data_ptr, N->next->data_ptr) != -1);
            N = N->next;
        }
    }
}
/* Contain four different types of ways to sort by Insertion sort, Iterative sort,
   Recursive Sort, and Merge Sort.
   Parameters: 
        list_ptr: pointer to a list_t block
        sort_type: Determine which sort type to use 1 = Insertion sort, 2 = Iterative sort
                   3 = Recursive Sort, and 4 = Merge Sort
   Return: Void

 */

void list_sort(list_t *list_ptr, int sort_type){

//list_debug_print(list_ptr);

    pList newlist;
    data_t *temp_ptr = NULL;
    llist_node_t *head = list_ptr->llist_head->next;
    llist_node_t *tail = list_ptr->llist_tail->prev;

//printf("Sort: %d\n", sort_type);

    if (sort_type == 1){
	newlist = list_construct();
	while(head->data_ptr != NULL){
//	    printf("p: %p\n", head->data_ptr);
	    temp_ptr = list_remove(list_ptr, head);
	    list_insert_sorted(newlist, temp_ptr);
	    head = list_ptr->llist_head->next; 
	}
	
	
	//hpot_print(newlist, "List");
	free (list_ptr->llist_head);
	free (list_ptr->llist_tail);
	
	*list_ptr = *newlist;
	free(newlist);
    }
    else if(sort_type == 3){
//printf("h: %p, t: %p\n", head, tail);
	IterativeSort(list_ptr, head, tail);
	
    }
    else if(sort_type == 2){
	    RecursiveSort(list_ptr, head, tail);
    }
    else if(sort_type == 4){
	    MergeSort(list_ptr);
    }   
     
//    list_debug_print(list_ptr);
    list_ptr->llist_sort = LIST_SORTED;
    
}

/* Selection Sort in Iterative form.  
   Parameter:
        list_ptr: pointer to unsorted list
        head_ptr: points to head in list_ptr which is where the sorts start
                  work its way towards the tail.
        tail_ptr: points to tail in list_ptr which serves for a stoping point 
                  to tell where to end the sort
    Return: Void
*/

void IterativeSort(list_t *list_ptr, llist_node_t *head, llist_node_t *tail){
    llist_node_t *min;
    data_t *temp;
    llist_node_t *rover;
    
    //while(head->data_ptr != NULL)
    if(list_ptr->llist_size == 0);
    else if(list_ptr->llist_size > 0){
        while(head != tail){
            rover = head;
            min = head;
            do{
                rover = rover->next;
                if(rover->data_ptr->dest_ip_addr < min->data_ptr->dest_ip_addr){
                    min = rover;
                }
            } while(rover != tail);
        
            temp = head->data_ptr;
            head->data_ptr = min->data_ptr;
            min->data_ptr = temp;
        
            head = head->next;  
        }
    }
    
}
/* Recursive version of the same selction sort as iterative selection sort but a faster so$
   It use the FindMax function to find the min value.  Once it finds a min value it just
   swap the pointers of the min item and current item.
   Parameters:
        list_ptr: pointer to an unsorted list
        head_ptr:  points to head in list_ptr which is where the sorts start
                  work its way towards the tail.
        tail_ptr: points to tail in list_ptr which serves for a stoping point 
                  to tell where to end the sort
   Return: Void
*/

void RecursiveSort(list_t *list_ptr, llist_node_t *head, llist_node_t *tail){
 
    llist_node_t *min;
    data_t *temp;
    
    if (list_ptr->llist_size == 0);
    else if (head != tail){
        min = FindMin(list_ptr, head, tail);
        
        temp = head->data_ptr;
        head->data_ptr = min->data_ptr;
        min->data_ptr = temp;
        
        RecursiveSort(list_ptr, head->next, tail);
    }
        
    
}
/* Finds the min value of the unsorted list and return it
   Parameters:
        list_ptr: pointer to unsorted list
        head_ptr: points to head in list_ptr which is where the sorts start
                  work its way towards the tail.
        tail_ptr: points to tail in list_ptr which serves for a stoping point 
                  to tell where to end the sort
   Return: the min value to be swap with the current item that head_ptr is pointing
           to in RSelectionSort.
*/

llist_node_t *FindMin(list_t *list_ptr, llist_node_t *head, llist_node_t *tail){

    llist_node_t *rover = head;
    llist_node_t *min_ptr = head;
    
    do{
        rover = rover->next;
        if(rover->data_ptr->dest_ip_addr < min_ptr->data_ptr->dest_ip_addr){
            min_ptr = rover;
        }
    } while (rover != tail);    
        
    return min_ptr;
}
/* MergeSort uses recursion to split the unsorted list into two halfs and countiues
   until each entry is in its own list.  It merge the right and left into a sorted
   list.  
   Parameter: list_ptr: pointer to an unsorted list
   Return:    Void
*/

void MergeSort(list_t *list_ptr){
    //Initialize list R and list L
    list_t *L, *R, *SL;
    data_t *temp;
    int mid, i, compare;
    int size = list_ptr->llist_size;
    llist_node_t *rover = list_ptr->llist_head->next;
   
    if(size > 1){
        R = list_construct();
        L = list_construct();
        mid = size/2;
        
        //rove to the middle pos
        for(i=1; i < mid; i++){
            rover = rover->next;
        }
        //split into two list
        list_ptr->llist_head->next->prev = L->llist_head;
        L->llist_head->next = list_ptr->llist_head->next;
        L->llist_tail->prev = rover;
        list_ptr->llist_tail->prev->next = R->llist_tail;
        R->llist_head->next = rover->next;
        R->llist_tail->prev = list_ptr->llist_tail->prev;
        rover->next = L->llist_tail;
        R->llist_head->next->prev = R->llist_head;
        L->llist_size = mid;
        R->llist_size = list_ptr->llist_size - mid;  
        
        free (list_ptr->llist_head);
	free (list_ptr->llist_tail);      
                
        MergeSort(L);
        MergeSort(R);
        
        SL = list_construct();
       // list_validate(L);
       // list_validate(R);
        
        while(R->llist_head->next->data_ptr != NULL || L->llist_head->next->data_ptr != NULL ){
            if(R->llist_head->next->data_ptr == NULL && L->llist_head->next->data_ptr != NULL){                
                temp = list_remove(L, list_iter_first(L));
                list_insert(SL, temp, list_iter_tail(SL));
            }
            else if(R->llist_head->next->data_ptr != NULL && L->llist_head->next->data_ptr == NULL){
                temp = list_remove(R, list_iter_first(R));
                list_insert(SL, temp, list_iter_tail(SL));
            }
            else {
                compare = comp_proc(L->llist_head->next->data_ptr, R->llist_head->next->data_ptr);
                if(compare == 1){
                    temp = list_remove(L, L->llist_head->next);
                    list_insert(SL, temp, list_iter_tail(SL));           
                }
                else{
                    temp = list_remove(R, R->llist_head->next);
                    list_insert(SL, temp, list_iter_tail(SL));                    
                }
            }
            
        //    free (list_ptr->llist_head);
	  //  free (list_ptr->llist_tail);
            //list_destruct(R);
            //list_destruct(L);
        }
        *list_ptr = *SL;
        list_destruct(R);
        list_destruct(L);
        free(SL);
             
    }
}
/*
void list_debug_print(list_t *list_ptr) {
    llist_node_t *L;
    int count;
    printf("List size: %d\n", list_ptr->llist_size);
    L = list_ptr->llist_head->next;
    count = 0;
    while(L != list_ptr->llist_tail) {
        printf("Pos: %d contains IP: %d\n", count, L->data_ptr->dest_ip_addr);
        L = L->next;
        count++;
    }
    if (count != list_ptr->llist_size)
        printf("Hey!! the list size is wrong! count is %d\n", count);
}
*/
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

