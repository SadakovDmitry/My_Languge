#include "parse_func.h"
#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
//#include "/Users/dima/MIPT/Differintsator/work_with_file.h"


// V  - Get_Variable
// F  - Get_Functon
// Op - Get_Operation
// Cond - Get_Condition


// G    ::= F+
// F    ::= 'def' Body | Op
// Op   ::= If | A | Wh | Call | Ret
// Body ::= '{' Op+ '}'
// If   ::= 'if' '('Cond')' Body {'else' Body}?
// Wh   ::= 'while' '('Cond')' Body
// Ass  ::= Id '=' Exp ';'
// Cmp  ::= Exp ['<','>','<=','>='] Exp
// Cond ::= Cmp { Cmp ['&&' , '||'] Cmp }*

// Exp  ::= Term {['+', '-'] Term}*
// Term ::= Pow  {['*', '/'] Pow}*
// Pow  ::= UnOp {'^' UnOp}*
// UnOp ::= {['sin', 'cos', 'tg', 'ctg', ...]}* PrExp
// ...








char* Read_file(FILE* file)
{
    struct stat st = {};

    stat("Expression.txt", &st);
    //tree -> len_tree_buf = st.st_size - 1;

    char* buf = (char*) calloc(st.st_size + 1, sizeof(char));
    fread(buf, sizeof (char), st.st_size, file);

    printf("\nbuf = %s\n", buf);

    return buf;
}


void syn_assert(struct Parse_inf* inf, char smb)
{
    if(smb == ')')
    {
        printf("inf -> pos < (inf -> str_lex)[inf -> pos]");
    }
}


struct Node* Get_Num   (struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = NODE_TYPE;

    if(node_type == NUM /*|| node_type == VAR*/)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        return node;
    }

    return Get_Var(inf);
}

struct Node* Get_Var   (struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = NODE_TYPE;

    if(node_type == VAR)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        return node;
    }

    printf(red(somthing wrong read unknown type)"\n");
    exit(1);

    return NULL;
}

struct Node* Get_Mul_Div   (struct Parse_inf* inf)
{
    struct Node* left = Get_Pow_Log(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    while(node_type == BIN_OP && (node_val.bin_op == DIV || node_val.bin_op == MUL))
    {
        (inf -> pos)++;

        struct Node* right = Get_Pow_Log(inf);

        node -> left = left;
        node -> right = right;

        return node;
    }

    return left;
}

struct Node* Get_Plus_Minus(struct Parse_inf* inf)
{
    struct Node* left = Get_Mul_Div(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    while(node_type == BIN_OP && (node_val.bin_op == ADD || node_val.bin_op == SUB))
    {
        (inf -> pos)++;

        struct Node* right = Get_Mul_Div(inf);

        node -> left  = left;
        node -> right = right;

        return node;
    }

    return left;
}

struct Node* Get_Bracket   (struct Parse_inf* inf)
{
    struct Node* node = NULL;

    enum TYPE node_type = NODE_TYPE;
    Tree_t node_val     = NODE_VAL;

    if(node_type == PUNCT && node_val.punct == L_BRACKET)
    {
        (inf -> pos)++;
        node = Get_Plus_Minus(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == R_BRACKET);
        (inf -> pos)++;

        return node;
    }

    return Get_Num(inf);
}

struct Node* Get_Start     (struct Parse_inf* inf)
{
    inf -> pos = 0;

    //struct Node* node = Get_Plus_Minus(inf);
    struct Node* node = Get_Op(inf);
    //assert((inf -> str)[inf -> pos] == '\0');

    return node;
}

struct Node* Get_Unary_Op  (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == UN_OP)
    {
        switch(node_val.un_op)
        {
        case SIN:
        case COS:
        case LN:
        case SQRT:
            (inf -> pos)++;

            assert ( NODE_TYPE == PUNCT && NODE_VAL.punct == L_BRACKET );
            (inf -> pos)++;

            node -> right = Get_Plus_Minus(inf);

            assert ( NODE_TYPE == PUNCT && NODE_VAL.punct == R_BRACKET );
            (inf -> pos)++;

            return node;
        default:
            break;
        }
    }

    return Get_Bracket(inf);;
}

struct Node* Get_Pow_Log   (struct Parse_inf* inf)
{
    struct Node* left = Get_Unary_Op(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == BIN_OP)
    {
        #warning my_warning wrong type
        switch(node_val.bin_op)
        {
        case POW:
        //case LN:
            (inf -> pos)++;

            node -> right = Get_Unary_Op(inf);
            node -> left = left;

            return node;
        default:
            break;
        }
    }

    return left;
}

struct Node* Create_Tree   (struct Parse_inf* inf)
{
    return Get_Start(inf);
}




struct Node* Get_Id         (struct Parse_inf* inf)
{
    struct Node* left = Get_Var(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == BIN_OP && node_val.bin_op == ASSIGN)
    {
        (inf -> pos)++;

