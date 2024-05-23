/*
    Created by :
        Tanmay Hatwar
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# ifndef CUSTOMERINVOICE_H
# define CUSTOMERINVOICE_H
# include"BPlus.h"
# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10

typedef struct user_bill{
    Bplus *bill_root;
    float total_cost;
}bill;

bill Invoice[INVOICE_SIZE];

void initialize_bill();

item* create_bill_item(int id, char *name, int quantity, int expiry_date[], float price);

void print_bill_node(FILE *fptr, item *node);

void print_bill(FILE *fptr, int userId);

# endif