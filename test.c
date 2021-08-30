#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#define MAX_STUDENT (LIST_MAX_NUM_NODES)
typedef struct 
{
    int index;
    char student_name[20];
    int grade[10];
}test_item;

test_item g_student[MAX_STUDENT];

void test_create_list();
void test_add_trim_list();
void test_append_next_list();
void test_prev_next_list();
void test_prepend_next_list();
void test_remove_list();
void test_concat_list();
void test_search_list();


void myfree_fn(void * item)
{
    //free(item);
    return ;
}
bool mycmp(void *pItem, void *pComparisonArg)
{
    return (((test_item *)pItem)->index == ((test_item *)pComparisonArg)->index);
}

int main()
{
    test_create_list();
    test_add_trim_list();
    //test_create_list();
    test_append_next_list();

    test_prev_next_list();
    test_prepend_next_list();
    test_remove_list();
    test_concat_list();
    test_search_list();
    return 0;
}

void test_create_list()
{
    List *list[LIST_MAX_NUM_HEADS], *overflow_list;
    for(int i =0; i<LIST_MAX_NUM_HEADS; i++){
        list[i] = List_create();
        assert(list[i] != NULL);
    }
    overflow_list = List_create();
    assert(overflow_list == NULL);
    for(int i =0; i<LIST_MAX_NUM_HEADS; i++){
        fflush(stdout);
        List_free(list[i], myfree_fn);
        assert(list[i] != NULL);
    }
    printf("%s passed\n", __func__);
}

void test_add_trim_list()
{
    List *list;
    list = List_create();
    int ret;
    int len = 0;
    assert(list != NULL);
    for(int i = 0; i < MAX_STUDENT; i++)
    {
        g_student[i].index = i;
        ret = List_add(list, (void *)&g_student[i]);
        assert(ret  == 0);
        assert(((test_item *)list->head->item)->index == i); // always insert at head
    
    }
    
    ret = List_add(list, (void *)&g_student[0]);//full, insert fail
    assert(ret  == -1);
    printf("List_add passed\n");
    for(int i = 0; i<10; i++){
        List_trim(list);
        //printf("%d\n", List_count(list));
        assert(List_count(list) == MAX_STUDENT-i-1);
    }

    
    List_free(list, myfree_fn);
     printf("List_trim passed\n");
    //printf("%s passed\n", __func__);
}

void test_append_next_list()
{
    List *list;
    list = List_create();
    int ret;
    int len = 0;
    assert(list != NULL);
    for(int i = 0; i < MAX_STUDENT; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
        assert(ret  == 0);
        assert(((test_item *)list->head->item)->index == 0); // always insert at head
        assert(List_count(list) == (i+1));

        test_item *tmp = List_next(list);
        assert(tmp == NULL);
    }
    
   
    printf("List_append passed\n");
    List_first(list);
    for(int i = 0; i<10; i++){
       test_item *tmp = List_next(list);
       //printf("%x\n", tmp);
       assert(tmp != NULL && tmp->index == i+1);
        //printf("%d\n", List_count(list));
    }
    printf("List_next passed\n");
    List_free(list, myfree_fn);
}

void test_prepend_next_list()
{
    List *list;
    list = List_create();
    int ret;
    int len = 0;
    assert(list != NULL);
    for(int i = 0; i < MAX_STUDENT; i++)
    {
        g_student[i].index = i;
        ret = List_prepend(list, (void *)&g_student[i]);
        assert(ret  == 0);
        assert(((test_item *)list->head->item)->index == i); // always insert at head
        assert(List_count(list) == (i+1));

        test_item *tmp = List_prev(list);
        assert(tmp == NULL);
    }
    ret = List_prepend(list, (void *)&g_student[0]);
    assert(ret  == -1);
    
   
    printf("List_prepend passed\n");
    List_first(list);
    for(int i = 0; i<MAX_STUDENT-1; i++){
       test_item *tmp = List_next(list);
       //printf("%x\n", tmp);
       assert(tmp != NULL && tmp->index == (MAX_STUDENT-i-2));
        //printf("%d\n", List_count(list));
    }
    //printf("List_next passed\n");
    List_free(list, myfree_fn);
}

