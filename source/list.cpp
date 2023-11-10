#include "list.h"

error_t ListCtor(List* list, const char* name, const char* file, int line, size_t capacity)
{
    assert((list != nullptr) && "Error! Pointer to list is NULL!!!\n");
    assert((name != nullptr) && "Error! Pointer to name is NULL!!!\n");
    assert((file != nullptr) && "Error! Pointer to file is NULL!!!\n");

    error_t error = NO_ERR;

    list->name = name;
    list->file = file;
    list->line = line;

    list->head = 0;
    list->tail = 0;
    list->free = 1;

    list->size     = 1;
    list->capacity = capacity;

    list->data = (elem_t*) calloc(capacity, sizeof(elem_t));
    if (list->data == nullptr)
    {
        return error | MEM_ALLOC_ERR;
    }

    list->next = (int*) calloc(capacity, sizeof(int));
    if (list->next == nullptr)
    {
        return error | MEM_ALLOC_ERR;
    }

    list->prev = (int*) calloc(capacity, sizeof(int));
    if (list->prev == nullptr)
    {
        return error | MEM_ALLOC_ERR;
    }

    for (size_t i = 0; i < capacity; i++)
    {
        list->data[i] = POISON_VALUE;
        list->next[i] = i + 1;
        list->prev[i] = LIST_INVALID_INDEX;
    }

    list->prev[0] = 0;
    list->next[0] = 0;


    return error;
}

error_t ListDtor(List* list)
{
    assert((list != nullptr) && "Error! Pointer to list is NULL!!!\n");

    error_t error = NO_ERR;

    free(list->data);
    list->data = nullptr;

    free(list->next);
    list->next = nullptr;

    free(list->prev);
    list->prev = nullptr;

    list->name = nullptr;
    list->file = nullptr;
    list->line = -1;

    list->head = -1;
    list->tail = -1;
    list->free = -1;

    list->size     = 0;
    list->capacity = 0;

    return error;
}

int ListInsert(List* list, int index, elem_t element, error_t* error)
{
    assert((list != nullptr) && "Error! Pointer to list is NULL!!!\n");
    assert((error != nullptr) && "Error! Pointer to error is NULL!!!\n");

    int ret_index = -1;

    int free_index = list->free;
    list->free     = list->next[free_index];

    list->next[free_index]             = list->next[index];
    list->prev[list->next[free_index]] = free_index;


    list->data[free_index] = element;
    list->next[index]      = free_index;
    list->prev[free_index] = index;

    ret_index = free_index;

    list->size++;

    return ret_index;
}

int ListPushFront(List* list, elem_t element, error_t* error)
{
    return ListInsert(list, list->head, element, error);
}

int ListPushBack(List* list, elem_t element, error_t* error)
{
    return ListInsert(list, list->tail, element, error);
}

int ListErase(List* list, int index, error_t* error)
{
    assert((list != nullptr) && "Error! Pointer to list is NULL!!!\n");
    assert((error != nullptr) && "Error! Pointer to error is NULL!!!\n");

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    list->data[index] = POISON_VALUE;
    list->next[index] = list->free;
    list->prev[index] = LIST_INVALID_INDEX;

    list->free = index;

    list->size--;

    return index;
}

int ListPopFront(List* list, error_t* error)
{
    return ListErase(list, list->head, error);
}

int ListPopBack(List* list, error_t* error)
{
    return ListErase(list, list->tail, error);
}

error_t ListVerify(List* list)
{
    assert((list != nullptr) && "Error! Pointer to list is NULLL!!!\n");

    error_t error = NO_ERR;

    if (list->data == nullptr)
    {
        error |= DATA_IS_NULL_ERR;
    }
    if (list->next == nullptr)
    {
        error |= NEXT_IS_NULL_ERR;
    }
    if (list->prev == nullptr)
    {
        error |= PREV_IS_NULL_ERR;
    }

    if (list->size > list->capacity)
    {
        error |= CAPACITY_FEWER_SIZE_ERR;
    }

    if (list->name == nullptr)
    {
        error |= NULL_NAME_ERR;
    }
    if (list->file == nullptr)
    {
        error |= NULL_FILE_ERR;
    }
    if (list->line <= 0)
    {
        error |= MINUS_LINE_ERR;
    }

    return error;
}

void PrintListError(List* list, error_t error, const char* file, const char* function, const int line)
{
    assert((list != nullptr) && "Error! Pointer to list is NULL!!!\n");

    FILE* ERROR_FILE = nullptr;

    error |= OpenFile(ERROR_FILE_NAME, &ERROR_FILE, "a");
    if (error & OPEN_FILE_ERR)
    {
        ERROR_FILE = stderr;
    }

    fprintf(ERROR_FILE, "********ERROR********\n");
    fprintf(ERROR_FILE, "In %s(%d) called from %s(%d) in function: %s\n",list->file,
                                                                         list->line,
                                                                         file, line,
                                                                         function);

    if (error & MEM_ALLOC_ERR)
    {
        fprintf(ERROR_FILE, "Error! Program can NOT allocate memory!!!\n");
    }

    if (error & OPEN_FILE_ERR)
    {
        fprintf(ERROR_FILE, "Error! Program can NOT open file!!!\n");
    }

    if (error & DATA_IS_NULL_ERR)
    {
        fprintf(ERROR_FILE, "Error! Pointer to data is NULL!!!\n");
    }
    if (error & NEXT_IS_NULL_ERR)
    {
        fprintf(ERROR_FILE, "Error! Pointer to next is NULL!!!\n");
    }
    if (error & PREV_IS_NULL_ERR)
    {
        fprintf(ERROR_FILE, "Error! Pointer to prev is NULL!!!\n");
    }

    if (error & CAPACITY_FEWER_SIZE_ERR)
    {
        fprintf(ERROR_FILE, "Error! Capacity[%d] < size[%d]!!!\n", list->capacity, list->size);
    }

    if (error & NULL_NAME_ERR)
    {
        fprintf(ERROR_FILE, "Error! Pointer to list name is NULL!!!\n");
    }
    if (error & NULL_FILE_ERR)
    {
        fprintf(ERROR_FILE, "Error! Pointer to list file is NULL!!!\n");
    }
    if (error & MINUS_LINE_ERR)
    {
        fprintf(ERROR_FILE, "Error! List's line[%d] < 0!!!\n", list->line);
    }

    fprintf(ERROR_FILE, "*********************\n");

    return;
}

error_t OpenFile(const char* file_name, FILE** file_pointer, const char* mode)
{
    assert((file_name    != nullptr) && "Error pointer to file name is NULL\n!!!");
    assert((file_pointer != nullptr) && "Error pointer to file pointer is NULL\n!!!");
    assert((mode         != nullptr) && "Error pointer to mode is NULL\n!!!");

    *file_pointer = fopen(file_name, mode);
    if (*file_pointer == nullptr)
    {
        return OPEN_FILE_ERR;
    }

    return NO_ERR;
}
