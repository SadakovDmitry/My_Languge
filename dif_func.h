#ifndef USED_DF
#define USED_DF

#include "lib.h"

#define tree_t "%s"
#define red(str) "\033[31m"#str"\033[0m"
#define green(str) "\033[32m"#str"\033[0m"

#define L               node->left
#define R               node->right
#define Ls              Calculate_Size((node->left))
#define Rs              Calculate_Size((node->right))

#define CONST(val)      Create_Node(NUM, {.num = val}, NULL, NULL, tree)
#define D(node)         Der(tree, node, rems, file_tex)
#define C(node)         Copy_Subtree(node)
#define E               Create_Node(VAR, {.name = "e"}, NULL, NULL, tree)
#define VAR(var)        Create_Node(VAR, {.name = var}, NULL, NULL, tree)

#define ADD(left,right) Create_Node(OP, {.op = ADD},  left, right, tree)
#define SUB(left,right) Create_Node(OP, {.op = SUB},  left, right, tree)
#define MUL(left,right) Create_Node(OP, {.op = MUL},  left, right, tree)
#define DIV(left,right) Create_Node(OP, {.op = DIV},  left, right, tree)
#define TAN(right)      Create_Node(OP, {.op = TAN},  NULL, right, tree)
#define COT(right)      Create_Node(OP, {.op = COT},  NULL, right, tree)
#define SIN(right)      Create_Node(OP, {.op = SIN},  NULL, right, tree)
#define COS(right)      Create_Node(OP, {.op = COS},  NULL, right, tree)
#define SQRT(right)     Create_Node(OP, {.op = SQRT}, NULL, right, tree)
#define POW(left,right) Create_Node(OP, {.op = POW},  left, right, tree)
#define LOG(left,right) Create_Node(OP, {.op = LOG},  left, right, tree)
#define LN(right)       Create_Node(OP, {.op = LOG},  E   , right, tree)

const int SIZE_STR = 100;
const int INSERT_LEFT = 1;
const int INSERT_RIGHT = -1;
const int NUM_PRIORITY = 3;
const int MAX_LEN_EXP = 5000;
const int NUM_VAR = 50;
const int SIZE_PHRASE_BANK = 16;
const int MAX_NUM_REPLASES = 25;
const int SIZE_NODE_BUF = 100;

enum TYPE
{
    NUM    = 0,
    OP     = 1,
    VAR    = 2,
    KWD    = 3
};



enum OPERATION
{
    //-----------------------------------
    #define OP(name, str, num_op, ...)\
        name   = num_op,
    //-----------------------------------

    #include "operators.h"
/*
    ADD       = 0,
    SUB       = 1,
    MUL       = 2,
    DIV       = 3,
    SQRT      = 4,
    SIN       = 5,
    COS       = 6,
    TAN       = 7,
    COT       = 8,
    POW       = 9,
    LOG       = 10,
    R_BRACKET = 11,
    L_BRACKET = 12,
    ZERO      = 13,
    ASSIGN    = 14,
    EQAL      = 15,
    MORE      = 16,
    LESS      = 17,
    MORE_E    = 18,
    LESS_E    = 19,
    N_EQAL    = 20,
    IF        = 21,
    WHILE     = 22
*/
    NO_VAL     = 222
};

//const char operations[] = "ADD SUB MUL DIV SQRT";
const char operations[] = " + - * / √ sin cos log ^ tg ctg ( ) if while <= >= == = !=";

struct Tree
{
    struct Node* root;
    int size;
    int len_tree_buf;
    struct Labels* var_buf;
    int num_var;
    int version;
};

union Value
{
    enum OPERATION op;
    double num;
    char* name;
    int var_id;
};

struct Labels
{
    enum TYPE type;
    char* var;
    double val;
};

typedef Value Tree_t;

struct Node
{
    enum TYPE type;
    Tree_t val;
    int priority;
    int size;
    struct Node* left;
    struct Node* right;
};

struct Remove
{
    char* name;
    struct Node* rem;
    int num_rems;
};

struct Node* Create_Node(enum TYPE type, Tree_t value, struct Node* left_node, struct Node* right_node, struct Tree* tree);

void Input_variable(struct Tree* tree);

struct Node* Copy_Subtree(struct Node* node);

void   Reduce_Tree(struct Tree* tree, struct Node* node);

int    Reduce_Const (struct Node* node);

double Calculate_val(struct Node* node);

int Delete_SubTree(struct Node* node);

int Calculate_Size(struct Node* node);

struct Node* Copy_Node(struct Node* sourse_node, struct Node* dest_node);

void Fill_Labels(struct Labels* labels);

#endif


