#include "parse_func.h"
#include "lib.h"
#include "dif_func.h"
#include "work_with_file.h"
//#include "/Users/dima/MIPT/Differintsator/work_with_file.h"


// V  - Get_Variable
// F  - Get_Functon
// Op - Get_Operation
// Cond - Get_Condition


// Main ::= F+
// F    ::= 'def' Arg Body
// Arg  ::= '(' {V ','? }* ')'
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

/*
void syn_assert(struct Parse_inf* inf, char smb)
{
    if(smb == ')')
    {
        printf("inf -> pos < (inf -> str_lex)[inf -> pos]");
    }
}
*/

struct Node* Get_Num       (struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = NODE_TYPE;

    printf(green(in Num type) " = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);
    if(node_type == NUM /*|| node_type == VAR*/)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        return node;
    }

    return Get_Var(inf);
}

struct Node* Get_Var       (struct Parse_inf* inf)
{
    struct Node* node = NULL;
    enum TYPE node_type = NODE_TYPE;

    if(node_type == VAR)
    {
        node =  &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        return node;
    }

    printf(green(in var type) " = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);
    printf(red(somthing wrong read unknown type)"\n");
    //exit(1);

    return Get_Call(inf);
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
    struct Node* node = Get_Main(inf);
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
        //#warning my_warning wrong type
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
    struct Node* left = Get_Num(inf);
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if(node_type == BIN_OP && node_val.bin_op == ASSIGN)
    {
        (inf -> pos)++;

        node -> left = Get_Plus_Minus(inf);

        printf(green(in Id type) " = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);
        //assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        if(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP)
        {
            (inf -> pos)++;
        }

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
        (inf -> pos)++;

        printf(green(in if type) " = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);
        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == L_BRACKET);
        (inf -> pos)++;

        node -> left  = Get_Cond(inf);

        printf(green(in if type) " = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);
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

    //printf("now_node type = %d, now_op = %d\n", node_type, node_val.un_op);
    if(NODE_TYPE == UN_OP && (NODE_VAL.un_op == OUT || NODE_VAL.un_op == OUT_C))
    {
        (inf -> pos)++;

        node -> left = NULL;
        node -> right = Get_Num(inf);

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        (inf -> pos)++;
        //printf("pos = %d\n", inf -> pos);

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
            return Get_While(inf);
        case IF:
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
        case OUT_C:
            return Get_Out(inf);
        case RET:
            return Get_Ret(inf);
        case CALL:
            return Get_Call(inf);
        case IN:
            return Get_In(inf);
        default:
            printf(red(sosi huy typoe uebishe) "\n");
            break;
        }
    }

    //return Get_Id(inf);
    return NULL;
}

struct Node* Get_Dot_Sup    (struct Parse_inf* inf)
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

struct Node* Get_Main       (struct Parse_inf* inf)
{
    struct Node* main_node = Get_Func(inf);
    struct Node* func_node = main_node;

    while (NODE_TYPE == KEY_OP && NODE_VAL.key_op == DEF)
    {
        func_node -> right = Get_Func(inf);
        func_node          = func_node -> right;
    }

    return main_node;
}

struct Node* Get_Func       (struct Parse_inf* inf)
{
    struct Node* node_body = NULL;
    struct Node* node_arg  = NULL;

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;

    if (node_type == KEY_OP && node_val.key_op == DEF)
    {
        assert(NODE_TYPE == KEY_OP && NODE_VAL.var_id == DEF);
        (inf -> pos)++;

        struct Node* node_func = &(inf -> str_lex)[inf -> pos];
        //printf("type = %d, val = %d\n", NODE_TYPE, NODE_VAL.var_id);

        (inf -> pos)++;

        node_arg          = Get_Arg(inf);
        node_body         = Get_Body(inf);
        node_func -> left = Create_Node(PUNCT, {.punct = DOT_SUP}, node_body, node_arg, NULL);

        return node_func;
    }

    return NULL;
}

struct Node* Get_Ret        (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    enum TYPE node_type = NODE_TYPE;
    Tree_t    node_val  = NODE_VAL;


    //printf("now_node type = %d, now_op = %d\n", node_type, node_val.un_op);
    if(node_type == UN_OP && node_val.un_op == RET)
    {
        (inf -> pos)++;

        node -> left = NULL;
        //if(NODE_TYPE == BIN_OP)
        //{
            node -> right = Get_Plus_Minus(inf);
        //}
        //else
        //{
            //node -> right = NULL;
        //}

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        (inf -> pos)++;
        //printf("pos = %d\n", inf -> pos);

        return node;
    }

    return NULL;
}

struct Node* Get_Call       (struct Parse_inf* inf)
{
    struct Node* node      = &(inf -> str_lex)[inf -> pos];
    struct Node* func_node = NULL;
    struct Node* arg_node  = NULL;

    //printf("now_node type = %d, now_op = %d\n", node_type, node_val.un_op);
    if(NODE_TYPE == UN_OP && NODE_VAL.un_op == CALL)
    {
        (inf -> pos)++;

        func_node = &(inf -> str_lex)[inf -> pos];
        (inf -> pos)++;

        arg_node  = Get_Arg(inf);

        node -> right = Create_Node(PUNCT, {.punct = DOT_SUP}, func_node, arg_node, NULL);
        node -> left  = NULL;

        assert(NODE_TYPE == PUNCT && NODE_VAL.punct == DOT_SUP);
        (inf -> pos)++;
        //printf("pos = %d\n", inf -> pos);

        return node;
    }

    return NULL;
}

struct Node* Get_Arg        (struct Parse_inf* inf)
{
    struct Node* now_node = NULL;
    struct Node* node_arg = NULL;
    struct Node* node_var = NULL;

    //fprintf(stderr, "have: %d\n",NODE_VAL.var_id);
    assert(NODE_TYPE == PUNCT && NODE_VAL.var_id == L_BRACKET);
    (inf -> pos)++;

    node_arg = Create_Node(PUNCT, {.punct = DOT_SUP}, NULL, NULL, NULL);

    while(!(NODE_TYPE == PUNCT && NODE_VAL.punct == R_BRACKET))
    {
        //node_var = Get_Plus_Minus(inf);
        node_var = Get_Id(inf);

        if(now_node == NULL)
        {
            node_arg -> left = node_var;
            now_node = node_arg;
        }
        else
        {
            now_node -> right = Create_Node(PUNCT, {.punct = DOT_SUP}, node_var, NULL, NULL);
            now_node = now_node -> right;
        }

        if(NODE_TYPE == PUNCT && NODE_VAL.punct == COM)
        {
            (inf -> pos)++;
        }
        else
        {
            break;
        }
    }

    assert(NODE_TYPE == PUNCT && NODE_VAL.var_id == R_BRACKET);
    (inf -> pos)++;

    return node_arg;
}

struct Node* Get_Body       (struct Parse_inf* inf)
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

struct Node* Get_In        (struct Parse_inf* inf)
{
    struct Node* node = &(inf -> str_lex)[inf -> pos];

    //printf("now_node type = %d, now_op = %d\n", NODE_TYPE, NODE_VAL.un_op);
    if(NODE_TYPE == UN_OP && NODE_VAL.un_op == IN)
    {
        (inf -> pos)++;

        node -> left  = NULL;
        node -> right = Get_Num(inf);

        return node;
    }

    return NULL;
}






struct Node* Lexecal_Pars(struct Tree* tree, char* buf, int size_of_file)
{
    struct Lexer_inf* lex_inf  = (struct Lexer_inf*) calloc(1, sizeof(Lexer_inf));
    lex_inf -> node_buf = (struct Node*) calloc(SIZE_NODE_BUF, sizeof(Node));
    lex_inf -> buf = buf;
    lex_inf -> buf_pos = 0;
    lex_inf -> node_buf_pos = 0;

    while(lex_inf -> buf_pos < size_of_file)
    {
        Set_Lex_Val(tree, lex_inf);
    }

    lex_inf -> node_buf = (struct Node*) realloc(lex_inf ->node_buf, sizeof(Node) * ((lex_inf -> node_buf_pos) + 1));

    return lex_inf -> node_buf;
}

char* Read_Str(struct Lexer_inf* lex_inf)
{
    char* buf = lex_inf -> buf;
    char* str = &buf[lex_inf -> buf_pos];

    while(isalnum(buf[lex_inf -> buf_pos]) ||
                 (buf[lex_inf -> buf_pos] >= ':' && buf[lex_inf -> buf_pos] <= '>') ||
                  buf[lex_inf -> buf_pos] == '!' ||
                  buf[lex_inf -> buf_pos] == ';' ||
                  buf[lex_inf -> buf_pos] == '&' ||
                  buf[lex_inf -> buf_pos] == '_' ||
                  buf[lex_inf -> buf_pos] == '|')
    //while(buf[pos_buf] != ' ')
    {
        //printf("%c\n", buf[pos_buf]);
        (lex_inf -> buf_pos)++;
    }
    //printf("\n");

    char* new_str = (char*) calloc((&buf[lex_inf -> buf_pos] - str + 1), sizeof(char));
    memcpy(new_str, str, (&buf[lex_inf -> buf_pos] - str));

    return new_str;
}

int Det_Lex_Val(struct Tree* tree, struct Lexer_inf* lex_inf)
{
    struct Node* node = &(lex_inf -> node_buf)[lex_inf -> node_buf_pos];
    struct Node* next_node = &(lex_inf -> node_buf)[lex_inf -> node_buf_pos + 1];
    char* buf = lex_inf -> buf;

    assert(node);

    if(isdigit(buf[lex_inf -> buf_pos]))
    {
        double val = 0;

        while(isdigit(buf[lex_inf -> buf_pos]) || buf[lex_inf -> buf_pos] == '-')
        {
            if(buf[lex_inf -> buf_pos] == '-')
            {
                val = val * -1;
            }
            {
                val = val * 10 + buf[lex_inf -> buf_pos] - '0';
            }
            (lex_inf -> buf_pos)++;
        }

        node -> type    = NUM;
        node -> val.num = val;
        node -> left    = NULL;
        node -> right   = NULL;

        printf("read number : %lg\n", val);
    }
    else
    {
        char* new_str = Read_Str(lex_inf);

        enum TYPE type = Check_Type(new_str);

        if(strcmp(new_str, (tree -> var_buf)[DEF].name) == 0 || strcmp(new_str, (tree -> var_buf)[CALL].name) == 0)
        {
            int id = Convert_op_to_enum(new_str);
            node -> type = type;
            (node -> val).var_id = id;

            printf("read operation : %s\n", new_str);

            Skip_Space(buf, &(lex_inf -> buf_pos));
            new_str = Read_Str(lex_inf);

             next_node -> type         = FUNC;
            (next_node -> val).func_id = Add_Function(tree, tree -> var_buf, new_str);
             next_node -> right        = NULL;
             next_node -> left         = NULL;
            (lex_inf -> node_buf_pos)++;

            printf("read func : %s\n", (tree -> var_buf)[(next_node -> val).func_id].name);
        }
        else if(type == BIN_OP || type == UN_OP || type == KEY_OP || type == PUNCT)
        {
            int id = Convert_op_to_enum(new_str);
            node -> type = type;
            (node -> val).var_id = id;

            printf("read operation : %s\n", new_str);
        }
        else
        {
            node -> type = VAR;
            (node -> val).var_id = Add_Variable(tree, tree -> var_buf, new_str);
            node -> right = NULL;
            node -> left  = NULL;

            printf("read variable : %s\n", new_str);
        }
    }

    return (lex_inf -> buf_pos) - 1;
}

int Set_Lex_Val(struct Tree* tree, struct Lexer_inf* lex_inf)
{
    struct Node* node = &(lex_inf -> node_buf)[lex_inf -> node_buf_pos];
    char* buf     = lex_inf -> buf;
    int   pos_buf = lex_inf -> buf_pos;

    switch(buf[lex_inf -> buf_pos])
    {
    case '\n':
    case ' ':
        fprintf(stderr, "read op : <%c>\n", buf[pos_buf]);
        (lex_inf -> node_buf_pos)--;
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
        lex_inf -> buf_pos = Det_Lex_Val(tree, lex_inf);
    }

    (lex_inf -> node_buf_pos)++;
    lex_inf -> buf_pos++;

    return lex_inf -> buf_pos;
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
    if(node -> type == FUNC)
    {
        printf("FUNC:%s\n", (tree -> var_buf)[(node -> val).func_id].name);
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
    for(int i = 0; i < file_size - 100 /*/ (sizeof(struct Labels))*/; i++)
    {
        Print_Node(tree, &(node_buf[i]));
    }
    printf(green(----------------------------------------------\n));
}
