#ifndef LIST_CONFIG_H_INCLUDED
#define LIST_CONFIG_H_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int error_t;

typedef int elem_t;

const elem_t POISON_VALUE = -666;

const int    LIST_INVALID_INDEX = -1;

static const char*  ERROR_FILE_NAME = "error.log";
static const char*  DUMP_FILE_NAME  = "list.dmp";

const size_t LEN_OF_LINE = 128;

enum Error : unsigned int
{
    NO_ERR                  = 0,
    MEM_ALLOC_ERR           = 1 << 0,
    OPEN_FILE_ERR           = 1 << 1,
    DATA_IS_NULL_ERR        = 1 << 2,
    NEXT_IS_NULL_ERR        = 1 << 3,
    PREV_IS_NULL_ERR        = 1 << 4,
    CAPACITY_FEWER_SIZE_ERR = 1 << 5,
    NULL_NAME_ERR           = 1 << 6,
    NULL_FILE_ERR           = 1 << 7,
    MINUS_LINE_ERR          = 1 << 8,
};

#endif // LIST_CONFIG_H_INCLUDED
