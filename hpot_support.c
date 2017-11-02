/* hpot_support.c         <<-- A template 
 * Christian Walker         <<-- many updates required
 * ECE 223 Spring 2014 
 * MP2
 *
 * Propose: Middle man between lab2.c and list.c
            Contains functions hpot_compare, pList hpot_create, hpot_cleanup
            hpot_record_fill, hpot_record_print, hpot_print, hpot_stats,
            hpot_add, hpot, hpot_list, hpot_remove, hpot_scan, hpot_add_tail
            hpot_remove_head.
 *
 * Assumptions: 
 *
 * Bugs: None
 *
 * You can modify the interface for any of the hpot_ functions if you like
 * But you must clearly document your changes.
 *
 * (You CANNOT modify any of the details of the list.h interface, or use any 
 *  of the private variables outside of list.c.)
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "datatypes.h"
#include "list.h"
#include "hpot_support.h"
/* hpot_compare is required by the list ADT for sorted lists. 
 *
 * This function returns 
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and 
 *     0 if the records are equal.
 *
 * For the packet data we want to sort from lowest IP address up, so
 * closer to the front means a smaller IP address.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int hpot_compare(packet_t *record_a, packet_t *record_b)
{
    assert(record_a != NULL && record_b !=NULL);
    if (record_a->dest_ip_addr < record_b->dest_ip_addr)
        return 1;
    else if (record_a->dest_ip_addr > record_b->dest_ip_addr)
        return -1;
    else
        return 0;
}
/* This functions reads in the information from the user for the packet 
 * record.  It assumes that the memory block for the record has been malloced
 * and the dest ip address has been set. 
 *
 * Do not change the sequence of prompts as grading requires this exact
 * format
 */