        node -> left = Get_Plus_Minus(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        (inf -> pos)++;

        node -> right  = left;

        return node;
    }

    return left;
}

struct Node* Get_Cmp        (struct Parse_inf* inf)
{
    struct Node* left = Get_Plus_Minus(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == BIN_OP)
    {
        switch(node_val.bin_op)
        {
        case LESS:
        case MORE:
        case MORE_E:
        case LESS_E:
        case EQAL:
        case N_EQAL:
            (inf -> pos)++;

            node -> right = Get_Plus_Minus(inf);
            node -> left  = left;

            return node;
        default:
            break;
        }
    }

    return Get_Id(inf);
    //return left;
}

struct Node* Get_Cond       (struct Parse_inf* inf)
{
    struct Node* left = Get_Cmp(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    while(node_type == BIN_OP && (node_val.bin_op == OR || node_val.bin_op == AND))
    {
        (inf -> pos)++;

        struct Node* right = Get_Cmp(inf);

        node -> left  = left;
        node -> right = right;

        return node;
    }

    return left;
}

struct Node* Get_If         (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];
    struct Node* else_node = NULL;
    struct Node* if_node   = NULL;

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == KEY_OP && node_val.key_op == IF)
    {
        //printf("create if\n");
        (inf -> pos)++;

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == L_BRACKET);
        (inf -> pos)++;

        node -> left  = Get_Cond(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == R_BRACKET);
        (inf -> pos)++;

        if_node = Get_Body(inf);

        if(NODE_TYPE == PUNCT && NODE_VAL.punct == L_FIG_BR)
        {
            else_node = Get_Body(inf);
        }

        node -> right = Create_Node(PUNCT, {.punct = DOT_SUP}, if_node, else_node, NULL);

        return node;
    }

    return Get_Op(inf);
    //return NULL;
}

struct Node* Get_While      (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == KEY_OP && node_val.key_op == WHILE)
    {
        //printf("create while\n");
        (inf -> pos)++;

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == L_BRACKET);
        (inf -> pos)++;

        node -> left  = Get_Cond(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == R_BRACKET);
        (inf -> pos)++;

        node -> right = Get_Body(inf);

        return node;
    }

    return Get_Op(inf);
    //return NULL;
}

