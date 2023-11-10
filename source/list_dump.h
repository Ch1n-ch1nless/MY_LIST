#ifndef LIST_DUMP_H_INCLUDED
#define LIST_DUMP_H_INCLUDED

#include "list.h"

#define LIST_TXT_DUMP(list, dump_file) ListTextDump(list, dump_file, __FILE__, __LINE__, __FUNCTION__);

void ListTextDump(List* list, FILE* output_fp, const char* file, const int line, const char* function);

void ListGraphDump(List* list, error_t* error);

void CreatePicture(List* list, error_t* error);

#endif // LIST_DUMP_H_INCLUDED
