#ifndef USED_DF
#define USED_DF

#include "lib.h"

#define tree_t "%s"
#define red(str)    "\033[31m"#str"\033[0m"
#define green(str)  "\033[32m"#str"\033[0m"
#define yellow(str) "\033[33m"#str"\033[0m"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define L               node->left
#define R               node->right
#define Ls              Calculate_Size((node->left))
#define Rs              Calculate_Size((node->right))

#define CONST(val)      Create_Node(NUM, {.num = val}, NULL, NULL, tree)
#define D(node)         Der(tree, node, rems, file_tex)
#define C(node)         Copy_Subtree(node)
#define E               Create_Node(VAR, {.name = "e"}, NULL, NULL, tree)
#define VAR(var)        Create_Node(VAR, {.name = var}, NULL, NULL, tree)

#define ADD(left,right) Create_Node(BIN_OP, {.op = ADD},  left, right, tree)
#define SUB(left,right) Create_Node(BIN_OP, {.op = SUB},  left, right, tree)
#define MUL(left,right) Create_Node(BIN_OP, {.op = MUL},  left, right, tree)
#define DIV(left,right) Create_Node(BIN_OP, {.op = DIV},  left, right, tree)
#define SIN(right)      Create_Node(UN_OP , {.op = SIN},  NULL, right, tree)
#define COS(right)      Create_Node(UN_OP , {.op = COS},  NULL, right, tree)
#define SQRT(right)     Create_Node(UN_OP , {.op = SQRT}, NULL, right, tree)
#define POW(left,right) Create_Node(BIN_OP, {.op = POW},  left, right, tree)
#define LN(right)       Create_Node(BIN_OP, {.op = LOG},  E   , right, tree)

#define SET_RUS setlocale(LC_ALL, "Rus");

const int SIZE_STR          = 100;
const int INSERT_LEFT       = 1;
const int INSERT_RIGHT      = -1;
const int NUM_PRIORITY      = 3;
const int NUM_VAR           = 50;
const int SIZE_PHRASE_BANK  = 16;
const int SIZE_NODE_BUF     = 1000;
const int NUM_OF_KWD        = 34;


enum IND_START_TYPES
{
    UN_OP_START  = 0,
    BIN_OP_START = 10,
    KEY_OP_START = 22,
    PUNCT_START  = 24
};


enum TYPE
{
    PUNCT  = 0,
    BIN_OP = 1,
    UN_OP  = 2,
    KEY_OP = 3,
    NUM    = 4,
    VAR    = 5,
    FUNC   = 6,
    STR    = 7,
    KWD    = 8
};

enum KEY_OPERATIONS
{
    IF    = 22,
    WHILE = 23,
    DEF   = 32
};

enum BIN_OPERATIONS
{
    ADD    = 10,
    SUB    = 11,
    MUL    = 12,
    DIV    = 13,
    POW    = 14,
    EQAL   = 15,
    MORE   = 16,
    LESS   = 17,
    MORE_E = 18,
    LESS_E = 19,
    N_EQAL = 20,
    ASSIGN = 21,
    AND    = 30,
    OR     = 31
};

enum UN_OPERATIONS
{
    SIN   = 0,
    COS   = 1,
    SQRT  = 2,
    LN    = 3,
    NOT   = 4,
    OUT   = 5,
    OUT_C = 6,
    IN    = 7,
    CALL  = 8,
    RET   = 9
};

enum PUNCTUATION
{
    ZERO      = 29,
    L_BRACKET = 28,
    R_BRACKET = 27,
    L_FIG_BR  = 26,
    R_FIG_BR  = 25,
    DOT_SUP   = 24,
    COM       = 33
};

const char* const un_operations[]  = {"sin", "cos", "sqrt", "in", "call", "ret", "out", "outc", "ln", "!"};
const char* const punctuation[]    = {"done", "(", ")", "{", "}", ","};
const char* const key_operations[] = {"if", "is_very_brief_and_terse", "def"};
const char* const bin_operations[] = {"+", "-", "*", "/", "^", "<", ">", "<=", ">=", "==" ,"said", "!=", "&&", "||"};

const char* const new_var[] = {"a", "b", "c", "d", "e", "f", "g"};
const char* const new_func[] = {"func_1", "func_2", "func_3", "func_4", "func_5", "func_6", "func_7", "func_8"};
//int NOW_NUM_FUNC = 0;
//int NOW_NUM_VAR  = 0;

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
    enum BIN_OPERATIONS bin_op;
    enum UN_OPERATIONS  un_op;
    enum KEY_OPERATIONS key_op;
    enum PUNCTUATION    punct;
    double num;
    char* name;
    int var_id;
    int func_id;
};

struct Labels
{
    enum TYPE type;
    const char* name;
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