void hpot_record_fill(packet_t *new)
{
    char line[MAXLINE];
    assert(new != NULL);
    printf("Source IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->src_ip_addr);
    printf("Destination port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->dest_port_num);
    printf("Source port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->src_port_num);
    printf("Hop count:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->hop_count);
    printf("Protocol:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->protocol);
    printf("Threat Score:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%f", &new->threat_score);
    printf("Time:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->time_received);
    printf("\n");
}
/* print the information for a particular record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * Do not change any of these lines and grading depends on this
 * format.
 */
void hpot_record_print(packet_t *rec)
{
    assert(rec != NULL);
    printf("Dest IP: %d, Src: %d, Dest port: %d,", rec->dest_ip_addr, 
            rec->src_ip_addr, rec->dest_port_num);
    printf(" Src: %d, Hop Count: %d", rec->src_port_num, rec->hop_count); 
    printf(" Prot: %d", rec->protocol);
    printf(" Score: %g, Time: %d\n", rec->threat_score, rec->time_received);
}
/* print one of the packet record lists
 *
 * inputs: 
 *    list_ptr: a pList pointer to either sorted or unsorted list
 *
 *    type_of_list: a charter string that must be either "pList" or "Queue"
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines 
 */
void hpot_print(pList list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List")==0 || strcmp(list_type, "Queue")==0);
    pIterator index;
    int counter = 0;
    index = list_iter_first(list_ptr);
    while (index != list_iter_tail(list_ptr))
    {
        printf("%d: ", ++counter);
        hpot_record_print(list_access(list_ptr, index));
        index = list_iter_next(index);
    }
    if (counter == 0) {
        printf("%s is empty\n", list_type);
    } else {
        printf("%s contains %d record%s\n", list_type, counter,
                counter==1 ? "." : "s.");
    }
    printf("\n");
}
/* This functions adds a packet record to the tail of a list.  The list is
 * unsorted, and it does not have any limits on the maximum size of the list.
 *
 * If the new packet has the same destination IP address as a packet that is
 * already in the list, the old matching packet is removed from the list and 
 * recycled.  The new packet is appended to the end of the list.
 */
void hpot_add_tail(pList list_ptr, packet_t *rec)
{
    int equal;
    int match = 0;
    packet_t *pdata;
    pIterator rover;
    rover = list_iter_first(list_ptr);
    while(rover != list_iter_tail(list_ptr)){
        pdata = list_access(list_ptr, rover);
        equal = hpot_compare(rec, pdata);
        if(equal == 0){
            match = 1;
            pdata = list_remove(list_ptr, rover);
            free(pdata);
            break;
        }
        rover = list_iter_next(rover);   
    }
    
    list_insert(list_ptr, rec, list_iter_tail(list_ptr));
    
    // if did not find a match in the list
    if(match == 0) printf("Appended %d onto queue\n", rec->dest_ip_addr);
    // else if found and removed a match in the list
    else printf("Appended %d onto queue and removed old copy\n", rec->dest_ip_addr);
}
/* This function removes the packet record at the head of the list.  The
 * list is unsorted, and there is no limit on the maximum size of the list.
 *
 * The pointer to the packet is returned to the calling function to be
 * freed
 */
packet_t *hpot_remove_head(pList list_ptr)
{
    packet_t *pkt_ptr = NULL;
    
    pkt_ptr = list_remove(list_ptr, list_iter_first(list_ptr));
    if (pkt_ptr != NULL)
        printf("Deleted head with IP addr: %d\n", pkt_ptr->dest_ip_addr);
    else
        printf("Queue empty, did not remove\n");
    return pkt_ptr;
}
/* This creates a list and it can be either a sorted or unsorted list
 */
pList hpot_create()
{
    return list_construct();
}
/* This function adds the packet to the sorted list.  
 * If the new packet has the same dest ip address as one or
 * more packets in the list, then the new packet is placed after all the
 * existing packets with the same address. 
 *
 * There is no return value since the insertion must always be 
 * successful, except in the catastrophic condition that the program
 * has run out of memory.
 */
void hpot_add(pList list_ptr, packet_t *new)
{
    list_insert_sorted(list_ptr , new);
    // after the packet is added you must print
    printf("Inserted %d into list\n", new->dest_ip_addr);
}
/* This function prints all packets with the matching ip_address in the
 * sorted list.  Print all fields of each matching packet record, and 
 * after the last packet print the number found.
 */
void hpot_list(pList list_ptr, int id)
{
    int count = 0;
    pIterator rover;
    rover = list_iter_first(list_ptr);
    packet_t *edata;
    packet_t *check_data;
    
    rover = list_iter_first(list_ptr);
    edata = list_access(list_ptr, rover);
    while(edata != NULL){
        if(edata->dest_ip_addr == id) break;
        rover = list_iter_next(rover);
        edata = list_access(list_ptr, rover);
    }
    
    if(edata != NULL){
        rover = list_elem_find(list_ptr, edata);    
        while(rover != NULL){
            check_data = list_access(list_ptr, rover);
            if(check_data == NULL) break;
            if(check_data->dest_ip_addr != id) break;
            else{
                count++;
                rover = list_iter_next(rover);
            }
        }
    }
    
    // after printing each matching record, print one of these summaries
    if (count > 0)
        printf("Found %d packets matching %d\n", count, id);
    else
        printf("Did not find: %d\n", id);
}
/* This function removes all packets from the sorted list with the matching
 * dest ip address
 */
void hpot_remove(pList list_ptr, int id)
{
    int count = 0;
    pIterator rover;
    rover = list_iter_first(list_ptr);
    packet_t *temp;
    rover = list_iter_first(list_ptr);
    temp = list_access(list_ptr, rover);
    while(temp != NULL){
        if(temp->dest_ip_addr == id){
            count++;
            temp = list_remove(list_ptr, rover);
            free(temp);
            rover = list_iter_first(list_ptr); 
            temp = list_access(list_ptr, rover);  
            
        }
        else{
            rover = list_iter_next(rover);
            temp = list_access(list_ptr, rover);
        }
    }
   
    if (count > 0)
        printf("Removed %d packets matching %d\n", count, id);
    else
        printf("Did not remove: %d\n", id);
}
void hpot_scan(pList list_ptr, int thresh)
{
    int count = 0;
   // int found_addr = -1;
    int sets = 0;
   // int reset;
    //int i;
    
    pIterator rover = list_iter_first(list_ptr);
    pIterator next;
    packet_t *new_rec = list_access(list_ptr , rover);
    packet_t *next_data;
    //found_addr = new_rec->dest_ip_addr;
    
    
    while(new_rec != NULL){
        next = list_iter_next(rover);
        next_data = list_access(list_ptr , next);
        count++;
        if(next_data == NULL){
            if(count >= thresh){
                printf("A set with address %d has %d packets\n", new_rec->dest_ip_addr, count);
                sets++;
                
            }
            break;
        }
                
        if(next_data->dest_ip_addr == new_rec->dest_ip_addr){
            rover = next;
            new_rec = next_data;
        }
        else{
            if(count >= thresh){
                printf("A set with address %d has %d packets\n", new_rec->dest_ip_addr, count);
                sets++;
            }
            count = 0;
            rover = next;
            new_rec = next_data;
        }
            
    }
    
    
   /* 
//    for(i = 0; i < list_size(list_ptr); i++){
    while(new_rec != NULL){
//        new_rec = list_access(list_ptr, rover);
//        if(new_rec == NULL) break;
//        found_addr = new_rec->dest_ip_addr;
        reset = 0;
      //  if (new_rec != NULL)
        //    printf("%d %d\n", new_rec->dest_ip_addr, count);
        if(new_rec->dest_ip_addr == found_addr){
            count++;
            rover = list_iter_next(rover);
            new_rec = list_access(list_ptr, rover);
           // printf("%d %d\n", new_rec->dest_ip_addr, count);
           reset = 1;
        }
         if (new_rec != NULL)
             printf("%d %d %d\n", new_rec->dest_ip_addr, count, found_addr);
        if(new_rec == NULL) break;
        if(count >= thresh && new_rec->dest_ip_addr != found_addr){
                // for each set print the following line
                printf("A set with address %d has %d packets\n", found_addr, count);
                sets++;
                count = 1;
            
            
       }
       if(new_rec == NULL) break;
       if(new_rec->dest_ip_addr != found_addr && reset == 0){
            rover = list_iter_next(rover);
            new_rec = list_access(list_ptr, rover);
            if(new_rec != NULL) {
                found_addr = new_rec->dest_ip_addr;
                count = 1;
            }
        }
        
     }
    if(count >= thresh){
        printf("A set with address %d has %d packets\n", found_addr, count);
        sets++;
    }
    */
    // after all sets have been discovered print one of the following
    if (sets > 0) 
        printf("Scan found %d sets\n", sets);
    else
        printf("Scan found no records with >= %d matches\n", thresh);
}
/* the function takes a pointer to each list and prints the
 * number of items in each list
 */
void hpot_stats(pList sorted, pList unsorted, pList newsort)
{
    // 1aget the number in list and size of the list
    int num_in_sorted_list = list_size(sorted);
    int num_in_unsorted_list = list_size(unsorted);
    int num_in_newsort_list = list_size(newsort);
    printf("Number records in list: %d, queue size: %d\n, sorted list: %d\n", 
            num_in_sorted_list, num_in_unsorted_list, num_in_newsort_list);
}
/* this function frees the memory for either a sorted or unsorted list.
 */
void hpot_cleanup(pList list_ptr)
{
    list_destruct(list_ptr);
}
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

