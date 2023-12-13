#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "parse_func.h"


int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));
    struct Labels* var_buf = (struct Labels*) calloc(NUM_VAR, sizeof(struct Labels));
    struct Remove* rems = (struct Remove*) calloc(MAX_NUM_REPLASES, sizeof(struct Remove));
    var_buf[0].var = (char*) calloc(5, sizeof(char));
    //strcpy(var_buf[0].var, "e");
    Fill_Labels(var_buf);

    rems -> num_rems = 0;
    tree -> var_buf = var_buf;
    tree -> num_var = 3;
    tree -> version = 0;

    //printf("var_buf[0] = %s = %lg\n", var_buf[0].var, var_buf[0].val);
    Read_tree_file(tree);

    Draw_Graph(tree);

    system("make draw");

}
