#ifndef BPLUS_H
# define BPLUS_H

# include<stdio.h>
# include<stdlib.h>
# include<limits.h>
# include"Store_Management.h"

# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10

int ceilingFunction(int n);

Bplus* createLeafNode();

Bplus* createNode();

Bplus* getPrevNode(Bplus *node);

Bplus* getNextNode(Bplus *node);

int insertIndx(Bplus *node, int data);

Bplus* splitData(Bplus *node, int s);

void upgrade(Bplus **root, Bplus *node);

void insertData(Bplus **root, Bplus *node, item *newNode);

void insert(Bplus **root, Bplus *node, item *newNode);

Bplus* getLeftmostDataNode(Bplus *root);

void printList(FILE *fptr, Bplus *root);

Bplus* initialize();

int binarySearch(item* arr[], int n, int itemId);

boolean isDataPresent(Bplus *node, int itemId);

item* searchItem(int data);

Bplus* search(Bplus *root, int data, int *index);

void mergeDataNode(Bplus *pnode, int index);

Bplus* getLeftSibling(Bplus *node, int *index);

Bplus* getRightSibling(Bplus *node, int *index);

void merge(Bplus *node, int index);

void defficiency(FILE *fptr, Bplus *node);

void deleteData(FILE *fptr, Bplus *node, int data, int index);

void deleteItem(FILE *fptr, Bplus **root, int data);

void deleteList(FILE *fptr, Bplus **root);

# endif