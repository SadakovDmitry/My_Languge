#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "back_end.h"




int main()
{
    struct Tree* tree = (struct Tree*) calloc(1, sizeof(struct Tree));

    Read_Tree(tree);

    Draw_Graph(tree);

    FILE* file = fopen("Asm_file.txt", "w");

    assert(file);

    Print_Asm_Depens_of_Type(tree, tree -> root, file);
    fclose(file);

    Draw_Graph(tree);
}




struct Node* Create_Node_from_file(struct Tree* tree, char* tree_buf, int* pos)
{
    assert(tree_buf);
    assert(pos);

    int node_type    = -1;
    double node_val  = -1;
    char* node_val_str = NULL;

    if(sscanf(tree_buf + *(pos), "%d %lg", &node_type, &node_val) != 2)
    {
        printf(red(Wrong input in back_end file) "\n");
    }

    Skip_Value(tree_buf, pos);
    Skip_Space(tree_buf, pos);

    Skip_Value(tree_buf, pos);
    Skip_Space(tree_buf, pos);

    switch(node_type)
    {
    case KEY_OP:
        return Create_Node(KEY_OP, {.var_id = (int) node_val}, NULL, NULL, tree);
    case BIN_OP:
        return Create_Node(BIN_OP, {.var_id = (int) node_val}, NULL, NULL, tree);
    case UN_OP:
        return Create_Node(UN_OP , {.var_id = (int) node_val}, NULL, NULL, tree);
    case PUNCT:
        return Create_Node(PUNCT , {.var_id = (int) node_val}, NULL, NULL, tree);
    case VAR:
        return Create_Node(VAR   , {.var_id = (int) node_val}, NULL, NULL, tree);
    case NUM:
        return Create_Node(NUM   , {.num    =       node_val}, NULL, NULL, tree);
    case FUNC:
        return Create_Node(FUNC  , {.var_id = (int) node_val}, NULL, NULL, tree);
    case STR:
        return Create_Node(STR   , {.var_id = (int) node_val}, NULL, NULL, tree);
    default:
        printf(red(ERROR) "wrong operation on \"%d\"in " green(Create_Node_from_file) "\n", *pos);
        exit(1);
    }

    return NULL;
}


struct Node* Read_Node(Tree* tree, char* tree_buf, int* pos)
{

    Skip_Space(tree_buf, pos);

    if(tree_buf[*pos] == '(')
    {
        *pos = *pos + 1;
        Skip_Space(tree_buf, pos);
    }
    if(tree_buf[*pos] == '_')
    {
        *pos = *pos + 1;
        Skip_Space(tree_buf, pos);

        return NULL;
    }

    struct Node* new_node = Create_Node_from_file(tree, tree_buf, pos);

    new_node -> left  = Read_Node(tree, tree_buf, pos);
    new_node -> right = Read_Node(tree, tree_buf, pos);

    Skip_Space(tree_buf, pos);

    if(tree_buf[*pos] == ')')
    {
        *pos = *pos + 1;
        Skip_Space(tree_buf, pos);

        return new_node;
    }
    else
    {
        printf(red(ERROR) "No close bracket in tree_buf!!!\n");
        exit(1);
    }
}

void Read_Tree(struct Tree* tree)
{
    tree -> var_buf = Names_Table_Ctor();
    //tree -> var_buf = Add_Variable(tree -> var_buf);
    FILE* file = fopen("Names_Table.txt", "r");
    tree -> var_buf = Create_Name_Table_from_file(tree, file);
    fclose(file);

    file = fopen("Text_Tree.txt", "r");

    int size_of_file = 0;
    int pos = 0;

    char* tree_buf = Read_file(tree, file, &size_of_file);

    tree -> root = Read_Node(tree, tree_buf, &pos);

    fclose(file);
}

struct Labels* Add_Variable(struct Labels* names_table)
{
    for (int i = NUM_OF_KWD; i < NUM_OF_KWD + NUM_REG; i++)
    {
        names_table[i].name = new_var[i - NUM_OF_KWD];
        names_table[i].type = VAR;
        names_table[i].val  = 0;
    }

    return names_table;
}





int Find_Num_of_Spaces( int file_size)
{
    FILE* file = fopen("Names_Table.txt", "r");

    assert ( file != nullptr );

    char input = 0;
    int num_sp = 0;

    while ((input = fgetc(file)) != EOF)
    {
        if (input == ' ')
        {
            num_sp++;
        }
    }

    printf(green(num_sp = ) " %d\n", num_sp);
    fclose(file);
    return num_sp;
}

struct Labels* Create_Name_Table_from_file(struct Tree* tree, FILE* file)
{
    int size_of_file = 0;

    char* name_table_buf = Read_file(tree, file, &size_of_file);

    int num_sp = Find_Num_of_Spaces(size_of_file);

    struct Labels* names_table = Read_Names_Table(name_table_buf, num_sp);

    for (int i = 0; i < NUM_OF_KWD; i++)
    {
        names_table[i].type = KWD;
        names_table[i].val  = 0;
    }

    for (int i = NUM_OF_KWD; i < num_sp; i++)
    {
        names_table[i].type = VAR;
        names_table[i].val  = 0;
    }

    return names_table;
}

struct Labels* Read_Names_Table(char* buf, int num_sp)
{
    int pos_buf = 0;

    struct Labels* names_table = (struct Labels*) calloc(NUM_OF_KWD + 100, sizeof(struct Labels));

