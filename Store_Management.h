/*
    Created by :
        Tanmay Hatwar
*/

# ifndef STORE_MANAGEMENT_H
# define STORE_MANAGEMENT_H

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10
# define M 5
# define C 3
typedef enum{FALSE, TRUE}boolean;
typedef enum{FAILURE, SUCCESS}status_code;

typedef struct item_node{
    int item_id;
    char *item_name;
    int quantity;
    int expiry_date[3];
    int threshold_quantity;
    float price; 
    struct item_node *next;
    int frequency;
}item;

typedef struct Tree_Node{
    union data{
        item* record[C+1];
        int indx[M];
    }data;
    int size;
    struct Tree_Node *parent;
    struct Tree_Node *pointers[M+1];
    boolean isRoot;
    boolean isLeaf;
    struct Tree_Node *next;
    struct Tree_Node *prev;
}Bplus;

typedef struct aisle_node{
    int aisle_id;
    char *product_type;
    Bplus *root;
}aisle;

aisle *aisle_array[DEFAULT_SIZE];

item* create_item(int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price);

aisle* create_aisle(int id, char *product_type);

void insert_item(int index,item* item_node);

void add_item(char *product_type, int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price);

void print_Item(FILE *ptr, item *curr);

void delete_item(FILE *fptr, int id);

void add_aisle(char *product_type);

void delete_aisle(FILE *fptr, int id);

boolean compare_dates(int expiry_date[], int required_date[], int curr_date[]);

void check_availability(FILE* fptr, int item_id, int req_quantity, int delivery_date[], int curr_date[]);

void inspect_quantity(FILE *fptr);

void find_date(int curr_date[], int required_date[], int day);

void inspect_readyToEat(FILE *fptr, int curr_date[]);

void inspect_dairyProduct(FILE *fptr, int curr_date[]);

void merge_aisle(FILE *fptr, char *product_type, int id1, int id2);

void print_aisle(FILE *fptr, aisle *a);

item* get_item(int item_id, int req_quantity);

void purchase_item(int item_id, int req_quantity, int userId);

void purchase_item_user(int id1, int q1, int id2, int q2, int id3, int q3, int id4, int q4, int id5, int q5, int userId);

void rangeSearch(FILE *fptr, int aisle_id, int id1, int id2);

# endif
