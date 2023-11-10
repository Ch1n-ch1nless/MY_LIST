#include "list_dump.h"

int main()
{
    List list = {};

    FILE* dump_file  = nullptr;

    error_t error = NO_ERR;

    error |= OpenFile(DUMP_FILE_NAME, &dump_file, "w");

    LIST_CTOR(&list, 8)

    LIST_TXT_DUMP(&list, dump_file)

    for (size_t i = 0; i < 4; i++)
    {
        ListInsert(&list, i, 10 * (i + 1), &error);
        LIST_TXT_DUMP(&list, dump_file)
    }

    ListInsert(&list, 2, 25, &error);
    LIST_TXT_DUMP(&list, dump_file)

    ListErase(&list, 1, &error);
    LIST_TXT_DUMP(&list, dump_file)

    CreatePicture(&list, &error);

    return 0;
}
