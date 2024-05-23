

# include<stdio.h>
# include<stdlib.h>
# include<limits.h>
# include"BPlus.h"

int ceilingFunction(int n) {
    int ret_val=n/2;
    if(n%2==1) {
        ret_val++;
    }
    return ret_val;
}

# define BPLUS_MINLIM (ceilingFunction(M)-1)
# define BPLUS_MAXLIM M-1
# define DATANODE_MINLIM (ceilingFunction(C))
# define DATANODE_MAXLIM C

Bplus* createLeafNode() {
    Bplus *newNode=(Bplus*)malloc(sizeof(Bplus));
    for(int i=0; i<M+1; i++) {
        newNode->pointers[i]=NULL;
    }
    for(int i=0; i<C+1; i++) {
        newNode->data.record[i]=NULL;
    }
    newNode->isLeaf=TRUE;
    newNode->isRoot=FALSE;
    newNode->parent=newNode->next=newNode->prev=NULL;
    newNode->size=0;
    return newNode;
}

Bplus* createNode() {
    Bplus *newNode=(Bplus*)malloc(sizeof(Bplus));
    for(int i=0; i<M+1; i++) {
        newNode->pointers[i]=NULL;
    }
    for(int i=0; i<C+1; i++) {
        newNode->data.record[i]=NULL;
    }
    newNode->isLeaf=FALSE;
    newNode->isRoot=FALSE;
    newNode->parent=newNode->next=newNode->prev=NULL;
    newNode->size=0;
    return newNode;
}

Bplus* getPrevNode(Bplus *node) {
    Bplus *ret_val=NULL;
    if(node->parent!=NULL) {
        int i=0;
        while(i<(node->parent->size) && node->parent->pointers[i]!=node) {
            i++;
        }
        i--;
        if(i>=0) {
            ret_val=node->parent->pointers[i];
        }
        else {
            Bplus *leftP=getPrevNode(node->parent);
            if(leftP!=NULL) {
                ret_val=leftP->pointers[(leftP->size)];
            }
        }
    }
    return ret_val;
}

Bplus* getNextNode(Bplus *node) {
    Bplus *ret_val=NULL;
    if(node->parent!=NULL) {
        int i=0;
        while(i<(node->parent->size) && node->parent->pointers[i]!=node) {
            i++;
        }
        i++;
        if(i<=node->parent->size) {
            ret_val=node->parent->pointers[i];
        }
        else {
            Bplus *RightP=getNextNode(node->parent);
            if(RightP!=NULL) {
                ret_val=RightP->pointers[0];
            }
        }
    }
    return ret_val;
}

int insertIndx(Bplus *node, int data) {
    int i=0;
    while(i<node->size && node->data.indx[i]<data) i++;
    for(int j=(node->size)-1; j>=i; j--) {
        node->data.indx[j+1]=node->data.indx[j];
        node->pointers[j+2]=node->pointers[j+1];
    }
    node->data.indx[i]=data;
    (node->size)++;
    return i;
}

Bplus* splitData(Bplus *node, int s) {
    Bplus *newNode;
    if(node->isLeaf) {
        newNode=createLeafNode();
        for(int i=0; i<((node->size)-s); i++) {
            newNode->data.record[i]=node->data.record[s+i];
        }
        newNode->size=(node->size-s); 
        newNode->parent=node->parent;
        newNode->prev=node;
        newNode->next=node->next;
        node->next=newNode;
        node->size=s;
    }
    else {
        newNode=createNode();
        for(int i=0; i<((node->size)-s); i++) {
            newNode->data.indx[i]=node->data.indx[s+i];
            newNode->pointers[i]=node->pointers[s+i];
        }
        newNode->pointers[(node->size)-s]=node->pointers[(node->size)];
        for(int i=s; i<(node->size); i++) {
            node->pointers[i]=NULL;
        }
        newNode->size=(node->size-s); 
        newNode->parent=node->parent;
        for(int i=0; i<newNode->size+1; i++) {
            if(newNode->pointers[i]!=NULL) {
                newNode->pointers[i]->parent=newNode;
            }
        }
        node->size=s-1;
    }
    return newNode;
}