    for(int i = 0; i < num_sp; i++)
    {
        char* str = &buf[pos_buf];

        while(buf[pos_buf] != ' ')
        {
            pos_buf++;
        }

        char* new_str = (char*) calloc((&buf[pos_buf] - str + 1), sizeof(char));
        memcpy(new_str, str, (&buf[pos_buf] - str));

        Skip_Space(buf, &pos_buf);

        names_table[i].name = new_str;
        //printf("pos = %d, new_str = %s\n", i, new_str);
    }

    return names_table;
}



void Read_Tree_for_Asm(struct Tree* tree, struct Node* node, FILE* file)
{
    if(node == NULL) return;

    Read_Tree_for_Asm(tree, node -> left, file);

    Read_Tree_for_Asm(tree, node -> right, file);

    Print_Asm_Depens_of_Type(tree, node, file);

    return;
}

void Print_Asm_Depens_of_Type(struct Tree* tree, struct Node* node, FILE* file)
{
    if(node == NULL) return;

    int now_while = NUM_WHILE;
    int now_if    = NUM_IF;

    switch(node -> type)
    {
    case BIN_OP:
        switch((node -> val).bin_op)
        {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            Print_Asm_Depens_of_Type(tree, node -> left, file);
            Print_Asm_Depens_of_Type(tree, node -> right, file);
            fprintf(file, "\t%s\n", assembler_cmd[(node -> val).var_id]);
            break;
        case MORE:
        case LESS:
        case MORE_E:
        case LESS_E:
        case N_EQAL:
        case EQAL:
            #warning Depends...
            Print_Asm_Depens_of_Type(tree, node -> left, file);
            Print_Asm_Depens_of_Type(tree, node -> right, file);
            fprintf(file, "\t%s ", assembler_cmd[(node -> val).var_id]);
            //fprintf(file, "\tsub\n");
            //fprintf(file, "\tpush 0\n");
            break;
        case AND:
        case OR:
            break;
        case ASSIGN:
            fprintf(file, "\n");
            Print_Asm_Depens_of_Type(tree, node -> left, file);
            fprintf(file, "\tpop r%sx\n", (tree -> var_buf)[((node -> right) -> val).var_id].name);
            fprintf(file, "\n");
        }
        break;
    case UN_OP:
        switch((node -> val).un_op)
        {
        case RET:
        case OUT:
        case OUT_C:
        case SQRT:
        case SIN:
        case COS:
            Print_Asm_Depens_of_Type(tree, node -> right, file);
            fprintf(file, "\t%s\n", assembler_cmd[(node -> val).var_id]);
            break;
        case CALL:
            Print_Asm_Depens_of_Type(tree, (node -> right) -> right, file);
            fprintf(file, "\t%s ", assembler_cmd[(node -> val).var_id]);
            Print_Asm_Depens_of_Type(tree, (node -> right) -> left, file);
            break;
        case IN:
            fprintf(file, "\t%s\n", assembler_cmd[(node -> val).var_id]);
            fprintf(file, "\tpop r%sx\n", (tree -> var_buf)[((node -> right) -> val).var_id].name);
            break;
        default:
            fprintf(file, "\t%s\n", assembler_cmd[(node -> val).var_id]);
            break;
        }
        break;
    case KEY_OP:
        switch((node -> val).key_op)
        {
        case WHILE:
            //now_while = NUM_WHILE;
            fprintf(file, "while_%d:\n\n", NUM_WHILE);
            Print_Asm_Depens_of_Type(tree, node -> left, file);
            fprintf(file, /*jmp*/"end_while_%d:\n\n", NUM_WHILE);
            NUM_WHILE++;
            Print_Asm_Depens_of_Type(tree, node -> right, file);
            fprintf(file, "\tjmp while_%d:\n\n", now_while);
            fprintf(file, "end_while_%d:\n\n", now_while);
            break;
        case IF:
            //now_if = NUM_IF;
            Print_Asm_Depens_of_Type(tree, node -> left, file);
            fprintf(file, /*jmp*/"else_%d:\n\n", NUM_IF);
            NUM_IF++;
            Print_Asm_Depens_of_Type(tree, (node -> right) -> left, file);
            fprintf(file, "\tjmp end_if_%d:\n\n", now_if);
            fprintf(file, "else_%d:\n\n", now_if);
            Print_Asm_Depens_of_Type(tree, (node -> right) -> right, file);
            fprintf(file, "end_if_%d:\n\n", now_if);
            break;
        default:
            printf(red(Wrong KEY_OP));
            break;
        }
        break;
    case PUNCT:
        Print_Asm_Depens_of_Type(tree, node -> left, file);
        Print_Asm_Depens_of_Type(tree, node -> right, file);
        break;
    case VAR:
        fprintf(file, "\tpush r%sx\n", (tree -> var_buf)[(node -> val).var_id].name);
        break;
    case NUM:
        fprintf(file, "\tpush %d\n", (int) (node -> val).num);
        #warning my do normal with accuracy to SPU
        break;
    case FUNC:
        fprintf(file, "%s:\n", (tree -> var_buf)[(node -> val).func_id].name);
        Print_Asm_Depens_of_Type(tree, node -> left , file);

        if(strcmp("main", (tree -> var_buf)[(node -> val).func_id].name) == 0)
        {
            fprintf(file, "\thlt\n\n\n");
        }
        else //if(node -> right == NULL)
        {
            //fprintf(file, "\tret\n\n\n");
        }

        Print_Asm_Depens_of_Type(tree, node -> right, file);

        break;
    case STR:
        printf(red(No str something wrong));
        break;
    default:
        printf(red(ERROR) "wrong type in \"" green(Print_Assembler_Depens_of_Type) "\"");
        exit(1);
    }

    return;
}
