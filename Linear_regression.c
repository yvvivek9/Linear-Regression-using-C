#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef struct house{
    int cost;
    int area;
    struct house* next;
}house;

typedef struct database{
    char description[MAX];
    int M;
    int C;
    struct house* list;
    struct database* next;
}database;

house* init_list()
{
    int area; int cost;
    printf("\t\tEnter area and cost: ");
    scanf("%d %d", &area, &cost);
    if(area == 0 && cost == 0){
        return NULL;
    }
    else{
        house* list = (house*)malloc(sizeof(house));
        list->area = area;
        list->cost = cost;
        list->next = init_list();
        return list;
    }
}

void linear_regression(database* data)
{
    int M; int C; int d; int n = 0;
    int sumx = 0; int sumy = 0;
    int sumxsq = 0; int sumxy = 0;
    house* list = data->list;
    while(list != NULL){
        sumx += list->area;
        sumxsq += (list->area*list->area);
        sumy += list->cost;
        sumxy += (list->area*list->cost);
        list = list->next;
        n += 1;
    }
    d = n*sumxsq-sumx*sumx;
    M = (n*sumxy-sumx*sumy)/d;
    C = (sumy*sumxsq-sumx*sumxy)/d;
    data->M = M; data->C = C;
}

void delete_list(house* list)
{
    if(list != NULL){
        delete_list(list->next);
        free(list);
    }
}

database* init_database(database* data, char description[MAX])
{
    database* newData = (database*)malloc(sizeof(database));
    strcpy(newData->description, description);
    printf("\tCreating a list for houses...... \n");
    printf("\tNote:- Enter (0 0) to end list | Make sure you do more than 2 entries\n");
    newData->list = init_list();
    newData->next = NULL;
    linear_regression(newData);
    if(data == NULL)
        return newData;
    else{
        database* temp = data;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = newData;
        return data;
    }
}

void edit_database(database* data)
{
    printf("\tDeleting old list of houses......\n");
    delete_list(data->list);
    printf("\tCreating a new list of houses....\n");
    printf("\tNote:- Enter (0 0) to end list | Make sure you do more than 2 entries\n");
    data->list = init_list();
    linear_regression(data);
}

database* delete_database(database* data, database* to_be_deleted)
{
    if(data == to_be_deleted){
        database* temp = data->next;
        delete_list(data->list);
        free(data);
        return temp;
    }
    else{
        database* temp = data;
        while(temp->next != to_be_deleted)
            temp = temp->next;
        if(to_be_deleted->next == NULL){
            delete_list(to_be_deleted->list);
            free(to_be_deleted);
            temp->next = NULL;
        }
        else{
            temp->next = to_be_deleted->next;
            delete_list(to_be_deleted->list);
            free(to_be_deleted);
        }
        return data;
    }
}

void open_database(database* data)
{
    int area;
    int price;
    printf("\tDescription: %s\n", data->description);
    printf("\tAreas: ");
    house* areas = data->list;
    while(areas != NULL){
        printf("%d  ", areas->area);
        areas = areas->next;
    }
    printf("\n");
    printf("\tCosts: ");
    house* costs = data->list;
    while(costs != NULL){
        printf("%d  ", costs->cost);
        costs = costs->next;
    }
    printf("\n");
    printf("\tLine of best fit: (Price) = %d(Area) + %d", data->M, data->C);
    printf("\n");
    printf("Enter 0 to go back or enter Area to predict price: ");
    scanf("%d", &area);
    while(area != 0){
        price = area*data->M + data->C;
        printf("\tPrice of the house is estimated around: %d\n", price);
        printf("\tEnter 0 to go back or enter another Area to predict price: ");
        scanf("%d", &area);
    }
}

void list_database(database* data)
{
    if(data == NULL)
        printf("\tNo Database found\n");
    else{
        int i = 1;
        while(data != NULL){
            printf("\t%d) %s\n", i++, data->description);
            data = data->next;
        }
    }
}

database* ret_database(database* data, int pos)
{
    for(int i = 1; i < pos; i++){
        if(data == NULL){
            printf("\tInvalid Input!\n");
            return NULL;
        }
        data = data->next;
    }
    return data;
}

int main()
{
    printf("\t\t\tLINEAR REGRESSION USING C\n\n");
    printf("- This program takes user input of area's and price's of houses and uses linear regression algorithm to predict prices of houses given their area.\n");
    printf("- Created By - Taavu tamma hesaru haakolli");
    database* data = NULL;
    int ch;
    char description[MAX];
    int sno;
    while(1){
        printf("\n\n");
        printf("\t[1] List Databases\n");
        printf("\t[2] Open a Database\n");
        printf("\t[3] Add a Database\n");
        printf("\t[4] Edit a Database\n");
        printf("\t[5] Delete a database\n");
        printf("\t[0] Exit\n");
        printf("\n\tEnter your choice: ");
        scanf("%d", &ch);
        printf("\n");
        switch(ch){
            case 1:
                list_database(data);
                break;
            case 2:
                list_database(data);
                printf("\tEnter S.No for database: ");
                scanf("%d", &sno);
                if(ret_database(data, sno) != NULL)
                    open_database(ret_database(data, sno));
                break;
            case 3:
                printf("\tEnter title for database: ");
                scanf("\n%[^\n]s", description);
                data = init_database(data, description);
                printf("\tDatabase created! \n");
                break;
            case 4:
                list_database(data);
                printf("\tEnter S.No for database: ");
                scanf("%d", &sno);
                if(ret_database(data, sno) != NULL){
                    edit_database(ret_database(data, sno));
                    printf("\tDatabase edited! \n");
                }
                break;
            case 5:
                list_database(data);
                printf("\tEnter S.No for database: ");
                scanf("%d", &sno);
                if(ret_database(data, sno) != NULL){
                    data = delete_database(data, ret_database(data, sno));
                    printf("\tDatabase deleted! \n");
                }
                break;
            case 0:
                exit(0);
        }
    }
}