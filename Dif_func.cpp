#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
#include "const_massivs.h"


void Fill_Labels(struct Labels* labels)
{
    labels[0].name = "e";
    labels[0].type = VAR;
    labels[0].val = 2.7;
    /*
    labels[1].var = "if";
    labels[1].type = KWD;
    labels[1].val = 0;

    labels[2].var = "while";
    labels[2].type = KWD;
    labels[2].val = 0;
    */
}

//                                                    Delete
//------------------------------------------------------------------------------------------------------------------------------
int Delete_SubTree(struct Node* node)
{
    //assert(node);
    if(!node) return 0;

    if(!L)
    {
        Delete_SubTree(L);
        L = NULL;
        free(node -> left);
    }
    if(!R)
    {
        Delete_SubTree(R);
        R = NULL;
        free(node -> right);
    }

    //free(node);
    node = NULL;

    return 1;
}

void Delete_Node(struct Node** node)
{
    free(*node);
    *node = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END


#define var(num) "var"#num


//                                                  Create_Tree
//------------------------------------------------------------------------------------------------------------------------------
int Check_Operation(struct Node* node)
{
    assert(node);

    switch(node -> type)
    {
    case BIN_OP:
    case UN_OP:
        if(node -> right == NULL)
        {
            fprintf(stderr, red(ERROR)"right == NULL after OP node");
            return 0;
        }
        break;
    case NUM:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after NUM node");
            return 0;
        }
        break;
    case VAR:
        if(node -> right != NULL || node -> left != NULL)
        {
            fprintf(stderr, red(ERROR)"right or left != NULL after VAR node");
            return 0;
        }
        break;
    default:
        fprintf(stderr, red(ERROR)" unkniwn type in\"" green(Check_Operation)"\"");
        exit(1);
    }

    return 1;
}

struct Node* Create_Node(enum TYPE type, Tree_t value, struct Node* left_node, struct Node* right_node, struct Tree* tree)
{
    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    new_node -> val   = value;
    new_node -> type  = type;
    new_node -> left  = left_node;
    new_node -> right = right_node;

    if(type == VAR)
    {
        //fprintf(stderr, green(name) " : %s\n", (tree -> var_buf)[value.var_id].name);
        (new_node -> val).var_id  = Add_Variable(tree, tree -> var_buf, (char*) (tree -> var_buf)[value.var_id].name);
        //(new_node -> val).var_id  = Add_Variable(tree, tree -> var_buf, (char*) new_var[NOW_NUM_VAR]);
        //NOW_NUM_VAR++;
    }
    if(type == FUNC)
    {
        //fprintf(stderr, green(name) " : %s\n", (tree -> var_buf)[value.var_id].name);
        (new_node -> val).var_id  = Add_Variable(tree, tree -> var_buf, (char*) (tree -> var_buf)[value.var_id].name);
        //NOW_NUM_FUNC++;
    }


    return new_node;
}

double Find_Var_Val(struct Tree* tree, int ind)
{
    return (tree -> var_buf)[ind].val;
}

