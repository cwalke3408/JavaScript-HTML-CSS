/* lab2.c template
 * Christian Walker           <<-- replace with your name!
 * ECE 223 Spring 2014
 * MP2
 * Subject: ECE223-1,#2
 *
 * Version 1
 *
 * Purpose: Calls functions from hpot_support.c 
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but the format of the commands and the prints found in 
 * hpot_record_fill hpot_print_rec should not be changed.
 *
 * Bugs: None
 *
 * See the ECE 223 programming guide
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "datatypes.h"
#include "list.h"
#include "hpot_support.h"
#include <time.h>

int main(int argc, char **argv)
{
    pList sorted_list;
    pList unsorted_list;
    pList new_sort;
    packet_t *new_rec;
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int num_items;
    int ip_add;

    clock_t start, end;
    double elapse_time; /* time in milliseconds */


    printf("Welcome to lab3\n");
    sorted_list = hpot_create();
    unsorted_list = hpot_create();
    new_sort = hpot_create();
 
    // remember fgets includes newline \n unless line too long
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s%d%s", command, &ip_add, junk);
        if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            /* found exit */
            hpot_cleanup(sorted_list);
            hpot_cleanup(unsorted_list);
            hpot_cleanup(new_sort);
            printf("cleanup\n");
            break;
        } else if (num_items == 2 && strcmp(command, "INSERT") == 0) {
            new_rec = (packet_t *) malloc(sizeof(packet_t));
            new_rec->dest_ip_addr = ip_add;
            hpot_record_fill(new_rec);
            hpot_add(sorted_list , new_rec);
            
        } else if (num_items == 2 && strcmp(command, "LIST") == 0) {
            hpot_list(sorted_list , ip_add);
        } else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
          hpot_remove(sorted_list , ip_add);
        } else if (num_items == 2 && strcmp(command, "SCAN") == 0) {
            hpot_scan(sorted_list, ip_add);
        } else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            hpot_print(sorted_list, "List");
        } else if (num_items == 2 && strcmp(command, "ADDTAIL") == 0) {
            packet_t *rec_ptr;
            rec_ptr = (packet_t *) malloc(sizeof(packet_t));
            rec_ptr->dest_ip_addr = ip_add;
            hpot_record_fill(rec_ptr);
            hpot_add_tail(unsorted_list , rec_ptr);
        } else if (num_items == 1 && strcmp(command, "RMHEAD") == 0) {
            new_rec = hpot_remove_head(unsorted_list);
            if(new_rec != NULL){
                free(new_rec);
                new_rec = NULL;
            } 
        } else if (num_items == 1 && strcmp(command, "PRINTQ") == 0) {
            hpot_print(unsorted_list, "Queue");
        } else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            hpot_stats(sorted_list, unsorted_list, new_sort);
	} else if (num_items == 2 && strcmp(command, "APPENDL") == 0){
	    new_rec = (packet_t *) malloc(sizeof(packet_t));
	    new_rec->dest_ip_addr = ip_add;
	    //hpot_record_fill(new_rec);
	    list_insert(new_sort, new_rec, list_iter_first(new_sort));
	} else if (num_items == 2 && strcmp(command, "SORTL") == 0){
	    int initialsize = list_size(new_sort);
	    start = clock();
	    list_sort(new_sort, ip_add);
	    end = clock();
	    elapse_time = 1000.0 * ((double) (end - start)) / CLOCKS_PER_SEC;
	    assert(list_size(new_sort) == initialsize);
	    printf("%d\t%f\t%d\n", initialsize, elapse_time, ip_add);
	    
	} else if (num_items == 2 && strcmp(command, "SCANL") == 0){
	    hpot_scan(new_sort, ip_add);
	} else if (num_items == 1 && strcmp(command, "PRINTL") == 0) {
	    hpot_print(new_sort, "List");
        } else {
            printf("# %s", line);
        }
    }
    exit(0);
}
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