struct Node* Get_Out        (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;


    printf("now_node type = %d, now_op = %d\n", node_type, node_val.un_op);
    if(node_type == UN_OP && (node_val.un_op == OUT || node_val.un_op == OUT_C))
    {
        (inf -> pos)++;

        node -> left = NULL;
        node -> right = Get_Var(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        (inf -> pos)++;
        printf("pos = %d\n", inf -> pos);

        return node;
    }

    return NULL;
}

struct Node* Get_Op         (struct Parse_inf* inf)
{
    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if (node_type == KEY_OP)
    {
        switch(node_val.key_op)
        {
        case WHILE:
            printf("read while\n");
            return Get_While(inf);
        case IF:
            printf("read if\n");
            return Get_If(inf);
        default:
            printf(red(ERROR)"\n");
            return NULL;
        }

    }
    if (node_type == VAR)
    {
        if(NEXT_NODE_TYPE == BIN_OP && NEXT_NODE_VAL.bin_op == ASSIGN)
        {
            return Get_Id(inf);
        }
    }
    if(node_type == UN_OP)
    {
        switch(node_val.un_op)
        {
        case OUT:
            return Get_Out(inf);
        default:
            printf(red(sosi huy typoe uebishe) "\n");
            break;
        }
    }

    //return Get_Id(inf);
    return NULL;
}

struct Node* Get_Dot_Sup      (struct Parse_inf* inf)
{
    struct Node* left = Get_Id(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = (inf -> str_lex)[inf -> pos].type;
    Tree_t    node_val  = (inf -> str_lex)[inf -> pos].val;

    if(node_type == PUNCT && node_val.punct == DOT_SUP)
    {
        (inf -> pos)++;

        node -> right = Get_Op(inf);
        node -> left  = left;

        return node;
    }
    //printf("huy4: node_val = %d\n", node_val.op);
    //printf(red(WRONG_OP) "\n");
    exit(1);

    return node;
}


struct Node* Get_Main(struct Parse_inf* inf)
{
    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    struct Node* node      = Get_Func(inf);
    struct Node* next_node = node;

    while (node_type == FUNC)
    {
        next_node -> left = Get_Func(inf);
        next_node          = next_node -> right;

    }

    return node;
}


struct Node* Get_Func         (struct Parse_inf* inf)
{
    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    while (node_type == FUNC)
    {
        assert(NODE_TYPE == KEY_OP && NODE_VAL.var_id == DEF);
        (inf -> pos)++;

        return Get_Body(inf);
    }
}

struct Node* Get_Body    (struct Parse_inf* inf)
{
    struct Node* node           = NULL;
    struct Node* next_node      = NULL;
    struct Node* node_seporator = NULL;

    assert(NODE_TYPE == PUNCT && NODE_VAL.punct == L_FIG_BR);
    (inf -> pos)++;

    while(NODE_TYPE == KEY_OP || NODE_TYPE      == BIN_OP || NODE_TYPE == UN_OP ||
         (NODE_TYPE == VAR    && NEXT_NODE_TYPE == BIN_OP) )
    {
        next_node = Get_Op(inf);

        if(node == NULL)
        {
            node           = Create_Node(PUNCT, {.punct = DOT_SUP}, next_node, NULL, NULL);
            node_seporator = node;
           }
        else
        {
            node_seporator -> right = Create_Node(PUNCT, {.punct = DOT_SUP}, next_node, NULL, NULL);
            node_seporator          = node_seporator -> right;
        }
    }

    assert(NODE_TYPE == PUNCT && NODE_VAL.punct == R_FIG_BR);
    (inf -> pos)++;

    return node;
}








struct Node* Sintactic_Pars(struct Tree* tree, char* buf, int size_of_file)
{
    struct Lexer_inf* lex_inf  = (struct Lexer_inf*) calloc(1, sizeof(Lexer_inf));
    lex_inf -> node_buf = (struct Node*) calloc(SIZE_NODE_BUF, sizeof(Node));
    //int node_buf_pos = 0;
    //int buf_pos = 0;
    lex_inf -> buf = buf;
    lex_inf -> buf_pos = 0;
    lex_inf -> node_buf_pos = 0;

    while(buf_pos < size_of_file)
    {
        //buf_pos = Set_Lex_Val(tree, &node_buf[node_buf_pos], buf, buf_pos, &node_buf_pos);
        Set_Lex_Val(tree, lex_inf);
    }

    node_buf = (struct Node*) realloc(node_buf, sizeof(Node) * (node_buf_pos + 1));
    return node_buf;
}

int Det_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf)
{

    assert(node);
    //assert(isalnum(buf[pos_buf]));

    if(isdigit(buf[pos_buf]))
    {
        double val = 0;

        while(isdigit(buf[pos_buf]))
        {
            val = val * 10 + buf[pos_buf] - '0';
            pos_buf++;
        }

        node -> type    = NUM;
        node -> val.num = val;
        node -> left    = NULL;
        node -> right   = NULL;

        printf("read number : %lg\n", val);
    }
    else
    {
        char* str = &buf[pos_buf];

        while(isalnum(buf[pos_buf]) ||
                     (buf[pos_buf] >= ':' && buf[pos_buf] <= '>') ||
                      buf[pos_buf] == '!' ||
                      buf[pos_buf] == ';' ||
                      buf[pos_buf] == '&' ||
                      buf[pos_buf] == '_' ||
                      buf[pos_buf] == '|')
        //while(buf[pos_buf] != ' ')
        {
            //printf("%c\n", buf[pos_buf]);
            pos_buf++;
        }
        //printf("\n");

        char* new_str = (char*) calloc((&buf[pos_buf] - str + 1), sizeof(char));
        memcpy(new_str, str, (&buf[pos_buf] - str));

        enum TYPE type = Check_Type(new_str);

        if(strcmp(new_str, (tree -> var_buf)[DEF].name) == 0 || strcmp(new_str, (tree -> var_buf)[CALL].name) == 0)
        {
            node -> type = KEY_OP;
            (node -> val).var_id = Add_Function(tree, tree -> var_buf, new_str);
            node -> right = NULL;
            node -> left  = NULL;

            printf("read operation : %s\n", new_str);
        }

        if(type == BIN_OP || type == UN_OP || type == KEY_OP || type == PUNCT)
        {
            int id = Convert_op_to_enum(new_str);
            node -> type = type;
            (node -> val).var_id = id;

            printf("read operation : %s\n", new_str);
        }
        /*
        else if (type == FUNC)
        {
            node -> type = KEY_OP;
            (node -> val).var_id = Add_Variable(tree, tree -> var_buf, new_str);
            node -> right = NULL;
            node -> left  = NULL;
        }
        */
        else
        {
            node -> type = VAR;
            (node -> val).var_id = Add_Variable(tree, tree -> var_buf, new_str);
            node -> right = NULL;
            node -> left  = NULL;

            printf("read variable : %s\n", new_str);
        }
    }

    return pos_buf - 1;
}

//int Set_Lex_Val(struct Tree* tree, struct Node* node, char* buf, int pos_buf, int* node_buf_pos)
int Set_Lex_Val(struct Tree* tree, struct Lexer_inf* lex_inf)
{
/*
    //----------------------------------------------------------------------
    #define OP(name, str, ...)                          \
    case str:                                           \
        case '+':                                       \
        node -> type = OP;                              \
        (node -> val).op = name;                        \
        printf("read op : %c\n", buf[pos_buf]);         \
        break;
    //----------------------------------------------------------------------
*/

    switch((lex_inf -> buf)[lex_inf -> pos_buf])
    {
    case '\n':
    case ' ':
        fprintf(stderr, "read op : <%c>\n", buf[pos_buf]);
        (*node_buf_pos)--;
        break;
    case '+':
        node -> type = BIN_OP;
        (node -> val).bin_op = ADD;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '-':
        node -> type = BIN_OP;
        (node -> val).bin_op = SUB;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '*':
        node -> type = BIN_OP;
        (node -> val).bin_op = MUL;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '^':
        node -> type = BIN_OP;
        (node -> val).bin_op = POW;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '/':
        node -> type = BIN_OP;
        (node -> val).bin_op = DIV;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '(':
        node -> type = PUNCT;
        (node -> val).punct = L_BRACKET;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case ')':
        node -> type = PUNCT;
        (node -> val).punct = R_BRACKET;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '{':
        node -> type = PUNCT;
        (node -> val).punct = L_FIG_BR;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '}':
        node -> type = PUNCT;
        (node -> val).punct = R_FIG_BR;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case '\0':
        node -> type = PUNCT;
        (node -> val).punct = ZERO;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    case ';':
        node -> type = PUNCT;
        (node -> val).punct = DOT_SUP;
        printf("read op : %c\n", buf[pos_buf]);
        break;
    default:
        pos_buf = Det_Lex_Val(tree, node, buf, pos_buf, node_buf_pos);
    }

    pos_buf++;
    (*node_buf_pos)++;

    return pos_buf;
    /*
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
            (node -> val).var = Add_Variable(tree, tree -> var_buf, str);
        }

        return i + strlen(str);
    }
    */
}




void Print_Lex_Str(struct Node* node)
{
    printf("\n\n");
    for(int i = 0; i < 9; i++)
    {
        if(node[i].type == BIN_OP)
        {
            printf("BIN_OP:%d\n", node[i].val.bin_op);
        }
        if(node[i].type == UN_OP)
        {
            printf("UN_OP:%d\n", node[i].val.un_op);
        }
        if(node[i].type == KEY_OP)
        {
            printf("KEY_OP:%d\n", node[i].val.key_op);
        }
        if(node[i].type == VAR)
        {
            printf("VAR:%d\n", node[i].val.var_id);
        }
        if(node[i].type == NUM)
        {
            printf("NUM:%lf\n", node[i].val.num);
        }
    }
}

void Print_Node(struct Tree* tree, struct Node* node)
{
    printf("\n");
    if(node == NULL)
    {
        printf("NULL\n");
        return;
    }
    if(node -> type == UN_OP)
    {
        printf("UN_OP:%s\n", (tree -> var_buf)[(node -> val).un_op].name);
        //printf("R:%p\n",   node -> right);
        //printf("L:%p\n",   node -> left);
    }
    if(node -> type == KEY_OP)
    {
        printf("KEY_OP:%s\n", (tree -> var_buf)[(node -> val).key_op].name);
        //printf("R:%p\n",   node -> right);
        //printf("L:%p\n",   node -> left);
    }
    if(node -> type == BIN_OP)
    {
        printf("BIN_OP:%s\n", (tree -> var_buf)[(node -> val).bin_op].name);
        //printf("R:%p\n",   node -> right);
        //printf("L:%p\n",   node -> left);
    }
    if(node -> type == PUNCT)
    {
        printf("PUNCT:%s\n", (tree -> var_buf)[(node -> val).punct].name);
        //printf("PUNCT:%d\n", (node -> val).punct);
        //printf("R:%p\n",   node -> right);
        //printf("L:%p\n",   node -> left);
    }
    if(node -> type == VAR)
    {
        printf("VAR:%s\n",(tree -> var_buf)[(node -> val).var_id].name);
        //printf("R:%p\n",   node -> right);
        //printf("L:%p\n",   node -> left);
    }
    if(node -> type == NUM)
    {
        printf("NUM:%lg\n", node -> val.num);
        //printf("R:%p\n",    node -> right);
        //printf("L:%p\n",    node -> left);
    }
    //printf("\n\n");
}

void Print_Lexems(struct Tree* tree, struct Node* node_buf, int file_size)
{
    printf(green(--------------------LEXEMS--------------------\n));
    for(int i = 0; i < file_size - 160 /*/ (sizeof(struct Labels))*/; i++)
    {
        Print_Node(tree, &(node_buf[i]));
    }
    printf(green(----------------------------------------------\n));
}