void Input_variable(struct Tree* tree)
{
    double value = 0;

    for (int i = 0; i < tree -> num_var; i++)
    {
        printf("\nPrint value of " green(%s)" !\n", (tree -> var_buf)[i].name);
        scanf("%lf", &value);

        (tree -> var_buf)[i].val = value;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                                  Differinstor
//------------------------------------------------------------------------------------------------------------------------------
struct Node* Copy_Subtree(struct Node* node)
{
    if(node == NULL) return NULL;

    struct Node* new_node = (struct Node*) calloc(1, sizeof(struct Node));

    if (node -> type == BIN_OP)
    {
        (new_node -> val).bin_op = (node -> val).bin_op;
    }
    else if (node -> type == UN_OP)
    {
        (new_node -> val).un_op = (node -> val).un_op;
    }
    else if (node -> type == KEY_OP)
    {
        (new_node -> val).key_op = (node -> val).key_op;
    }
    else if (node -> type == NUM)
    {
        (new_node -> val).num = (node -> val).num;
    }
    else if (node -> type == VAR)
    {
        (new_node -> val).var_id = (node -> val).var_id;
    }

    new_node -> type  = node -> type;
    new_node -> left  = Copy_Subtree(node -> left);
    new_node -> right = Copy_Subtree(node -> right);
    //new_node -> prev  = node -> prev;
    new_node -> priority = node -> priority;

    return new_node;
}

//------------------------------------------------------------------------------------------------------------------------------
//                                                      END





//                                              Reduce_Tree_functions
//------------------------------------------------------------------------------------------------------------------------------
struct Node* Copy_Node(struct Node* sourse_node, struct Node* dest_node)
{
    switch(sourse_node -> type)
    {
    case BIN_OP:
        (dest_node -> val).bin_op = (sourse_node -> val).bin_op;
        break;
    case UN_OP:
        (dest_node -> val).un_op  = (sourse_node -> val).un_op;
        break;
    case KEY_OP:
        (dest_node -> val).key_op = (sourse_node -> val).key_op;
        break;
    case NUM:
        (dest_node -> val).num    = (sourse_node -> val).num;
        break;
    case VAR:
        (dest_node -> val).var_id = (sourse_node -> val).var_id;
        break;
    default:
        printf(red(ERROR) "no type node in C\"" green(Copy_Node) "\"");
        exit(1);
    }

    dest_node -> left     = sourse_node -> left;
    dest_node -> right    = sourse_node -> right;
    dest_node -> type     = sourse_node -> type;
    dest_node -> priority = sourse_node -> priority;
    dest_node -> size     = sourse_node -> size;

    return dest_node;
}

double Calculate_val(struct Node* node)
{
    assert(node);
    assert(R);

    double left = 0;

    if(L)
    {
        left = ((node -> left) -> val).num;
    }

    double right = ((node -> right) -> val).num;

    //---------------------------------------------------------------
    #define OP(name, str_op, calculate, ...) \
    case name:                                                           \
        return calculate;
    //---------------------------------------------------------------
    if(node -> type == KEY_OP)
    {
        switch((node -> val).key_op)
        {
        #include "key_operators.h"
        default:
            fprintf(stderr, "\n" red(ERROR) " no operation!!!");
            exit(1);
        }
    }

    if(node -> type == BIN_OP)
    {
        switch((node -> val).bin_op)
        {
        #include "bin_operators.h"
        default:
            fprintf(stderr, "\n" red(ERROR) " no operation!!!");
            exit(1);
        }
    }

    if(node -> type == UN_OP)
    {
        switch((node -> val).un_op)
        {
        #include "un_operations.h"
        default:
            fprintf(stderr, "\n" red(ERROR) " no operation!!!");
            exit(1);
        }
    }

    return 0;

    #undef OP
}

int Is_Zero(struct Node* node)
{
    if(node -> type == NUM && (node -> val).num == 0) return 1;

    return 0;
}

int Is_One(struct Node* node)
{
    if(node -> type == NUM && (node -> val).num == 1) return 1;

    return 0;
}


int Reduce_MUL(struct Tree* tree, struct Node* node)
{
    assert(node);
    assert(node -> right);
    assert(node -> left);
    assert(&((node -> right) -> val));

    if(Is_One(R))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_One(L))
    {
        Node* tmp_node = R;
        Delete_SubTree(L);
        Copy_Node(R, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(L) || Is_Zero(R))
    {
        Delete_SubTree(R);
        Copy_Node(CONST(0), node);

        return 1;
    }

    return 0;
}

int Reduce_ADD(struct Tree* tree, struct Node* node)
{

    if(Is_Zero(L))
    {
        Node* tmp_node = R;
        Delete_SubTree(L);
        Copy_Node(R, node);
        Delete_Node(&(tmp_node));

        return 1;
    }
    if(Is_Zero(R))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_Node(&(tmp_node));

        return 1;
    }

    return 0;
}

int Reduce_POW(struct Tree* tree, struct Node* node)
{
    if(Is_One(L))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_One(R))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(L))
    {
        Node* tmp_node = L;
        Delete_SubTree(R);
        Copy_Node(L, node);
        Delete_SubTree(tmp_node);
        tmp_node = NULL;

        return 1;
    }
    if(Is_Zero(R))
    {
        Delete_SubTree(R);
        Delete_SubTree(L);
        Copy_Node(CONST(1), node);

        return 1;
    }

    return 0;
}


int  Reduce_Node(struct Tree* tree, struct Node* node)
{
    if(node)
    {
        if(L && R)
        {
            if((L) -> type == NUM && (R) -> type == NUM)
            {
                double new_val = Calculate_val(node);
                Copy_Node(CONST(new_val), node);
                return 1;
            }
        }

        if(node -> type == BIN_OP)
        {
            switch((node -> val).bin_op)
            {
            case MUL:
                if (Reduce_MUL(tree, node)) return 1;
                break;
            case ADD:
                if (Reduce_ADD(tree, node)) return 1;
                break;
            case POW:
                if (Reduce_POW(tree, node)) return 1;
                break;
            default:
                break;
            }
        }
    }

    return 0;
}

void Reduce_Tree(struct Tree* tree, struct Node* node)
{
    if(node == NULL) return;

    Check_Operation(node);
    Reduce_Tree(tree, node -> left);

    tree -> version += Reduce_Node(tree, node);

    Check_Operation(node);
    Reduce_Tree(tree, node -> right);

    if(tree -> version != 0)
    {
        tree -> version = 0;
        Reduce_Tree(tree, tree -> root);
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//                                                      END