void upgrade(Bplus **root, Bplus *node) {
    int data=node->data.indx[M/2];
    int index;
    if(node->parent==NULL) {
        node->parent=createNode();
        node->parent->data.indx[0]=data;
        (node->parent->size)++;
        Bplus *newNode=splitData(node, (M/2)+1);
        node->parent->pointers[0]=node;
        node->parent->pointers[1]=newNode;
        *root=node->parent;
        node->parent->isRoot=TRUE;
    }
    else {
        index=insertIndx(node->parent, data);
        Bplus *newNode=splitData(node, (M/2)+1);
        node->parent->pointers[index]=node;
        node->parent->pointers[index+1]=newNode;
        if(node->parent->size==M) {
            upgrade(root, node->parent);
        }
    }
}

void insertData(Bplus **root, Bplus *node, item *newNode) {
    int i=0;
    while(i<node->size && node->data.record[i]!=NULL && node->data.record[i]->item_id<newNode->item_id) {
        i++;
    }
    if(node->data.record[i] !=NULL && node->data.record[i]->item_id==newNode->item_id) {
        printf("Error : duplicate data\n");
    }
    else {
        for(int j=node->size-1; j>=i; j--) {
            node->data.record[j+1]=node->data.record[j];
        }
        node->data.record[i]=newNode;
        (node->size)++;
        if(node->size>DATANODE_MAXLIM) {
            int d=node->data.record[(C+1)/2]->item_id;
            int index=insertIndx(node->parent, d);
            Bplus *newNode=splitData(node, ((C+1)/2));
            node->parent->pointers[index+1]=newNode;
            if((node->parent->size)>BPLUS_MAXLIM) {
                upgrade(root, node->parent);
            }
        }
    }
}

void insert(Bplus **root, Bplus *node, item *newNode) {
    if(node->isLeaf) {
        insertData(root, node, newNode);
    }
    else {
        int i=0;
        while(i<node->size && node->data.indx[i]<=newNode->item_id) {
            i++;
        }
        if(node->pointers[i]==NULL) {
            Bplus *newNode=createLeafNode();
            newNode->parent=node;
            node->pointers[i]=newNode;
            newNode->next=getNextNode(newNode);
            newNode->prev=getPrevNode(newNode);
        }
        insert(root, node->pointers[i], newNode);
    }
}

Bplus* getLeftmostDataNode(Bplus *root) {
    Bplus *ret_val=NULL;
    Bplus *ptr=root;
    if(root==NULL) {
        printf("Empty tree\n");
    }
    else {
        while(ptr->pointers[0]!=NULL) {
            ptr=ptr->pointers[0];
        }
        ret_val=ptr;
    }
    return ret_val;
}

void printList(FILE *fptr, Bplus *root) {
    if(root!=NULL) {
        Bplus *ptr=getLeftmostDataNode(root);
        if(ptr==NULL) {
            fprintf(fptr, "Cannot find list\n");
        }
        while(ptr!=NULL) {
            for(int i=0; i<ptr->size; i++) {
                print_Item(fptr, ptr->data.record[i]);
            }
            ptr=ptr->next;
        }
        fprintf(fptr, "\n\n");
    }
    else {
        fprintf(fptr, "Tree does not exist\n");
    }
}

Bplus* initialize() {
    Bplus *newNode=createNode();
    newNode->data.indx[0]=INT_MAX;
    newNode->isRoot=TRUE;
    newNode->size=0;
    for(int i=0; i<C+1; i++) {
        newNode->data.record[i]=NULL;
    }
    for(int i=0; i<M+1; i++) {
        newNode->pointers[i]=NULL;
    }
    return newNode;
}

int binarySearch(item* arr[], int n, int itemId) {
    int l=0, h=n-1;
    int mid=l+(h-l)/2;
    while(l<=h && arr[mid]->item_id!=itemId) {
        if(arr[mid]->item_id>itemId) {
            h=mid-1;
        }
        else {
            l=mid+1;
        }
        mid=l+(h-l)/2;
    }
    return mid;
}

boolean isDataPresent(Bplus *node, int itemId) {
    boolean ret_val=FALSE;
    int index=binarySearch(node->data.record, node->size, itemId);
    if(node->data.record[index]->item_id==itemId) {
        ret_val=TRUE;
    }
    return ret_val;
}

item* searchItem(int data) {
    int index=(data/10000)-1;
    item *ret_val=NULL;
    Bplus *ptr=aisle_array[index]->root;
    while(ptr!=NULL && !(ptr->isLeaf)) {
        int i=0;
        while(i<ptr->size && ptr->data.indx[i]<=data) {
            i++;
        }
        ptr=ptr->pointers[i];
    }
    if(ptr!=NULL) {
        int index=binarySearch(ptr->data.record, ptr->size, data);
        if(ptr->data.record[index]->item_id==data) {
            ret_val=ptr->data.record[index];
        }
    }
    return ret_val;
}

