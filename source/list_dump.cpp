#include "list_dump.h"

void ListTextDump(List* list, FILE* output_fp, const char* file, const int line, const char* function)
{
    assert((list != nullptr)      && "Error! Pointer to list is NULL!!!\n");
    assert((output_fp != nullptr) && "Error! Pointer to file is NULL!!!\n");

    error_t error = ListVerify(list);
    if (error != NO_ERR)
    {
        PRINT_LIST_ERROR(list, error);
    }

    char border_line[200] = {};
    memset(border_line, '-', (7 * list->capacity - 2));;

    fprintf(output_fp, "List \"%s\": [%p] from %s(%d)\n", list->name, list, list->file, list->line);
    fprintf(output_fp, "called from file: %s(%d) in function: %s\n{\n", file, line, function);
    fprintf(output_fp, "\tSize     = %d\n", list->size);
    fprintf(output_fp, "\tCapacity = %d\n\n", list->capacity);
    fprintf(output_fp, "\tHead     = %d\n", list->head);
    fprintf(output_fp, "\tTail     = %d\n", list->tail);
    fprintf(output_fp, "\tFree     = %d\n\n", list->free);
    fprintf(output_fp, "\tData     = [%p]\n", list->data);
    fprintf(output_fp, "\tNext     = [%p]\n", list->next);
    fprintf(output_fp, "\tPrev     = [%p]\n", list->prev);

    fprintf(output_fp, "\n\tElements of list:\n\n\tIndex:  ");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(output_fp, "|%5d ", i);
    }
    fprintf(output_fp, "\n\t\t%s\n\tData:   ", border_line);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(output_fp, "|%5d ", list->data[i]);
    }
    fprintf(output_fp, "\n\t\t%s\n\tNext:   ", border_line);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(output_fp, "|%5d ", list->next[i]);
    }
    fprintf(output_fp, "\n\t\t%s\n\tPrev:   ", border_line);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(output_fp, "|%5d ", list->prev[i]);
    }
    fprintf(output_fp, "\n\t\t%s\n", border_line);
    fprintf(output_fp, "\t}\n}\n");
}

void CreatePicture(List* list, error_t* error)
{
    assert((list  != nullptr) && "Pointer to list is NULL!!!\n");
    assert((error != nullptr) && "Pointer to list is NULL!!!\n");

    ListGraphDump(list, error);
    if (*error != NO_ERR)
    {
        return;
    }

    char command[2048] = {};

    sprintf(command, "dot %s -T %s -o %s", DOT_FILE_LOCATION, EXTENSION, FILE_LOCATION);

    printf("%s\n", command);

    system(command);
}

void ListGraphDump(List* list, error_t* error)
{
    assert((list  != nullptr) && "Pointer to list is NULL!!!\n");
    assert((error != nullptr) && "Pointer to list is NULL!!!\n");

    FILE* dot_file = nullptr;

    *error |= OpenFile(DOT_FILE_NAME, &dot_file, "w");
    if (*error & OPEN_FILE_ERR)
    {
        return;
    }

    //=================================================================================

    fprintf(dot_file, "digraph G\n");
    fprintf(dot_file, "{\n");
    fprintf(dot_file, "\tgraph [dpi = 200];\n\n");
    fprintf(dot_file, "\tranksep = 1.5;\n\n");
    fprintf(dot_file, "\tsplines = ortho\n\n");
    fprintf(dot_file, "\tedge[minlen = 3, penwidth = 3];\n");
    fprintf(dot_file, "\tnode[shape = rectangle, style = \"rounded, filled\", height = 4, "
                      "width = 6, fixedsize = true, fillcolor = \"#AABDFF\", fontsize = 40, "
                      "penwidth = 3];\n");

	fprintf(dot_file, "\t{rank = min;\n");
	fprintf(dot_file, "\t\thead_node[label = \"Head\", height = 1, fillcolor = yellow];\n");
	fprintf(dot_file, "\t\tfree_node[label = \"Free\", height = 1, fillcolor = green];\n");
	fprintf(dot_file, "\t}\n\n");

    //=================================================================================

    fprintf(dot_file, "\t{rank = same;\n");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(dot_file, "\t\tnode%d[label = \"%d\n value: %d\n next:  %d\nprev:  %d\"];\n",
                i, i, list->data[i], list->next[i], list->prev[i]);
    }

    fprintf(dot_file, "\t}\n\n");

    //=================================================================================

    fprintf(dot_file, "\t{rank = max;\n");
    fprintf(dot_file, "\t\ttail_node[label = \"Tail\", height = 1, fillcolor = yellow];\n");
	fprintf(dot_file, "\t}\n\n");

	//=================================================================================

	fprintf(dot_file, "\thead_node -> node%d [weight = 8; color =\"yellow\"];\n", HEAD);
    fprintf(dot_file, "\ttail_node -> node%d [weight = 8; color =\"yellow\"];\n", TAIL);
    fprintf(dot_file, "\tfree_node -> node%d [weight = 8; color =\"#00FF41  \"];\n", FREE);

    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        fprintf(dot_file, "\t node%d -> node%d [weight = 8; style = invis];\n", i, i + 1);
    }

	//=================================================================================

	size_t cur_index = 0;

	for (size_t i = 0; i < list->size; i++)
	{
        size_t next_index = list->next[cur_index];
        fprintf(dot_file, "\tnode%d -> node%d[constraint = false, weight = 8, color = \"#FF4001\"];\n",
                          cur_index, next_index);
        cur_index = next_index;
	}

	//------------------------------------------------------------------------------

	cur_index = 0;

	for (size_t i = 0; i < list->size; i++)
	{
        size_t prev_index = list->prev[cur_index];
        fprintf(dot_file, "\tnode%d -> node%d[constraint = false, weight = 8, color = \"#0080FF\"];\n",
                          cur_index, prev_index);
        cur_index = prev_index;
	}

	//------------------------------------------------------------------------------

	cur_index = list->free;

    for (size_t i = 0; i < number_of_free_elements - 1; i++)
    {
        size_t next_index = list->next[cur_index];
        fprintf(dot_file, "\tnode%d -> node%d[constraint = false, weight = 8, color = \"#00FF41\"];\n",
                          cur_index, next_index);
        cur_index = next_index;
    }

    //=================================================================================

    fprintf(dot_file, "}\n");

}
