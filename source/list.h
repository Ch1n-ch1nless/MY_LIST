#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "list_config.h"

#define LIST_CTOR(list, capacity) ListCtor(list, #list, __FILE__, __LINE__, capacity);

#define PRINT_LIST_ERROR(list, error) PrintListError(list, error, __FILE__, __FUNCTION__, __LINE__);

#define HEAD list->next[0]

#define TAIL list->prev[0]

#define FREE list->free

#define number_of_free_elements list->capacity - list->size

struct List
{
    elem_t* data = nullptr;
    int*    next = nullptr;
    int*    prev = nullptr;

    int head = -1;
    int tail = -1;
    int free = -1;

    size_t size     = 0;
    size_t capacity = 0;

    const char* name = nullptr;
    const char* file = nullptr;
    int         line = -1;
};

error_t ListCtor(List* list, const char* name, const char* file, int line, size_t capacity);
error_t ListDtor(List* list);

int     ListInsert(List* list, int index, elem_t element, error_t* error);
int     ListPushFront(List* list, elem_t element, error_t* error);
int     ListPushBack(List* list, elem_t element, error_t* error);

int     ListErase(List* list, int index, error_t* error);
int     ListPopFront(List* list, error_t* error);
int     ListPopBack(List* list, error_t* error);

error_t ListVerify(List* list);

void    PrintListError(List* list, error_t error, const char* file,
                                                  const char* function,
                                                  const int line);

error_t OpenFile(const char* file_name, FILE** file_pointer, const char* mode);

#endif // LIST_H_INCLUDED