Bplus* search(Bplus *root, int data, int *index) {
    Bplus *ret_val=NULL;
    Bplus *ptr=root;
    while(ptr!=NULL && !(ptr->isLeaf)) {
        int i=0;
        while(i<ptr->size && ptr->data.indx[i]<=data) {
            i++;
        }
        ptr=ptr->pointers[i];
        *index=i;
    }
    if(ptr!=NULL) {
        if(isDataPresent(ptr, data)) {
            ret_val=ptr;
        }
    }
    return ret_val;
}

void mergeDataNode(Bplus *pnode, int index) {
    Bplus *node=pnode->pointers[index];
    Bplus *nextNode=node->next;
    for(int j=0; j<nextNode->size; j++) {
        node->data.record[j+node->size]=nextNode->data.record[j];
    }
    node->size=node->size+nextNode->size;
    for(int j=index; j<pnode->size-1; j++) {
        pnode->data.indx[j]=pnode->data.indx[j+1];
        pnode->pointers[j+1]=pnode->pointers[j+2];
    }
    pnode->pointers[pnode->size-1]=pnode->pointers[pnode->size];
    node->next=nextNode->next;
    node->parent=pnode;
    pnode->pointers[index]=node;
    (pnode->size)--;
    free(nextNode);
}

Bplus* getLeftSibling(Bplus *node, int *index) {
    Bplus *ret_val=NULL;
    if(node->parent!=NULL) {
        int i=0;
        while(i<(node->parent->size+1) && node->parent->pointers[i]!=node) {
            i++;
        }
        i--;
        if(i>=0) {
            ret_val=node->parent->pointers[i];
            *index=i;
        }
        else {
            *index=-1;
        }
    }
    else{
        *index=-1;
    }
    return ret_val;
}

Bplus* getRightSibling(Bplus *node, int *index) {
    Bplus *ret_val=NULL;
    if(node->parent!=NULL) {
        int i=0;
        while(i<node->parent->size && node->parent->pointers[i]!=node) {
            i++;
        }
        i++;
        if(i<=node->parent->size) {
            ret_val=node->parent->pointers[i];
            *index=i;
        }
        else {
            *index=-1;
        }
    }
    else {
        *index=-1;
    }
    return ret_val;
}

void merge(Bplus *node, int index) {
    Bplus *child1=node->pointers[index];
    Bplus *child2=node->pointers[index+1];
    child1->data.indx[child1->size]=node->data.indx[index];
    (child1->size)++;
    int j=child1->size;
    for(int i=0; i<child2->size; i++, j++) {
        child1->data.indx[j]=child2->data.indx[i];
        child1->pointers[j]=child2->pointers[i];
    }
    child1->pointers[j]=child1->pointers[child2->size];
    child1->size=(child1->size)+(child2->size);
    for(int i=index+1; i<node->size; i++) {
        node->data.indx[i-1]=node->data.indx[i];
        node->pointers[i]=node->pointers[i+1];
    }
    for(int i=0; i<=child2->size; i++) {
        if(child2->pointers[i]!=NULL) {
            child2->pointers[i]->parent=child1;
        }
    }
    (node->size)--;
    free(child2);
}

void defficiency(FILE *fptr, Bplus *node) {
    if(node!=NULL) {
        int index1, index2;
        Bplus *left=getLeftSibling(node, &index1);
        if(left!=NULL) {
            if(left->size>BPLUS_MINLIM) {
                for(int i=0; i<node->size; i++) {
                    node->data.indx[i+1]=node->data.indx[i];
                }
                node->data.indx[0]=node->parent->data.indx[index1];
                node->parent->data.indx[index1]=left->data.indx[left->size];
                (node->size)++;
                (left->size)--;
            }
            else {
                Bplus *right=getRightSibling(node, &index2);
                if(right!=NULL) {
                    if(right->size>BPLUS_MINLIM) {
                        node->data.indx[node->size]=node->parent->data.indx[index2-1];
                        node->parent->data.indx[index2-1]=right->data.indx[0];
                        for(int i=1; i<right->size; i++) {
                            right->data.indx[i-1]=right->data.indx[i];
                        }
                        (node->size)++;
                        (right->size)--;
                    }
                    else {
                        if(node->parent!=NULL) {
                            merge(node->parent, index2-1);
                            if(node->parent->size<BPLUS_MINLIM) {
                                defficiency(fptr, node->parent);
                            }
                        }
                    }
                }
                else {
                    if(node->parent!=NULL) {
                        merge(node->parent, index1-1);
                        if(node->parent->size<BPLUS_MINLIM) {
                            defficiency(fptr, node->parent);
                        }
                    }
                }
            }
        }
        else {
            Bplus *right=getRightSibling(node, &index2);
            if(right!=NULL) {
                if(right->size>BPLUS_MINLIM) {
                    node->data.indx[node->size]=node->parent->data.indx[index2-1];
                    node->parent->data.indx[index2-1]=right->data.indx[0];
                    for(int i=1; i<right->size; i++) {
                        right->data.indx[i-1]=right->data.indx[i];
                    }
                    (node->size)++;
                    (right->size)--;
                }
                else {
                    if(node->parent!=NULL) {
                        merge(node->parent, index2-1);
                        if(node->parent->size<BPLUS_MINLIM) {
                            defficiency(fptr, node->parent);
                        }
                    }
                }
            }
        }
    }
}

