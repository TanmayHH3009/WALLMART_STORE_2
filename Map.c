/*
    Created by :
        Tanmay Hatwar
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<limits.h>
# include"Map.h"

void initialize_map() {
    for(int i=0; i<MAP_SIZE; i++) {
        map[i].itemNode=NULL;
        map[i].root=NULL;
    }
}

int hash_function(int item_id) {
    int ans=0;
    for(int i=5; i>=1; i--) {
        ans=ans+(i)*((item_id)%10);
        item_id=(item_id)/10;
    }
    return ans%MAP_SIZE;
}

item* searchItemMap(Bplus *root, int data) {
    item *ret_val=NULL;
    Bplus *ptr=root;
    while(ptr!=NULL && !(ptr->isLeaf)) {
        int i=0;
        while(i<ptr->size && ptr->data.indx[i]<=data) {
            i++;
        }
        ptr=ptr->pointers[i];
    }
    if(ptr!=NULL) {
        int index=binarySearch(ptr->data.record, ptr->size, data);
        if(ptr->data.record[index]!=NULL && ptr->data.record[index]->item_id==data) {
            ret_val=ptr->data.record[index];
        }
    }
    return ret_val;
}

void update_frequency(Bplus *Billroot) {
    Bplus *curr=getLeftmostDataNode(Billroot);
    while(curr!=NULL) {
        for(int i=0; i<curr->size; i++) {
            Bplus *node=getLeftmostDataNode(Billroot);
            int row=hash_function(curr->data.record[i]->item_id);
            map[row].itemNode=curr->data.record[i];
            while(node!=NULL) {
                for(int j=0; j<node->size; j++) {
                    if(node->data.record[j]->item_id!=curr->data.record[i]->item_id) {
                        item *it_node=searchItemMap(map[row].root, node->data.record[j]->item_id);
                        if(map[row].root==NULL) {
                            map[row].root=initialize();
                            item *currNode=node->data.record[j];
                            item *newNode=create_item(currNode->item_id, currNode->item_name, currNode->quantity, currNode->expiry_date, currNode->threshold_quantity, currNode->price);
                            newNode->frequency=1;
                            insert(&map[row].root, map[row].root, newNode);
                        }
                        else {
                            if(it_node!=NULL) {
                                (it_node->frequency)++;
                            }
                            else {
                                item *currNode=node->data.record[j];
                                item *newNode=create_item(currNode->item_id, currNode->item_name, currNode->quantity, currNode->expiry_date, currNode->threshold_quantity, currNode->price);
                                newNode->frequency=1;
                                insert(&map[row].root, map[row].root, newNode);
                            }
                        }
                    }
                }
                node=node->next;
            }
        }
        curr=curr->next;
    }
}

void print_frequency(FILE *fptr, int item_id) {
    int index=hash_function(item_id);
    int maxi=INT_MIN;
    Bplus *head=getLeftmostDataNode(map[index].root);
    fprintf(fptr, "Printing for %s : \n", map[index].itemNode->item_name);
    Bplus *curr=head;
    while(curr!=NULL) {
        for(int i=0; i<curr->size; i++) {
            if(curr->data.record[i]->frequency>maxi) {
                maxi=curr->data.record[i]->frequency;
            }
        }
        curr=curr->next;
    }
    curr=head;
    while(curr!=NULL) {
        for(int i=0; i<curr->size; i++) {
            if(curr->data.record[i]->frequency==maxi) {
                fprintf(fptr, "Item : %s\t\tItem_id : %d\n", curr->data.record[i]->item_name, curr->data.record[i]->item_id);
            }
        }
        curr=curr->next;
    }
    fprintf(fptr, "\n\n");
}