void test_prev_next_list()
{
    List *list;
    list = List_create();
    int ret;
    int len = 0;
    assert(list != NULL);
    for(int i = 0; i < 10; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
        assert(ret  == 0);
        assert(((test_item *)list->head->item)->index == 0); // always insert at head
    
    }
    List_first(list);
    test_item *tmp = List_prev(list);
    assert(tmp  == NULL);
    tmp = List_last(list);
    assert(tmp != NULL &&tmp->index == 9);
    for(int i = 9; i >0; i--){
        tmp = List_prev(list);
        assert(tmp->index == i-1);
    }
    tmp = List_prev(list);
    assert(tmp  == NULL);
    printf("List_prev passed\n");

    List_free(list, myfree_fn);
}

void test_remove_list()
{
    List *list;
    list = List_create();
    int ret;
    int len = 0;
    test_item *tmp;
    assert(list != NULL);
    for(int i = 0; i < 20; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
        assert(ret == 0);
        assert(((test_item *)list->head->item)->index == 0); // always insert at head
    
    }
    List_first(list);
    for(int i = 0; i < 20; i++)
    {
        tmp = List_remove(list);
         assert(tmp->index == i);
         assert(List_count(list) == 19-i);
    }
    tmp = List_remove(list);
    assert(tmp == NULL);
    printf("List_remove 1 passed\n");
    for(int i = 0; i < 20; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
        assert(ret == 0);
        assert(((test_item *)list->head->item)->index == 0); // always insert at head
    
    }
    List_last(list);
    tmp = List_remove(list);
    assert(tmp -> index = 19);
    assert(List_count(list) == 19);
    tmp = List_remove(list);
    assert(!tmp);

    printf("List_remove 2 passed\n");
    List_free(list, myfree_fn);
}

void test_concat_list()
{
    List *list, *concat_list;
    
    int ret;
    int len = 0;
    test_item *tmp;
    list = List_create();
    concat_list = List_create();
    assert(list != NULL);
    assert(concat_list != NULL);
    for(int i = 0; i < 20; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
    }
    for(int i = 20; i < 30; i++)
    {
        g_student[i].index = i;
        ret = List_append(concat_list, (void *)&g_student[i]);
        assert(ret == 0);
    }
    List_concat(list, concat_list);
    assert(List_count(list) == 30);
    List_first(list);
    for(int i = 0; i < 30; i++){
        tmp = List_curr(list);
        List_next(list);
        assert(tmp->index == i);

    }
    List_free(list, myfree_fn);
    //List_free(list, myfree_fn);
    printf("List concat passed\n");

}

void test_search_list()
{
    List *list, *concat_list;
    int ret;
    int len = 0;
    test_item *tmp;
    list = List_create();
    assert(list != NULL);
    for(int i = 0; i < 20; i++)
    {
        g_student[i].index = i;
        ret = List_append(list, (void *)&g_student[i]);
    }
    List_first(list);
    tmp = List_search(list, mycmp, (void *)&g_student[10]);
    assert(tmp != NULL && tmp->index == 10);
    g_student[50].index = 1000;
    tmp = List_search(list, mycmp, &g_student[50]);
    assert(tmp == NULL);
    List_first(list);
    for(int i = 0; i < 9; i++){
        List_next(list);
    }
    tmp = List_search(list, mycmp, (void *)&g_student[10]);
    assert(tmp != NULL && tmp->index == 10);
    tmp = List_search(list, mycmp, (void *)&g_student[9]);
    assert(!tmp);
    tmp = List_search(list, mycmp, (void *)&g_student[15]);
    assert(!tmp);
    List_free(list, myfree_fn);
    //List_free(list, myfree_fn);
    printf("List search passed\n");
}