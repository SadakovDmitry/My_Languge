#ifndef USED_PARSE_FUNC
#define USED_PARSE_FUNC

#include "dif_func.h"

#define red(str)   "\033[31m"#str"\033[0m"
#define green(str) "\033[32m"#str"\033[0m"

#define NODE_TYPE      (inf -> str_lex)[inf -> pos].type
#define NODE_VAL       (inf -> str_lex)[inf -> pos].val
#define NEXT_NODE_TYPE (inf -> str_lex)[inf -> pos + 1].type
#define NEXT_NODE_VAL  (inf -> str_lex)[inf -> pos + 1].val

#include "lib.h"

struct Parse_inf
{
    struct Node* str_lex;
    int pos;
};

struct Lexer_inf
{
    struct Node* node_buf;
    char* buf;
    int node_buf_pos;
    int buf_pos;
};


char*  Read_file (FILE* file);
struct Node* Get_Bracket   (struct Parse_inf* inf);
struct Node* Get_Num       (struct Parse_inf* inf);
struct Node* Get_Var       (struct Parse_inf* inf);
struct Node* Get_Mul_Div   (struct Parse_inf* inf);
struct Node* Get_Start     (struct Parse_inf* inf);
struct Node* Get_Plus_Minus(struct Parse_inf* inf);
struct Node* Get_Pow_Log   (struct Parse_inf* inf);
struct Node* Get_Unary_Op  (struct Parse_inf* inf);

struct Node* Get_Main      (struct Parse_inf* inf);
struct Node* Get_Func      (struct Parse_inf* inf);
struct Node* Get_Arg       (struct Parse_inf* inf);
struct Node* Get_Id        (struct Parse_inf* inf);
struct Node* Get_Cmp       (struct Parse_inf* inf);
struct Node* Get_Cond      (struct Parse_inf* inf);
struct Node* Get_If        (struct Parse_inf* inf);
struct Node* Get_While     (struct Parse_inf* inf);
struct Node* Get_Op        (struct Parse_inf* inf);
struct Node* Get_Dot_Sup   (struct Parse_inf* inf);
struct Node* Get_Body      (struct Parse_inf* inf);
struct Node* Get_Ret       (struct Parse_inf* inf);
struct Node* Get_Out       (struct Parse_inf* inf);
struct Node* Get_Call      (struct Parse_inf* inf);
struct Node* Get_In        (struct Parse_inf* inf);

struct Node* Create_Tree(struct Parse_inf* inf);

/*
struct Labels labels[100] = {
    {KWD, "sin"  , 0},
    {KWD, "cos"  , 0},
    {KWD, "√"    , 0},
    {KWD, "ln"   , 0},
    {KWD, "!"    , 0},
    {KWD, "out"  , 0},
    {KWD, "out_c", 0},
    {KWD, "in"   , 0},
    {KWD, "call" , 0},
    {KWD, "ret"  , 0},
    {KWD, "add"  , 0},
    {KWD, "sub"  , 0},
    {KWD, "mul"  , 0},
    {KWD, "div"  , 0},
    {KWD, "pow"  , 0},
    {KWD, "=="   , 0},
    {KWD, ">"    , 0},
    {KWD, "<"    , 0},
    {KWD, ">="   , 0},
    {KWD, "<="   , 0},
    {KWD, "!="   , 0},
    {KWD, "="    , 0},
    {KWD, "if"   , 0},
    {KWD, "while", 0},
    {KWD, "def"  , 0},
    {KWD, "call" , 0}
    };
*/

struct Node* Lexecal_Pars(struct Tree* tree, char* buf, int size_of_file);
//int Set_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf, int* node_buf_pos);
int Set_Lex_Val(struct Tree* tree, struct Lexer_inf* lex_inf);
//int Det_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf);
int Det_Lex_Val(struct Tree* tree, struct Lexer_inf* lex_inf);
void Print_Lex_Str(struct Node* node);
void Print_Node(struct Node* node);
void Print_Lexems(struct Tree* tree, struct Node* node_buf, int file_size);
char* Read_Str(struct Lexer_inf* lex_inf);

#endif
