#include "list.h"
#include <malloc.h>
#include <assert.h>
static Node g_node[LIST_MAX_NUM_NODES];
static List g_list[LIST_MAX_NUM_HEADS];
static int node_num = 0;
List *List_create()
{
    List *p = NULL;
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++)
    {
        if (!g_list[i].occupied)
        {
            g_list[i].occupied = true;
            p = &g_list[i];
            break;
        }
    }
    if (p)
    {
        p->cur = p->head = p->tail = NULL;
        p->cur_state = LIST_OOB_START;
    }
    return p;
}

// Returns the number of items in pList.
int List_count(List *pList)
{
    int len = 0;
    Node *head = pList->head, *start = head;
    if (head == NULL)
        return len;
    do
    {
        ++len;
        head = head->next;
    } while (head != start);
    return len;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_first(List *pList)
{
    assert(pList != NULL);
    if (!pList->head)
        return NULL;
    pList->cur = pList->head;
    return pList->head->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_last(List *pList)
{
    if (!pList->tail)
        return NULL;
    pList->cur = pList->tail;
    return pList->tail->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond end of pList.
void *List_next(List *pList) //if cur is before the start of list?
{
    if (!pList->head)
        return NULL;
    else if (pList->cur == NULL && pList->cur_state == LIST_OOB_END)
        return NULL;
    else if (pList->cur == NULL && pList->cur_state == LIST_OOB_START)
        return List_first(pList);
    else if (pList->cur == pList->tail)
    {
        pList->cur = NULL;
        pList->cur_state = LIST_OOB_END;
        return NULL;
    }
    else
        pList->cur = pList->cur->next;
    return pList->cur->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void *List_prev(List *pList)
{
    if (!pList->head)
        return NULL;
    if(!pList->cur){
        //if(pList->cur_state == LIST_OOB_START)
            return NULL;
    }
    if (pList->cur == pList->head)
    {
        pList->cur = NULL;
        pList->cur_state = LIST_OOB_START;
        return NULL;
    }

    pList->cur = pList->cur->prev;
    return pList->cur->item;
}

// Returns a pointer to the current item in pList.
void *List_curr(List *pList)
{
    if (pList->cur)
        return pList->cur->item;
    else
        return NULL;
}

static Node *alloc_node(void *pItem)
{
    Node *ret = NULL;
    if (node_num >= LIST_MAX_NUM_NODES)
    {
        return ret;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++)
    {
        if (!g_node[i].occupied)
        {
            g_node[i].occupied = true;
            g_node[i].next = g_node[i].prev = NULL;
            g_node[i].item = pItem;
            ret = &g_node[i];
            ++node_num;
            break;
        }
    }
    return ret;
}
// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_add(List *pList, void *pItem)
{
    Node *ret = alloc_node(pItem);
    if (!ret)
        return -1;
    if (!pList->head)
    {
        pList->head = pList->tail = ret;
        ret->next = ret;
        ret->prev = ret;
        return 0;
    }
    if (pList->cur == NULL)
    {
        if (pList->cur_state == LIST_OOB_START)
        {
            ret->next = pList->head;
            ret->prev = pList->head->prev;
            pList->head->prev->next = ret;
            pList->head->prev = ret;
            pList->head = ret;
        }
        else if (pList->cur_state == LIST_OOB_END)
        {
            ret->next = pList->tail->next;
            ret->prev = pList->tail;
            pList->tail->next->prev = ret;
            pList->tail->next = ret;
            pList->tail = ret;
        }
    }
    else
    {
        ret->next = pList->cur->next;
        ret->prev = pList->cur;
        pList->cur->next->prev = ret;
        pList->cur->next = ret;
    }
    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert(List *pList, void *pItem)
{
    Node *ret = alloc_node(pItem);
    if (!ret)
        return -1;
    if (!pList->head)
    {
        pList->head = pList->tail = ret;
        ret->next = ret;
        ret->prev = ret;
        return 0;
    }
    if (pList->cur == NULL)
    {
        if (pList->cur_state == LIST_OOB_START)
        {
            ret->next = pList->head;
            ret->prev = pList->head->prev;
            pList->head->prev->next = ret;
            pList->head->prev = ret;
            pList->head = ret;
        }
        else if (pList->cur_state == LIST_OOB_END)
        {
            ret->next = pList->tail->next;
            ret->prev = pList->tail;
            pList->tail->next->prev = ret;
            pList->tail->next = ret;
            pList->tail = ret;
        }
    }
    else
    {
        ret->next = pList->cur;
        ret->prev = pList->cur->prev;
        pList->cur->prev->next = ret;
        pList->cur->prev = ret;
    }
    return 0;
}

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List *pList, void *pItem)
{
    Node *ret = alloc_node(pItem);
    if (!ret)
        return -1;
    if (!pList->head)
    {
        pList->head = pList->tail = ret;
        ret->next = ret;
        ret->prev = ret;
    }
    else
    {
        ret->next = pList->tail->next;
        ret->prev = pList->tail;
        pList->tail->next->prev = ret;
        pList->tail->next = ret;
        pList->tail = ret;
    }
    pList->cur = ret;
    return 0;
}

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List *pList, void *pItem)
{
    Node *ret = alloc_node(pItem);
    if (!ret)
        return -1;

    if (!pList->head)
    {
        pList->head = pList->tail = ret;
        ret->next = ret;
        ret->prev = ret;
    }
    else
    {
        ret->next = pList->head;
        ret->prev = pList->head->prev;
        pList->head->prev->next = ret;
        pList->head->prev = ret;
        pList->head = ret;
    }
    pList->cur = ret;

    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void *List_remove(List *pList)
{
    void *item_ptr = NULL;
    Node *delete_node = pList->cur;
    if (!pList->cur)
        return NULL;
    item_ptr = pList->cur->item;
   
    
    List_next(pList);
    if(pList->head == pList->tail){
        pList->head = pList->tail = pList->cur = NULL;
        //pList->cur_state = LIST_OOB_START;
    }
    else{
        delete_node->prev->next = delete_node->next;
        delete_node->next->prev = delete_node->prev;
        if(delete_node == pList->head)
        {
            pList->head = delete_node->next;
        }
        if(delete_node == pList->tail)
        {
            pList->tail = delete_node->prev;
        }
        
    }
    delete_node->occupied = false;
    --node_num;
    
    return item_ptr;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List *pList1, List *pList2)
{
    Node *tail = pList1->tail;
    if (!pList2->head)
    {
        pList2->occupied = false;
        return;
    }
    if (!pList1->head)
    {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
    }
    else
    {
        pList1->tail->next = pList2->head;
        pList1->head->prev = pList2->tail;
        pList2->head->prev = pList1->tail;
        pList2->tail->next = pList1->head;
        pList1->tail = pList2->tail;
    
    }

    pList2->occupied = false;
    return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
typedef void (*FREE_FN)(void *pItem);
void List_free(List *pList, FREE_FN pItemFreeFn)
{
    Node *head = pList->head, *end = head;
    if(head != NULL){
        do{
            pItemFreeFn(head->item);
            head->occupied = false;
            --node_num;
            head = head->next;
        }while (head && (head != end));
    }
    //printf("node_num = %d\n", node_num);
    //assert(node_num == 0);
    pList->occupied = false;
    pList->head = pList->cur = pList->tail = NULL;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void *List_trim(List *pList)
{
    Node *ret = NULL;
    if (!pList->head)
        return NULL;
    ret = pList->tail;
    pList->tail->occupied = false;
    if(pList->head == pList->tail){
        pList->head = pList->tail = pList->cur = NULL;
        pList->cur_state = LIST_OOB_START;

    }
    else{
    //pList->tail->prev->next = pList->next;
        pList->tail = pList->tail->prev;
        pList->tail->next = pList->head;
        pList->head->prev = pList->tail;
        
        pList->cur = pList->tail;
    }

    --node_num;
    return ret->item; //caller should free it
}

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
//
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void *pItem, void *pComparisonArg);
void *List_search(List *pList, COMPARATOR_FN pComparator, void *pComparisonArg)
{
    Node *cur = pList->cur;
    if (cur == NULL && pList->cur_state == LIST_OOB_START)
        cur = pList->head;
    while (cur)
    {
        //pList->cur = cur;
        if (pComparator(cur->item, pComparisonArg) == 1)
        {
            pList->cur = cur;
            return cur->item;
        }

        if (cur == pList->tail)
            break;
        cur = cur->next;
    }
    pList->cur = NULL;
    pList->cur_state = LIST_OOB_END;
    
    return NULL;
}