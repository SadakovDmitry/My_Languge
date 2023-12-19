#include "dif_func.h"
#include "lib.h"
#include "work_with_file.h"
#include "parse_func.h"
#include "labels.h"


void Beautiful_Dump()
{
    printf( "\n\n\033[36m          /＞   フ\n"
            "         |  _  _| \n"
            "        /`ミ _x 彡 \n"
            "       /        | \n"
            "      /  ヽ     ﾉ \n"
            "  ／￣|    | | | \n"
            "  | (￣ヽ__ヽ_)_) \n"
            "  ＼二つ    \033[31mDUMP\033[0m\n\n");
}


//                                                 Print to dot file
//------------------------------------------------------------------------------------------------------------------------------
void Print_Node_depends_of_type(struct Tree* tree, struct Node* node, FILE* file_dot)
{
    if(node -> type == BIN_OP || node -> type == UN_OP || node -> type == KEY_OP || node -> type == PUNCT)
    {
        fprintf(file_dot, "%lld [color = \"red\", shape = record, style = \"rounded\", label = \"{ ", (long long int) node);
        Print_Node(tree, node, file_dot);
        fprintf(file_dot, " pri: %d | size: %d|{ <left> left | <right> right }}\"];\n\t", node -> priority, node -> size);
    }
    else if(node -> type == NUM)
    {
        fprintf(file_dot, "%lld [color = \"green\", shape = record, style = \"rounded\", label = \"{ %lf | NUM | pri: %d |{ <left> left | <right> right }}\"];\n\t", (long long int) node, (node -> val).num, node -> priority);
    }
    else if(node -> type == VAR)
    {
        fprintf(file_dot, "%lld [color = \"grey\", shape = record, style = \"rounded\", label = \"{ %s | VAR | pri: %d |{ <left> left | <right> right }}\"];\n\t", (long long int) node, (tree -> var_buf)[(node -> val).var_id].name, node -> priority);
    }
}

void Print_Node_to_file(struct Tree* tree, struct Node* node, FILE* file_dot)
{
    if(node == NULL) return;

    Print_Node_to_file(tree, node -> left, file_dot);

    Print_Node_depends_of_type(tree, node, file_dot);

    Print_Node_to_file(tree, node -> right, file_dot);
}

void Arrows_in_Graph(struct Node* node, FILE* file_dot)
{
    if(node == NULL) return;

    if (node -> left != NULL)
    {
        fprintf(file_dot, "%lld:<left> -> %lld[color = \"green\"]\n\t", (long long int) node, (long long int) node -> left);
    }

    Arrows_in_Graph(node -> left, file_dot);

    if (node -> right != NULL)
    {
        fprintf(file_dot, "%lld:<right> -> %lld[color = \"red\"]\n\t", (long long int) node, (long long int) node -> right);
    }

    Arrows_in_Graph(node -> right, file_dot);

}

