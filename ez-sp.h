#ifndef EZ_SP_H
#define EZ_SP_H

#include <stdio.h>
#include <err.h>

/* Specifies the maximum number of tokens per cell,
 * no cell can have more than this, if this number
 * is exceeded an error will pop up
 * */
#define __macro_tokens_per_cell         32

#define __macro_check_ptr(ptr, proc)    do { if (ptr) break; err(-1, "ez-sp:fatal:%s", proc); } while (0)
#define __macro_mark_todo(ToDo)         do { printf("todo:%s:%d: %s\n", __FILE__, __LINE__, ToDo); } while (0)

enum token_kind
{
    token_is_string      = '"',
    token_is_varia_ref   = '@',
    token_is_const_ref   = '$',
    token_is_expr_init   = '=',
    token_is_sub_sign    = '-',
    token_is_mul_sign    = '*',
    token_is_div_sign    = '/',
    token_is_add_sign    = '+',
    token_is_lhs_par     = '(',
    token_is_rhs_par     = ')',
    token_is_conditional = '?',
    token_is_semicolon   = ';',
    token_is_clone_up    = '^',
    token_is_true_bool   = 'T',
    token_is_false_bool  = 'F',
    token_is_number      = 256,
};

/* Premature defintion of `cell` struct in order
 * to be able to have a reference to a cell within
 * a token see `struct token -> as -> ref`
 */
struct cell;

enum cell_kind
{
    cell_is_not_defined  = 0,
    cell_is_number_const = 1,
    cell_is_string_const = 2,
    cell_is_error        = 3
};

struct token
{
    struct
    {
        char         *definition;
        unsigned int length;
        unsigned int numline;
        unsigned int offset;
        unsigned int column;
    } info;
    union
    {
        char        *text;
        long double number;
        struct      { unsigned row, col; struct cell *ptr; } ref;
        
    } as;
    enum token_kind kind;
};

struct cell
{
    struct token   stream[__macro_tokens_per_cell];
    unsigned int   streamsz;
    enum cell_kind kind;
};

struct table
{
    struct cell  *grid;
    unsigned int rows;
    unsigned int cols;
};

struct program
{
    struct
    {
        char  *doc;
        char  *fmt;
        char  *out;
        char  *style;
        char  sep;
    } args;
    struct table  table;
    char          *docstr;
};

#endif
