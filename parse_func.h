#ifndef USED_PARSE_FUNC
#define USED_PARSE_FUNC


#define red(str) "\033[31m"#str"\033[0m"
#define green(str) "\033[32m"#str"\033[0m"

#include "lib.h"

struct Parse_inf
{
    struct Node* str_lex;
    int pos;
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

struct Node* Get_Id        (struct Parse_inf* inf);
struct Node* Get_Cmp       (struct Parse_inf* inf);
struct Node* Get_If        (struct Parse_inf* inf);
struct Node* Get_While     (struct Parse_inf* inf);
struct Node* Get_Op        (struct Parse_inf* inf);
struct Node* Get_Dot_Sup   (struct Parse_inf* inf);

struct Node* Create_Tree(struct Parse_inf* inf);


struct Node* Sintactic_Pars(struct Tree* tree, char* buf, int size_of_file);
int Set_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf, int* node_buf_pos);
int Det_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf);
void Print_Lex_Str(struct Node* node);
void Print_Node(struct Node* node);

#endif
