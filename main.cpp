#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "parse_func.h"
//#include "labels.h"

int main(const int argc, const char* argv[])
{
    if (argc < 1)
    {
        printf("\033[31mERROR\033[0m of argc %d\n", argc);

        return 1;
    }

    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    tree -> num_var = NUM_OF_KWD;
    tree -> version = 0;

    FILE* file = fopen(argv[1], "r");
    Read_tree_file(tree, file);
    fclose(file);

    Draw_Graph(tree);

    file = fopen("Text_Tree.txt", "w");
    Print_Pre_Order(tree, tree -> root, file);
    fclose(file);

    file = fopen("Names_Table.txt", "w");
    Print_Name_Table_to_file(tree, file);
    fclose(file);

    system("make draw");

}
