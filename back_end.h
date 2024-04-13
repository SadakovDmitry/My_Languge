struct Node* Create_Node_from_file(struct Tree* tree, char* tree_buf, int* pos);
void Read_Tree(struct Tree* tree);
struct Node* Read_Node(Tree* tree, char* tree_buf, int* pos);

int Find_Num_of_Spaces(int file_size);
struct Labels* Read_Names_Table (char* buf, int num_sp);
struct Labels* Create_Name_Table_from_file(struct Tree* tree, FILE* file);
char* Remove_Spaces(char* buf, int size);
struct Labels* Add_Variable(struct Labels* names_table);

void Print_Asm_Depens_of_Type(struct Tree* tree, struct Node* node, FILE* file);
void Read_Next_Node(struct Tree* tree, struct Node* node, Tree_t value, char* buf, int* i);
void Insert_Node_from_file(struct Tree* tree, struct Node* node, Tree_t value, char* buf, int* i, int L_or_R_insert);

const int NUM_REG   = 4;
      int NUM_WHILE = 0;
      int NUM_IF    = 0;

const char* const assembler_cmd[] = {
                                        "sin",                    //1
                                        "cos",                    //2
                                        "sqrt",                   //3
                                        "ln",                     //4
                                        "not",                    //5
                                        "out",                    //6
                                        "outc",                   //7
                                        "in",                     //8
                                        "call",                   //9
                                        "ret",                    //10
                                        "add",                    //11
                                        "sub",                    //12
                                        "mul",                    //13
                                        "div",                    //14
                                        "pow",                    //15
                                        "jne",                     //16
                                        "jbe",                     //17
                                        "jae",                     //18
                                        "jb",                    //19
                                        "ja",                    //20
                                        "je",                    //21
                                        "said",                   //22  =
                                        "if",                     //23
                                        "is_very_brief_and_terse",//24  while
                                        "done",                   //25  ;
                                        "}",                      //26
                                        "{",                      //27
                                        ")",                      //28
                                        "(",                      //29
                                        "0",                      //30
                                        "&&",                     //31
                                        "||",                     //32
                                        "dif",                    //33
                                        ","                       //34
                                        };

