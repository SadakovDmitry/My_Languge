#ifndef USED_WWF
#define USED_WWF

#include "lib.h"

void Beautiful_Dump();
void Print_to_TEX(struct Node* node, FILE* file_tex);
void Print_Operation(enum OPERATION op, FILE* file);
void Print_Operation_to_TEX(enum OPERATION op, FILE* file);
void Print_Node(struct Tree* tree, struct Node* node, FILE* file);
void Print_In_Order(struct Tree* tree, struct Node* node, FILE* file);
void Tree_Dump(struct Tree* tree);
enum   OPERATION Convert_op_to_enum(struct Node* node, char* str_op);
char*  Read_file(FILE* file);
struct Node* Read_and_Insert_Node(struct Tree* tree, struct Node* now_node, char* buf, int indicator, int i);
void Convert_Pre_Order_Tree(struct Tree* tree);
void Convert_In_Order_Tree(struct Tree* tree);
void Print_Node_depends_of_type(struct Tree* tree,struct Node* node, FILE* file_dot);
void Print_Node_to_file(struct Tree* tree, struct Node* node, FILE* file_dot);
void Arrows_in_Graph(struct Node* node, FILE* file_dot);
void Draw_Graph(struct Tree* tree);
void Read_tree_file(struct Tree* tree);
void Insert_Node_from_file(struct Tree* tree, struct Node* node, Tree_t value, char* buf, int* i, int L_or_R_insert);
void Skip_Value(char* buf, int* i);
void Print_Title_TEX(FILE* file_tex);
void Print_Tex(struct Tree* tree, struct Node* node, FILE* file_tex, struct Remove* rems);
void Print_Node_to_Tex(struct Tree* tree, struct Node* node, FILE* file_tex);
int  Set_Node_Value(struct Tree* tree, struct Node* node, char* buf, int i);
enum TYPE Check_Type(char* str_val);
int  Add_Variable(struct Tree* tree, struct Labels* var_buf, char* name);
int  Set_Node_Value(struct Tree* tree, struct Node* node, char* buf, int i);

#endif
