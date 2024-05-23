/*
    Created by :
       Tanmay Hatwar
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# ifndef MAP_H
# define MAP_H
# include"Store_Management.h"
# include"BPlus.h"
# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10

typedef struct item_map{
    item *itemNode;
    Bplus *root;
}item_map;

item_map map[MAP_SIZE];

void initialize_map();

int hash_function(int item_id);

item* searchItemMap(Bplus *root, int data);

void update_frequency(Bplus *Billroot);

void print_frequency(FILE *fptr, int item_id);

# endif