void Draw_Graph(struct Tree* tree)
{
    FILE* file_dot = fopen("Tree_Graph.dot", "w");

    assert(file_dot);

    fprintf(file_dot, "digraph G\n{\n\trankdir = \"TB\";\n\n\t"
                                    "node[color = \"red\", style=\"filled, rounded\", fontsize = 14];\n\t"
                                    "edge[color = \"black\", fontcolor = \"blue\", fontsize = 12, weight = 0];\n\n\t");

    fprintf(file_dot, "Tree [shape = record, style = \"rounded\", label = \"root: %p | size: %d\"];\n\t", tree -> root, tree -> size);

    Print_Node_to_file(tree, tree -> root, file_dot);

    Arrows_in_Graph(tree -> root, file_dot);

    fprintf(file_dot, "\n}\n");

    fclose(file_dot);
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                                 Print to consol
//------------------------------------------------------------------------------------------------------------------------------
void Print_Operation(Tree_t val, FILE* file)
{
/*
    switch(op)
    {
    //---------------------------------------------
    #define OP(name, str_op, num, calculate, str_to_dot ...)\
    case name:                                              \
        fprintf(file, str_to_dot" ");                       \
        break;
    //---------------------------------------------

    #include "operators.h"

    #undef OP
*/
        if (strcmp(key_words[val.var_id], "<")  ||
            strcmp(key_words[val.var_id], ">")  ||
            strcmp(key_words[val.var_id], "<=") ||
            strcmp(key_words[val.var_id], ">="))
        {
            fprintf(file, "\\%s ", key_words[val.var_id]);
        }
        else
        {
            fprintf(file, "%s ", key_words[val.var_id]);
        }
}

void Print_Node(struct Tree* tree, struct Node* node, FILE* file)
{
    switch (node -> type)
    {
    case BIN_OP:
        //Print_Operation((node -> val).bin_op, file);
        //break;
        Print_Operation(node -> val, file);
        fprintf(file, " | BIN_OP |");
        break;
    case UN_OP:
        //Print_Operation((node -> val).un_op, file);
        //break;
        Print_Operation(node -> val, file);
        fprintf(file, " | UN_OP |");
        break;
    case KEY_OP:
        Print_Operation(node -> val, file);
        fprintf(file, " | KEY_OP |");
        break;
    case PUNCT:
        Print_Operation(node -> val, file);
        fprintf(file, " | PUNCT |");
        break;
    case NUM:
        fprintf(file, "%lg ", (node -> val).num);
        break;
    case VAR:
        fprintf(file, "%s ", (tree -> var_buf)[(node -> val).var_id].name);              //var
        break;
    default:
        printf(red(ERROR)" uncorrect type in \"" green(Print_Node)"\"!!!");
        exit(1);
    }
}

void Print_In_Order(struct Tree* tree, struct Node* node, FILE* file)
{
    if(node == NULL){
        return;
    }

    if(node -> left != NULL)
    {
        if((node -> left) -> priority > node -> priority)
        {
            fprintf(file, "( ");
        }
    }

    Print_In_Order(tree, node -> left, file);
    Print_Node(tree, node, file);
    Print_In_Order(tree, node -> right, file);

    if(node -> right != NULL)
    {
        if((node -> right) -> priority > node -> priority)
        {
            fprintf(file, ") ");
        }
    }

}
//------------------------------------------------------------------------------------------------------------------------------
//                                                       END




//                                         Read file and create tree (OLD VERSION)
//------------------------------------------------------------------------------------------------------------------------------
void Tree_Dump(struct Tree* tree)
{
    assert(tree);

    Beautiful_Dump();
    printf("root = %p\nsize = %d\n", tree -> root, tree -> size);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    Print_In_Order(tree, tree -> root, stdout);
}

char* Read_file(struct Tree* tree, FILE* file, int* file_size)
{
    struct stat st = {};

    fstat(file->_file, &st);
    tree -> len_tree_buf = st.st_size - 1;
    //printf("\nL%lld\n", st.st_size);
    *(file_size) = st.st_size;

    char* buf = (char*) calloc(st.st_size + 1, sizeof(char));
    fread(buf, sizeof (char), st.st_size, file);

    printf("\nBUF = %s\n", buf);


    return buf;
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                               Work with variable
//------------------------------------------------------------------------------------------------------------------------------
enum TYPE Check_Type(char* str_val)
{
    /*
    if(strstr(operations, str_val) != NULL)
    {
        return OP;
    }
    else
    {
        return VAR;
    }
    */
    if(strstr(un_operations, str_val) != NULL)
    {
        return UN_OP;
    }
    if(strstr(bin_operations, str_val) != NULL)
    {
        return BIN_OP;
    }
    if(strstr(key_operations, str_val) != NULL)
    {
        return KEY_OP;
    }
    if(strstr(punctuation, str_val) != NULL)
    {
        return PUNCT;
    }
    else
    {
        return VAR;
    }
    /*
    if(id > PUNCT)
    {
        return PUNCT;
    }
    if(id > KEY_OP)
    {
        return KEY_OP;
    }
    if(id > BIN_OP)
    {
        return BIN_OP;
    }
    if(id > UN_OP)
    {
        return UN_OP;
    }
    */
    //return -1;

}

int Check_var_in_var_buf(struct Tree* tree, char* name)
{
    assert(tree);

    //fprintf(stderr,"num_var = %d\n", tree -> num_var);
    for (int i = 0; i < tree -> num_var; i++)
    {
        //printf("name: %s vs now_name: %s\n", (tree -> var_buf)[i].name, name);
        if(strcmp((tree -> var_buf)[i].name, name) == 0)
        {
            return i;   //1
        }
    }
    return -1;
}

int Add_Variable(struct Tree* tree, struct Labels* var_buf, char* name)
{
    int var_id = Check_var_in_var_buf(tree, name);
    //printf("id: %d\n", var_id);

    if(var_id == -1)
    {
        int len = strlen(name);
        //printf("len = %d\n", len);
        char* spot_for_cpy = (char*) calloc(len + 1, sizeof(char));
        strcpy(spot_for_cpy, name);

        var_buf[tree -> num_var].type = VAR;
        var_buf[tree -> num_var].name = spot_for_cpy;
        var_buf[tree -> num_var].val  = 0;
        (tree -> num_var)++;

        return (tree -> num_var) - 1;

    }

    return var_id;
}
/*
int Set_Node_Value(struct Tree* tree, struct Node* node, char* buf, int i)
{
    char str[SIZE_STR] = "";
    double new_val = 0;

    if(sscanf(&buf[i], "%lf", &new_val) == 1)
    {
        node -> type = NUM;
        node -> priority = NUM_PRIORITY;
        (node -> val).num = new_val;

        return i;
    }
    else
    {
        sscanf(&buf[i], "%s", str);

        if(Check_Type(str) == OP)
        {
            enum OPERATION op = Convert_op_to_enum(node, str);
            node -> type = OP;
            (node -> val).op = op;
        }
        else
        {
            node -> type = VAR;
            node -> priority = NUM_PRIORITY;
            (node -> val).var_id = Add_Variable(tree, tree -> var_buf, str);
        }

        return i + strlen(str);
    }
}
*/
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                                 Read tree file
//------------------------------------------------------------------------------------------------------------------------------
int Convert_op_to_enum(struct Node* node, char* str_op)
{
    /*
    //--------------------------------------------------------
    #define OP(name, str, ...)                               \
        if(strcasecmp(str, str_op) == 0)                     \
        {                                                    \
            return (int) name;                               \
        }
    //--------------------------------------------------------

    #include "operators.h"

    else
    {
        printf("\n" red(ERROR)" uncorrect operation (%s) in Convert_op_to_enum!!!\n", str_op);
        exit(1);
    }
    */
    for (int i = 0; i < NUM_OF_KWD; i++)
    {
        if(strcasecmp(str_op, key_words[i]) == 0)
        {
            return i;
        }
    }

    printf("\n" red(ERROR)" uncorrect operation (%s) in Convert_op_to_enum!!!\n", str_op);
    exit(1);

    return -1;
}

void Read_tree_file(struct Tree* tree)
{
    struct Parse_inf parse_inf = {};

    //struct Labels* var_buf = (struct Labels*) calloc(NUM_VAR, sizeof(struct Labels));
   // var_buf[0].name = (char*) calloc(2, sizeof(char));

    tree -> var_buf = Names_Table_Ctor();
    //Ptint_Name_Buf(tree -> var_buf);

    FILE* file = fopen("Tree.txt", "r");

    int size_of_file = 0;

    char* buf = Read_file(tree, file, &size_of_file);
    parse_inf.pos = 0;
    parse_inf.str_lex = Sintactic_Pars(tree, buf, size_of_file);
    //Print_Lex_Str(parse_inf.str_lex);
    Print_Lexems(tree, parse_inf.str_lex, size_of_file);

    tree -> root = Create_Tree(&parse_inf);

    fclose(file);

}

struct Labels* Names_Table_Ctor()
{
    struct Labels* names_table = (struct Labels*) calloc(NUM_OF_KWD + 100, sizeof(struct Labels));

    for (int i = 0; i < NUM_OF_KWD; i++)
    {
        names_table[i].name = key_words[i];
        names_table[i].type = KWD;
        names_table[i].val  = 0;
    }

    return names_table;
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END


void Ptint_Name_Buf(struct Labels* labels)
{
    for (int i = 0; i < NUM_OF_KWD + 1; i++)
    {
        printf("enum type = %d\n", labels[i].type);
        printf("str val   = %s\n", labels[i].name);
        printf("num val   = %lg\n\n", labels[i].val);
    }
}