void deleteData(FILE *fptr, Bplus *node, int data, int index) {
    int i=binarySearch(node->data.record, node->size, data);
    if(node->data.record[i]->item_id==data) {
        item *itemNode=node->data.record[i];
        free(itemNode);
    }
    for(int j=i; j<(node->size-1); j++) {
        node->data.record[j]=node->data.record[j+1];
    }
    (node->size)--;
    if(node->size<DATANODE_MINLIM) {
        if(node->prev!=NULL && node->prev->parent==node->parent && node->prev->size>DATANODE_MINLIM) {
            item* d=node->prev->data.record[(node->prev->size)-1];
            (node->prev->size)--;
            for(int l=node->size-1; l>=0; l--) {
                node->data.record[l+1]=node->data.record[l];
            }
            node->data.record[0]=d;
            (node->size)++;
            if(index>0) {
                node->parent->data.indx[index-1]=d->item_id;
            }
        }
        else if(node->next!=NULL && node->next->parent==node->parent && node->next->size>DATANODE_MINLIM) {
            item* d=node->next->data.record[0];
            for(int j=1; j<node->next->size; j++) {
                node->next->data.record[j-1]=node->next->data.record[j];
            }
            (node->next->size)--;
            node->data.record[node->size]=d;
            (node->size)++;
            if(index<node->parent->size && node->parent->data.indx[index]<=d->item_id) {
                node->parent->data.indx[index]=node->next->data.record[0]->item_id;
            }
        }
        else {
            if(node->next!=NULL && node->next->parent==node->parent) {
                mergeDataNode(node->parent, index);
                if(node->parent->size<BPLUS_MINLIM) {
                    defficiency(fptr, node->parent);
                }
            }
            else if(node->prev!=NULL && node->prev->parent==node->parent) {
                mergeDataNode(node->parent, index-1);
                if(node->parent->size<BPLUS_MINLIM) {
                    defficiency(fptr, node->parent);
                }
            }
            else if(node->size==0) {
                if(node->next!=NULL || node->prev!=NULL) {
                    for(int j=index-1; j<(node->parent->size-1); j++) {
                        node->parent->data.indx[j]=node->parent->data.indx[j+1];
                        node->parent->pointers[j+1]=node->parent->pointers[j+2];
                    }
                    node->parent->pointers[node->parent->size-1]=node->parent->pointers[node->parent->size];
                    (node->parent->size)--;
                    if(node->parent->size<BPLUS_MINLIM) {
                        defficiency(fptr, node->parent);
                    }
                    free(node);
                }
            }
        }
    }
}

void deleteItem(FILE *fptr, Bplus **root, int data) {
    int index;
    Bplus *node=search(*root, data, &index);
    if(node==NULL) {
        fprintf(fptr, "Data not found\n");
    }
    else {
        deleteData(fptr, node, data, index);
    }
}

void deleteList(FILE *fptr, Bplus **root) {
    Bplus *ptr=getLeftmostDataNode(*root);
    while(ptr!=NULL) {
        if(ptr->size==0) {
            ptr=NULL;
        }
        else {
            deleteItem(fptr, root, ptr->data.record[0]->item_id);
            ptr=getLeftmostDataNode(*root);
        }
    }
    for(int i=0; i<M+1; i++) {
        (*root)->pointers[i]=NULL;
    }
    *root=NULL;
}