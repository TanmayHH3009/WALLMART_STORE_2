/*
    Created by :
        Tanmay Hatwar
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include "CustomerInvoice.h"
# include "Store_Management.h"
# include "Map.h"

int aisle_index=0;

item* create_item(int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price) {
    item *new_Item=(item*)malloc(sizeof(item));
    new_Item->item_id=id;
    new_Item->item_name=(char*)malloc(MAX_NAME_SIZE);
    strcpy(new_Item->item_name, name);
    new_Item->quantity=quantity;
    for(int i=0; i<3; i++) {
        new_Item->expiry_date[i]=expiry_date[i];
    }
    new_Item->threshold_quantity=threshold_quantity;
    new_Item->price=price;
    new_Item->next=NULL;
    new_Item->frequency=0;
    return new_Item;
}

aisle* create_aisle(int id, char *product_type) {
    aisle *new_aisle=(aisle*)malloc(sizeof(aisle));
    new_aisle->aisle_id=id;
    new_aisle->product_type=(char*)malloc(MAX_NAME_SIZE);
    strcpy(new_aisle->product_type, product_type);
    new_aisle->root=NULL;
    return new_aisle;
}

void insert_item(int index,item* item_node) {
    Bplus *curr=aisle_array[index]->root;
    if(curr==NULL) {
        aisle_array[index]->root=initialize();

    }
    insert(&(aisle_array[index]->root), aisle_array[index]->root, item_node);
}

void add_item(char *product_type, int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price) {
    item *new_Item=create_item(id, name, quantity, expiry_date, threshold_quantity, price);
    if(new_Item ==NULL) {
        printf("Cannot create node for %d\n", id);
    }
    int index=(new_Item->item_id/10000)-1;
    if(aisle_array[index]==NULL) {
        aisle_array[index]=create_aisle(index+1, product_type);
        aisle_index=index+1;
    }
    insert_item(index, new_Item);
}

void print_Item(FILE *fptr, item* curr) {
    if(curr != NULL) {
        int aisle_id = (curr->item_id) / 10000;
        fprintf(fptr, "Aisle ID: %d\tItem ID: %d\tPrice: %.2f\t", aisle_id, curr->item_id, curr->price);
        fprintf(fptr, "Item: %s\tQuantity: %d\t", curr->item_name, curr->quantity);
        fprintf(fptr, "Threshold Quantity: %d\t", curr->threshold_quantity);
        fprintf(fptr, "Expiry Date: %02d-%02d-%04d\n", curr->expiry_date[0], curr->expiry_date[1], curr->expiry_date[2]);
    }
}


void delete_item(FILE *fptr, int id) {
    int index=(id/10000)-1;
    if(aisle_array[index]==NULL) {
        fprintf(fptr, "No item of aisle_id:%d exists\n", index+1);
    }
    else {
        deleteItem(fptr, &(aisle_array[index]->root), id);
    }
}

void add_aisle(char *product_type) {
    int i=0;
    while(i<aisle_index && aisle_array[i]!=NULL && strcmp(aisle_array[i]->product_type, product_type)!=0) i++;
    if(i==aisle_index) {
        aisle_array[aisle_index]=create_aisle(aisle_index+1, product_type);
        aisle_index++;
    }
    else if(aisle_array[i]==NULL) {
        aisle_array[i]=create_aisle(i+1, product_type);
    }
}

void delete_aisle(FILE *fptr, int id) {
    int index=id-1;
    if(aisle_array[index]==NULL) {
        fprintf(fptr, "No aisle with id %d exists\n", index+1);
    }
    else {
        deleteList(fptr, &(aisle_array[index]->root));
        free(aisle_array[index]->root);
        free(aisle_array[index]);
        aisle_array[index]=NULL;
    }
}

boolean compare_dates(int expiry_date[], int required_date[], int curr_date[]) {
    boolean ret_val=FALSE;
    if(curr_date[2]<expiry_date[2] && expiry_date[2]<required_date[2]) {
        ret_val=TRUE; 
    }
    else if(expiry_date[2]==required_date[2] ) {
        if(expiry_date[1]==required_date[1]) {
            if(expiry_date[0]<=required_date[0]) {
                if(curr_date[2]<expiry_date[2]) {
                    ret_val=TRUE;
                }
                else if(curr_date[2]==expiry_date[2] ) {
                    if(curr_date[1]==expiry_date[1] && expiry_date[0]>=curr_date[0]) {
                        ret_val=TRUE;
                    }
                    else if(curr_date[1]<expiry_date[1]) {
                        ret_val=TRUE;
                    }
                }
            }
        }
        else if(expiry_date[1]<required_date[1]) {
            if(curr_date[2]<expiry_date[2]) {
                ret_val=TRUE;
            }
            else if(curr_date[2]==expiry_date[2] ) {
                if(curr_date[1]==expiry_date[1] && expiry_date[0]>=curr_date[0]) {
                    ret_val=TRUE;
                }
                else if(curr_date[1]<expiry_date[1]) {
                    ret_val=TRUE;
                }
            }
        }
    }
    return ret_val;
}

void check_availability(FILE* fptr, int item_id, int req_quantity, int delivery_date[], int curr_date[]) {
    item *itemNode=searchItem(item_id);
    if(itemNode==NULL) {
        fprintf(fptr, "No item with id %d exists\n", item_id);
    }
    else {
        if(itemNode->quantity>req_quantity) {
            if(compare_dates(itemNode->expiry_date, delivery_date, curr_date)) fprintf(fptr, "Status : Not Available\n");
            else fprintf(fptr, "Status : Avaliable\n");
            print_Item(fptr, itemNode);
        }
        else {
                fprintf(fptr, "Status : Not Avaliable\n");
                print_Item(fptr, itemNode);
            }
        }
    
}

void inspect_quantity(FILE *fptr) {
    fprintf(fptr, "\n\nItems needed to be ordered : \n\n");
    for(int i=0; i<aisle_index; i++) {
        if(aisle_array[i]!=NULL) {
            Bplus *curr=getLeftmostDataNode(aisle_array[i]->root);
            while(curr!=NULL) {
                for(int i=0; i<curr->size; i++) {
                    if(curr->data.record[i]->quantity<curr->data.record[i]->threshold_quantity) {
                        print_Item(fptr, curr->data.record[i]);
                    }
                }
                curr=curr->next;
            }
        }
    }
}

void find_date(int curr_date[], int required_date[], int day) {
    if(curr_date[1]==1 || curr_date[1]==3 || curr_date[1]==5 || curr_date[1]==7 || curr_date[1]==8 || curr_date[1]==10 || curr_date[1]==12) {
        required_date[0]=(curr_date[0]+day)%31;
        if(required_date[0]==0) required_date[0]=31;
        if(curr_date[0]+day>31) {
            required_date[1]=(curr_date[1]+1)%12;
            if(required_date[1]==0) required_date[1]=12;
            if(curr_date[1]==12) {
                required_date[2]=curr_date[2]+1;
            }
        }
        else {
            required_date[1]=curr_date[1];
            required_date[2]=curr_date[2];
        }
    }
    else if(curr_date[1]==2) {
        if(curr_date[2]%4==0) {
            required_date[0]=(curr_date[0]+day)%29;
            if(required_date[0]==0) required_date[0]=29;
            if(curr_date[0]+day>29) {
                required_date[1]=(curr_date[1]+1)%12;
                required_date[2]=(curr_date[2]);
            }
            else {
                required_date[1]=curr_date[1];
                required_date[2]=curr_date[2];
            }
        }
        else {
            required_date[0]=(curr_date[0]+day)%28;
            if(required_date[0]==0) required_date[0]=28;
            if(curr_date[0]+day>28) {
                required_date[1]=(curr_date[1]+1)%12;
                required_date[2]=curr_date[2];
            }
            else {
                required_date[1]=curr_date[1];
                required_date[2]=curr_date[2];
            }
        }
    }
    else {
        required_date[0]=(curr_date[0]+day)%30;
        if(required_date[0]==0) required_date[0]=30;
        if(curr_date[0]+day>30) {
            required_date[1]=(curr_date[1]+1)%12;
            if(required_date[1]==0) required_date[1]=12;
            required_date[2]=curr_date[2];
        }
        else {
            required_date[1]=curr_date[1];
            required_date[2]=curr_date[2];
        }
    }
}

void inspect_readyToEat(FILE *fptr, int curr_date[]) {
    Bplus *curr=getLeftmostDataNode(aisle_array[3]->root);
    int required_date[3];
    find_date(curr_date, required_date, 7);
    while(curr!=NULL) {
        for(int i=0; i<curr->size; i++) {
            if(compare_dates(curr->data.record[i]->expiry_date, required_date, curr_date)) {
                print_Item(fptr, curr->data.record[i]);
                fprintf(fptr, "(WARNING expiring within one week)\n\n");
            }
            else {
                print_Item(fptr, curr->data.record[i]);
                fprintf(fptr, "\n");
            }
        }
        curr=curr->next;
    }
}

void inspect_dairyProduct(FILE *fptr, int curr_date[]) {
    Bplus *curr=getLeftmostDataNode(aisle_array[0]->root);
    int required_date[3];
    find_date(curr_date, required_date, 1);
    while(curr!=NULL) {
        for(int i=0; i<curr->size; i++) {
            if(compare_dates(curr->data.record[i]->expiry_date, required_date, curr_date)) {
                print_Item(fptr, curr->data.record[i]);
                fprintf(fptr, "(WARNING expiring within one day)\n\n");
            }
            else {
                print_Item(fptr, curr->data.record[i]);
                fprintf(fptr, "\n");
            }
        }
        curr=curr->next;
    }
}

void merge_aisle(FILE *fptr, char *product_type, int id1, int id2) {
    int index1=id1-1;
    int index2=id2-1;
    aisle *curr_aisle=aisle_array[index1];
    if(curr_aisle==NULL) {
        if(aisle_array[index2]!=NULL) {
            aisle_array[index1]=create_aisle(id1, product_type);
            aisle_array[index1]->root=aisle_array[index2]->root;
        }
    }
    else {
        strcpy(curr_aisle->product_type, product_type);
        if(aisle_array[index2]!=NULL) {
            Bplus *curr=getLeftmostDataNode(aisle_array[index2]->root);
            while(curr!=NULL) {
                if(curr->size==0) {
                    curr=NULL;
                }
                else {
                    item *node=curr->data.record[0];
                    item *itemNode=create_item(node->item_id, node->item_name, node->quantity, node->expiry_date, node->threshold_quantity, node->price);
                    deleteItem(fptr, &(aisle_array[index2]->root), curr->data.record[0]->item_id);
                    insert(&(aisle_array[index1]->root), aisle_array[index1]->root, itemNode);
                    curr=getLeftmostDataNode(aisle_array[index2]->root);
                }
            }
            if(aisle_array[index2]->root!=NULL) {
                free(aisle_array[index2]->root);
            }
            free(aisle_array[index2]);
            aisle_array[index2]=NULL;
        }
    }
}

void print_aisle(FILE *fptr, aisle *a) {
    Bplus *curr=a->root;
    if(a->root==NULL) {
        fprintf(fptr, "No item exists\n\n");
    }
    else {
        printList(fptr, a->root);
    }
}

item* get_item(int item_id, int req_quantity) {
    item* ret_val=searchItem(item_id);
    if(ret_val!=NULL && ret_val->quantity<req_quantity) {
        ret_val=NULL;
    }
    return ret_val;
}

void purchase_item(int item_id, int req_quantity, int userId) {
    int index=(userId/10000)-1;
    item *ret_item=get_item(item_id, req_quantity);
    if(ret_item!=NULL) {
        item *new_item=create_bill_item(ret_item->item_id, ret_item->item_name, req_quantity, ret_item->expiry_date, ret_item->price);
        if(Invoice[index].bill_root==NULL) {
            Invoice[index].bill_root=initialize();
        }
        insert(&(Invoice[index].bill_root), Invoice[index].bill_root, new_item);
        ret_item->quantity=ret_item->quantity-req_quantity;
        Invoice[index].total_cost=Invoice[index].total_cost+(ret_item->price)*req_quantity;
    }
}

void purchase_item_user(int id1, int q1, int id2, int q2, int id3, int q3, int id4, int q4, int id5, int q5, int userId) {
    purchase_item(id1, q1, userId);
    purchase_item(id2, q2, userId);
    purchase_item(id3, q3, userId);
    purchase_item(id4, q4, userId);
    purchase_item(id5, q5, userId);
}

void rangeSearch(FILE *fptr, int aisle_id, int id1, int id2) {
    Bplus *ptr=aisle_array[aisle_id-1]->root;
    while(ptr!=NULL && !(ptr->isLeaf)) {
        int i=0;
        while(i<ptr->size && ptr->data.indx[i]<=id1) {
            i++;
        }
        ptr=ptr->pointers[i];
    }
    fprintf(fptr, "\n\nItems with id in range <%d - %d> present in aisle %d : \n", id1, id2, aisle_id);
    if(ptr!=NULL) {
        while(ptr!=NULL && ptr->data.record[0]->item_id<=id2) {
            for(int i=0; i<ptr->size; i++) {
                if(ptr->data.record[i]->item_id>=id1 && ptr->data.record[i]->item_id<=id2) {
                    print_Item(fptr, ptr->data.record[i]);
                }
            }
            ptr=ptr->next;
        }
    }
}

int main() {

    FILE *fptr1=fopen("./input.txt", "r");
    FILE *fptr2=fopen("./output.txt", "w");
    FILE *fptr3=fopen("Purchase_input.txt", "r");
    FILE *fptr4=fopen("Bill_output.txt", "w");
    FILE *fptr5=fopen("./aisle_Info.txt", "w");
    FILE *fptr6=fopen("./AvailabilityCheck.txt", "w");

    if(fptr1==NULL) printf("Cannot read input.txt\n");
    else if(fptr2==NULL) printf("Cannot open output.txt\n");
    else if(fptr3==NULL) printf("cannot read Purchase_input.txt\n");
    else if(fptr4==NULL) printf("cannot open Bill_output.txt");
    else if(fptr5==NULL) printf("cannot open aisle_Info.txt");
    else if(fptr6==NULL) printf("cannot open AvailabilityCheck.txt");
    else {

        initialize_bill();
        initialize_map();

        char product_type[MAX_NAME_SIZE];
        while(fscanf(fptr1,"%s", product_type)!=EOF) {
            fgetc(fptr1);
            int id, quantity, expiry_date[3], threshold_q;
            float price;
            fscanf(fptr1,"%d", &id);
            fgetc(fptr1);
            char name[MAX_NAME_SIZE];
            fscanf(fptr1,"%s", name);
            fgetc(fptr1);
            fscanf(fptr1,"%d", &quantity);
            fgetc(fptr1);
            for(int i=0; i<3; i++) {
                fscanf(fptr1,"%d", &expiry_date[i]);
                fgetc(fptr1);
            }
            fscanf(fptr1,"%d", &threshold_q);
            fgetc(fptr1);
            fscanf(fptr1,"%f", &price);
            fgetc(fptr1);
            add_aisle(product_type);
            add_item(product_type, id, name, quantity, expiry_date, threshold_q, price);
        }

        fprintf(fptr5, "____________Printing aisle____________\n\n");
for(int i=0; i<aisle_index; i++) {
    fprintf(fptr5, "Product Type : %s\n", aisle_array[i]->product_type);
    print_aisle(fptr5, aisle_array[i]);
    fprintf(fptr5, "\n\n");
}

int curr_date[3];
printf("Enter today's date : ");
for(int i=0; i<3; i++) {
    scanf("%d", &curr_date[i]);
}


        int flag=1;
        while(flag) {
            printf("\n\nEnter the key for corresponding option : \n");
            printf("1 : Print a particular aisle info\n");
            printf("2 : Print a particular item info\n");
            printf("3 : Print all aisle info\n");
            printf("4 : Add an aisle\n");
            printf("5 : Add an item\n");
            printf("6 : Delete an aisle\n");
            printf("7 : Delete an item\n");
            printf("8 : Check availability of a particular item\n");
            printf("9 : Inspect for quantity of items\n");
            printf("10 : Inspect dairy products\n");
            printf("11 : Inspect ready-to-eat items\n");
            printf("12 : Merge aisle\n");
            printf("13 : Generate bill for users\n");
            printf("14 : Calculate frequency of items\n");
            printf("15 : Search all items in a particular aisle in the range between two given item_ids\n");
            printf("(Enter -1 to Exit)\n\n");
            int key;
            printf("Enter the key : ");
            scanf("%d", &key);
            int id, quantity, expiry_date[3], threshold_q;
            float price;
            char product_type[MAX_NAME_SIZE], name[MAX_NAME_SIZE];
            int date[3];
            int aisle_id1, aisle_id2;
            char str[MAX_NAME_SIZE];
            int id1, q1, id2, q2, id3, q3, id4, q4, id5, q5;
            int userId;
            switch(key) {
                case 1 : 
                         printf("Enter the aisle id : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "____________Printing aisle %d____________\n\n", id);
                         fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[id-1]->product_type);
                         print_aisle(fptr5, aisle_array[id-1]);
                         fprintf(fptr5, "\n\n");
                         break;
                case 2 : 
                         printf("Enter the item id : ");
                         scanf("%d", &id);
                         fprintf(fptr2, "____________Printing item with id <%d>____________\n\n", id);
                         item* curr=searchItem(id);
                         print_Item(fptr2, curr);
                         break;
                case 3 : 
                         fprintf(fptr5, "____________Printing aisle____________\n\n");
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                fprintf(fptr5, "Product Type : %s\n", aisle_array[i]->product_type);
                                print_aisle(fptr5, aisle_array[i]);
                                fprintf(fptr5, "\n\n");
                             }
                         }
                         break;
                case 4 : 
                         printf("Enter the product type : ");
                         scanf("%s", product_type); 
                         add_aisle(product_type);
                         break; 
                case 5 : 
                         printf("Enter product type : ");
                         scanf("%s", product_type);
                         printf("Enter id : ");
                         scanf("%d", &id);
                         printf("Enter name : ");
                         scanf("%s", name);
                         printf("Enter quantity : ");
                         scanf("%d", &quantity);
                         printf("Enter expiry_date : ");
                         for(int i=0; i<3; i++) {
                            scanf("%d", &expiry_date[i]);
                         }
                         printf("Enter threshold quantity : ");
                         scanf("%d", &threshold_q);
                         printf("Enter price : ");
                         scanf("%f", &price);
                         add_item(product_type, id, name, quantity, expiry_date, threshold_q, price);
                         break;
                case 6 : 
                         printf("Enter the aisle id to be deleted : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "\n\n\n____________Deleting aisle %d____________\n", id);
                         delete_aisle(fptr5, id); 
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 7 : 
                         printf("Enter the item id to be deleted : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "\n\n\n____________Deleting item with id <%d>____________\n", id);
                         delete_item(fptr5, id); 
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 8 : 
                         printf("Enter the item id whose availability is to be checked : ");
                         scanf("%d", &id);
                         printf("Enter the required quantity : ");
                         scanf("%d", &quantity);
                         printf("Enter the expected delivery date : ");
                         for(int i=0; i<3; i++) {
                            scanf("%d", &date[i]);
                         }
                         fprintf(fptr6, "____________Checking for availability of item with id <%d>____________\n\n", id);
                         check_availability(fptr6, id, quantity, date, curr_date);
                         fprintf(fptr6, "\n\n\n");
                         break;
                case 9 : 
                         fprintf(fptr2, "\n____________Inspecting for quantity of items____________\n");
                         inspect_quantity(fptr2);
                         break;
                case 10: 
                         fprintf(fptr2, "\n\n\n____________Inspecting dairy products____________\n\n");
                         inspect_dairyProduct(fptr2, curr_date);
                         break;
                case 11: 
                         fprintf(fptr2, "\n\n\n____________Inspecting ready-to-eat products____________\n\n");
                         inspect_readyToEat(fptr2, curr_date);
                         break;
                case 12: 
                         printf("Enter the aisle ids to be merged : ");
                         scanf("%d %d", &aisle_id1, &aisle_id2);
                         printf("Enter the name of the merged aisle : ");
                         scanf("%s", str);
                         fprintf(fptr5, "\n\n\n____________Merging aisle %d to aisle %d____________\n", aisle_id2, aisle_id1);
                         merge_aisle(fptr5, str, aisle_id1, aisle_id2);
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 13: 
                         fprintf(fptr2, "\n\n\n____________Purchasing items____________\n\n");
                         while(fscanf(fptr3, "%d %d %d %d %d %d %d %d %d %d %d", &id1, &q1, &id2, &q2, &id3, &q3, &id4, &q4, &id5, &q5, &userId)!=EOF) {
                             fgetc(fptr3);
                             purchase_item_user(id1, q1, id2, q2, id3, q3, id4, q4, id5, q5, userId);
                         }
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr2, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr2, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr2, "\n<NULL>\n\n");
                             }
                         }
                         print_bill(fptr4, 12345);
                         print_bill(fptr4, 23456);
                         print_bill(fptr4, 34567);
                         print_bill(fptr4, 45678);
                         print_bill(fptr4, 56789);
                         print_bill(fptr4, 67890); 
                         break;
                case 14: 
                         for(int i=0; i<6; i++) {
                             update_frequency(Invoice[i].bill_root);
                         }
                         fprintf(fptr2, "\n\n\n____________Printing the frequencies____________\n\n");
                         printf("Enter the item id for which frequently boughts are to be printed (Note:enter 1 to print for all items): ");
                         scanf("%d", &id);
                         if(id==1) {
                             print_frequency(fptr2, 11234);
                             print_frequency(fptr2, 21345);
                             print_frequency(fptr2, 15232);
                             print_frequency(fptr2, 44212);
                             print_frequency(fptr2, 51233);
                             print_frequency(fptr2, 24345);
                             print_frequency(fptr2, 41456);
                             print_frequency(fptr2, 42881);
                             print_frequency(fptr2, 42456);
                             print_frequency(fptr2, 55234);
                             print_frequency(fptr2, 12234);
                         }
                         else {
                             print_frequency(fptr2, id);
                         }
                         break;
                case 15: 
                         fprintf(fptr2, "\n\n\n____________Printing itemsin particular range____________\n\n");
                         printf("Enter the aisle id in which items need to be searched : ");
                         scanf("%d", &aisle_id1);
                         printf("Enter the range(Note:Enter the lower limit first and enter space in between numbers no other symbols): ");
                         scanf("%d %d", &id1, &id2);
                         rangeSearch(fptr2, aisle_id1, id1, id2);
                         break;
                case -1: 
                         flag=0;
                         break;
                default: 
                         printf("Enter a valid key \n");
            }
        }
    }
    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);
    fclose(fptr4);
    fclose(fptr5);
    fclose(fptr6);
    return 0;
}