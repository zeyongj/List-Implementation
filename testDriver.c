/*
 * Assignment 1 test driver for that weird python file
 * Andy Sun
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)

// For searching
static bool itemEquals(void* pItem, void* pArg) {
    return (pItem == pArg);
}

// For checking the "free" function called
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    CHECK(*(int*)pItem == 0);
    (*(int*)pItem)++;
}

static void simpleFreeFn(void* pItem) {
    return;
}

static int freeCounter = 0;
static int nullCounter = 0;
static void modFreeFn(void* pItem) {
    List* l = NULL;
    if (pItem != NULL) {
        freeCounter++;
        l = (List*) pItem;
        List_free(l, simpleFreeFn);
    } else {
        nullCounter++;
    }
}

typedef void (*TestFn)();

void TestCreate() {
    List* lists[LIST_MAX_NUM_HEADS] = { 0 }; // 0 initialize all pointers

    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        lists[i] = List_create();
        CHECK(lists[i] != 0);
        CHECK(List_count(lists[i]) == 0);
        CHECK(List_curr(lists[i]) == NULL);
    }

    int duplicate = 0;
    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        for (int j = 0; j < LIST_MAX_NUM_HEADS; ++j) {
            if (lists[i] == lists[j] && i != j) {
                duplicate = 1;
            }
        }
    }

    CHECK(duplicate == 0);
}

void TestAdd() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    // Add item with nothing (before start)
    // 1
    CHECK(List_add(l, &one) == 0);
    CHECK(List_count(l) == 1);
    CHECK(List_curr(l) == &one);

    // Move to end of list null
    CHECK(List_next(l) == NULL);

    // Add item (beyond end)
    // 1 4
    CHECK(List_add(l, &four) == 0);
    CHECK(List_count(l) == 2);
    CHECK(List_curr(l) == &four);

    // Rewind to start
    CHECK(List_prev(l) == &one);
    CHECK(List_prev(l) == NULL);

    // Add item (before start)
    // 9 1 4
    CHECK(List_add(l, &nine) == 0);
    CHECK(List_count(l) == 3);
    CHECK(List_curr(l) == &nine);
    CHECK(List_next(l) == &one);
    CHECK(List_next(l) == &four);

    // Add item in middle
    // 9 1 6 4
    CHECK(List_prev(l) == &one);
    CHECK(List_add(l, &six) == 0);
    CHECK(List_count(l) == 4);
    CHECK(List_prev(l) == &one);

    // Add null in middle
    // 9 1 NULL 6 4
    CHECK(List_add(l, NULL) == 0);
    CHECK(List_count(l) == 5);
    CHECK(List_curr(l) == NULL);
    CHECK(List_next(l) == &six);
    CHECK(List_next(l) == &four);
    CHECK(List_next(l) == NULL);
}

void TestInsert() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    // Add item with nothing (before start)
    // 1
    CHECK(List_insert(l, &one) == 0);
    CHECK(List_count(l) == 1);
    CHECK(List_curr(l) == &one);

    // Move to end of list null
    CHECK(List_next(l) == NULL);

    // Add item (beyond end)
    // 1 4
    CHECK(List_insert(l, &four) == 0);
    CHECK(List_count(l) == 2);
    CHECK(List_curr(l) == &four);

    // Rewind to start
    CHECK(List_prev(l) == &one);
    CHECK(List_prev(l) == NULL);

    // Add item (before start)
    // 9 1 4
    CHECK(List_insert(l, &nine) == 0);
    CHECK(List_count(l) == 3);
    CHECK(List_curr(l) == &nine);
    CHECK(List_next(l) == &one);
    CHECK(List_next(l) == &four);

    // Add item in middle
    // 9 6 1 4
    CHECK(List_prev(l) == &one);
    CHECK(List_insert(l, &six) == 0);
    CHECK(List_count(l) == 4);
    CHECK(List_next(l) == &one);

    // Add null in middle
    // 9 6 NULL 1 4
    CHECK(List_insert(l, NULL) == 0);
    CHECK(List_count(l) == 5);
    CHECK(List_curr(l) == NULL);
    CHECK(List_next(l) == &one);
    CHECK(List_next(l) == &four);
    CHECK(List_next(l) == NULL);
}

void TestAppend() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    // Append item with nothing (before start)
    // 1
    CHECK(List_append(l, &one) == 0);
    CHECK(List_count(l) == 1);
    CHECK(List_curr(l) == &one);

    // Append item with something in list
    // 1 4
    CHECK(List_append(l, &four) == 0);
    CHECK(List_count(l) == 2);
    CHECK(List_curr(l) == &four);

    // Append item with something in list
    // 1 4 9
    CHECK(List_append(l, &nine) == 0);
    CHECK(List_count(l) == 3);
    CHECK(List_curr(l) == &nine);
    CHECK(List_prev(l) == &four);
    CHECK(List_prev(l) == &one);
    CHECK(List_prev(l) == NULL);

    // Append null with something in list
    // 1 4 9 NULL
    CHECK(List_append(l, NULL) == 0);
    CHECK(List_count(l) == 4);
    CHECK(List_curr(l) == NULL);

    // Append item after null
    // 1 4 9 NULL 6
    CHECK(List_append(l, &six) == 0);
    CHECK(List_count(l) == 5);
    CHECK(List_curr(l) == &six);
    CHECK(List_prev(l) == NULL);
    CHECK(List_prev(l) == &nine);
    CHECK(List_prev(l) == &four);
    CHECK(List_prev(l) == &one);
    CHECK(List_prev(l) == NULL);
}

void TestPrepend() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    // Prepend item with nothing (before start)
    // 1
    CHECK(List_prepend(l, &one) == 0);
    CHECK(List_count(l) == 1);
    CHECK(List_curr(l) == &one);

    // Prepend item with something in list
    // 4 1
    CHECK(List_prepend(l, &four) == 0);
    CHECK(List_count(l) == 2);
    CHECK(List_curr(l) == &four);

    // Prepend item with something in list
    // 9 4 1
    CHECK(List_prepend(l, &nine) == 0);
    CHECK(List_count(l) == 3);
    CHECK(List_curr(l) == &nine);
    CHECK(List_next(l) == &four);
    CHECK(List_next(l) == &one);

    // Prepend null with something in list
    // NULL 9 4 1
    CHECK(List_prepend(l, NULL) == 0);
    CHECK(List_count(l) == 4);
    CHECK(List_curr(l) == NULL);

    // Prepend item after null
    // 6 NULL 9 4 1
    CHECK(List_prepend(l, &six) == 0);
    CHECK(List_count(l) == 5);
    CHECK(List_curr(l) == &six);
    CHECK(List_next(l) == NULL);
    CHECK(List_next(l) == &nine);
    CHECK(List_next(l) == &four);
    CHECK(List_next(l) == &one);
    CHECK(List_next(l) == NULL);
}

void TestRemove() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    CHECK(List_remove(l) == NULL);

    // 1 4 6 9
    CHECK(List_append(l, &one) == 0);
    CHECK(List_append(l, &four) == 0);
    CHECK(List_append(l, &six) == 0);
    CHECK(List_append(l, &nine) == 0);
    CHECK(List_count(l) == 4);

    CHECK(List_next(l) == NULL);
    CHECK(List_remove(l) == NULL);
    CHECK(List_count(l) == 4);

    // 1 6 9
    CHECK(List_first(l) == &one);
    CHECK(List_next(l) == &four);
    CHECK(List_remove(l) == &four);
    CHECK(List_count(l) == 3);
    CHECK(List_curr(l) == &six);

    // 1 6
    CHECK(List_last(l) == &nine);
    CHECK(List_remove(l) == &nine);
    CHECK(List_count(l) == 2);
    CHECK(List_curr(l) == NULL);

    // 6
    CHECK(List_first(l) == &one);
    CHECK(List_remove(l) == &one);
    CHECK(List_count(l) == 1);
    CHECK(List_curr(l) == &six);

    CHECK(List_remove(l) == &six);
    CHECK(List_count(l) == 0);
    CHECK(List_curr(l) == NULL);
}

void TestTrim() {
    List* l = List_create();
    int one = 1;
    int four = 4;

    CHECK(List_trim(l) == NULL);

    // 1 4
    CHECK(List_append(l, &one) == 0);
    CHECK(List_append(l, &four) == 0);
    CHECK(List_next(l) == NULL);
    CHECK(List_count(l) == 2);

    // 1
    CHECK(List_trim(l) == &four);
    CHECK(List_count(l) == 1);

    CHECK(List_trim(l) == &one);
    CHECK(List_count(l) == 0);
    CHECK(List_trim(l) == NULL);
}

void TestConcat() {
    List* lists[LIST_MAX_NUM_HEADS];
    List* recycled = NULL;
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        lists[i] = List_create();
        CHECK(lists[i] != NULL);
    }

    // Check empty + empty
    List_concat(lists[0], lists[1]);
    CHECK(List_count(lists[0]) == 0);
    CHECK(List_curr(lists[0]) == NULL);

    // Check recycling
    recycled = List_create();
    CHECK(recycled != NULL);

    // Check 1 4 + 6 9
    CHECK(List_append(lists[0], &one) == 0);
    CHECK(List_append(lists[0], &four) == 0);
    CHECK(List_append(lists[1], &six) == 0);
    CHECK(List_append(lists[1], &nine) == 0);
    List_concat(lists[0], lists[1]);
    CHECK(List_count(lists[0]) == 4);
    CHECK(List_curr(lists[0]) == &four);
    CHECK(List_first(lists[0]) == &one);
    CHECK(List_last(lists[0]) == &nine);

    // Check recycling
    recycled = List_create();
    CHECK(recycled != NULL);
}

void TestFree() {
    List* l = List_create();
    int arr[] = {0, 0, 0, 0};

    CHECK(List_append(l, &(arr[0])) == 0);
    CHECK(List_append(l, &(arr[1])) == 0);
    CHECK(List_append(l, &(arr[2])) == 0);
    CHECK(List_append(l, &(arr[3])) == 0);
    List_free(l, complexTestFreeFn);

    CHECK(arr[0] == 1);
    CHECK(arr[1] == 1);
    CHECK(arr[2] == 1);
    CHECK(arr[3] == 1);
}

void TestFreeNull() {
    List* l = List_create();
    int one = 1;

    CHECK(List_append(l, &one) == 0);
    List_free(l, NULL);
}

void TestSearch() {
    List* l = List_create();
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    CHECK(List_append(l, &one) == 0);
    CHECK(List_append(l, &four) == 0);
    CHECK(List_append(l, &six) == 0);

    CHECK(List_first(l) == &one);
    CHECK(List_prev(l) == NULL);
    CHECK(List_search(l, itemEquals, &one) == &one);
    CHECK(List_search(l, itemEquals, &one) == &one);
    CHECK(List_search(l, itemEquals, &nine) == NULL);
    CHECK(List_curr(l) == NULL);
    CHECK(List_prev(l) == &six);
}

void TestSearchNull() {
    List* l = List_create();
    int one = 1;

    CHECK(List_append(l, &one) == 0);
    CHECK(List_search(l, NULL, NULL) == NULL);
}

void TestNodeRecycling() {
    List* lists[LIST_MAX_NUM_HEADS];
    int one = 1;
    int four = 4;
    int six = 6;

    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        lists[i] = List_create();
        CHECK(lists[i] != NULL);
    }

    CHECK(List_append(lists[0], &one) == 0);
    for (int i = 0; i < LIST_MAX_NUM_NODES - 1; ++i) {
        CHECK(List_append(lists[1], &four) == 0);
    }

    CHECK(List_count(lists[0]) == 1);
    CHECK(List_count(lists[1]) == 99);

    CHECK(List_add(lists[1], &six) == -1);
    CHECK(List_insert(lists[1], &six) == -1);
    CHECK(List_append(lists[1], &six) == -1);
    CHECK(List_prepend(lists[1], &six) == -1);

    List_free(lists[0], simpleFreeFn);
    CHECK(List_append(lists[1], &six) == 0);
    CHECK(List_append(lists[1], &one) == -1);
    CHECK(List_count(lists[1]) == 100);

    List_free(lists[1], simpleFreeFn);
    for (int i = 0; i < LIST_MAX_NUM_NODES; ++i) {
        CHECK(List_append(lists[2], &one) == 0);
    }
    CHECK(List_count(lists[2]) == 100);
}

void TestHeadRecycling() {
    List* lists[LIST_MAX_NUM_HEADS];
    List* recycled = NULL;
    int one = 1;
    int four = 4;
    int six = 6;
    int nine = 9;

    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        lists[i] = List_create();
    }

    // Check empty + empty
    List_concat(lists[0], lists[1]);

    // Check recycling
    recycled = List_create();
    CHECK(recycled != NULL);
    CHECK(recycled == lists[1]);
    CHECK(List_count(recycled) == 0);
    CHECK(List_curr(recycled) == NULL);

    // Check 1 4 + 6 9
    CHECK(List_append(lists[0], &one) == 0);
    CHECK(List_append(lists[0], &four) == 0);
    CHECK(List_append(lists[1], &six) == 0);
    CHECK(List_append(lists[1], &nine) == 0);
    List_concat(lists[0], lists[1]);
    CHECK(List_first(lists[0]) == &one);
    CHECK(List_last(lists[0]) == &nine);

    // Check recycling
    recycled = List_create();
    CHECK(recycled != NULL);
    CHECK(recycled == lists[1]);
    CHECK(List_count(recycled) == 0);
    CHECK(List_curr(recycled) == NULL);
}

void TestInFreeListChange() {
    List* lists[LIST_MAX_NUM_HEADS];

    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        lists[i] = List_create();
    }
    for (int i = 0; i < LIST_MAX_NUM_HEADS; ++i) {
        CHECK(List_append(lists[0], &(lists[i])) == 0);
    }
    List_free(lists[0], modFreeFn);

    CHECK(freeCounter >= LIST_MAX_NUM_HEADS);
}

static TestFn dispatchTable[] = {
    // Basic functions
    TestCreate, TestAdd, TestInsert, TestAppend, TestPrepend, TestRemove, TestTrim,
    // Advanced functions
    TestConcat, TestFree, TestFreeNull, TestSearch, TestSearchNull,
    // List/node recycling
    TestNodeRecycling, TestHeadRecycling, TestInFreeListChange,
};

int main(int argCount, char *args[]) 
{
    int TestID = -1;
    int TotalTests = sizeof(dispatchTable)/sizeof(dispatchTable[0]);

    if (argCount != 2) {
        printf("Missing test arg\n");
        return 1;
    }

    if (strcmp(args[1], "get_num_tests") == 0) {
        printf("%d\n", TotalTests);
        return 0;
    }

    TestID = atoi(args[1]);
    if (TestID < 0 || TestID >= TotalTests) {
        printf("Invalid test ID\n");
        return 1;
    }
    dispatchTable[TestID]();
    
    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}
