#include "public.h"

/* display list from top */
void dump(struct node* list)
{
    while(list->next != NULL)
    {
        printf("0x%04X %s\n", list->address, list->label);
        list = list->next;
    }
}

/* delete list top from top */
void delt(struct node* list)
{
    while(list->next != NULL)
    {
        struct node* next = list->next;
        free(list->label);
        free(list);
        list = next;
    }
    free(list->label);
    free(list);
}

/* hash find function where label is key */
int find(struct node* list, const char* label)
{
    struct node* temp = list;
    while(temp->next != NULL)
    {
        if(strcmp(temp->label, label) == 0)
            return temp->address;
        temp = temp->next;
    }
    return -1;
}

/* insert node at top of list */
struct node* push(struct node* list, int address, char* label)
{
    struct node* temp = malloc(sizeof(struct node));
    temp->address = address;
    temp->label = strdup(label);
    temp->next = list;
    return temp;
}
