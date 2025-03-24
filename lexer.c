#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

static void get_table_size (char*, unsigned int*, unsigned*, const char);

void lexer_ (struct program *_p, const size_t bytes)
{
    get_table_size(_p->docstr, &_p->table.rows, &_p->table.cols, _p->args.sep);
    _p->table.grid = (struct cell*) calloc(_p->table.cols * _p->table.rows, sizeof(struct cell));
    assert(_p->table.grid && "internal error; cannot alloc");

    unsigned int numline = 1, offset = 0, row = 0, col = 0;

    for (size_t i = 0; i < bytes; i++)
    {
        const char chr = _p->docstr[i];
        if (isspace(chr))
        {
            continue;
        }
        if (chr == _p->args.sep)
        {
            continue;
        }

        switch (chr)
        {
            case token_is_conditional:
            case token_is_false_bool:
            case token_is_true_bool:
            case token_is_semicolon:
            case token_is_expr_init:
            case token_is_clone_up:
            case token_is_mul_sign:
            case token_is_div_sign:
            case token_is_add_sign:
            case token_is_lhs_par:
            case token_is_rhs_par:
                printf("(%d, %d, %d, %d): LITERAL token: %c\n", numline, offset, row, col, chr);
                break;

            case token_is_sub_sign:
                printf("(%d, %d, %d, %d): LITERAL token: %c\n", numline, offset, row, col, '-');
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                printf("(%d, %d, %d, %d): NUMBER token: %c\n", numline, offset, row, col, chr);
                break;
            
            case token_is_string:
                printf("(%d, %d, %d, %d): NUMBER token: %c\n", numline, offset, row, col, chr);
                break;
        }
    }
}

static void get_table_size (char *src, unsigned int *rows, unsigned *cols, const char sep)
{
    unsigned int c = 0;

    while (*src)
    {
        const char this = *src++;
        if (this == '\n')
        {
            *rows += 1;
            *cols = *cols > c ? *cols : c;
            c = 0;
        }
        else if (this == sep)
        {
            c++;
        }
    }
    *cols = *cols > c ? *cols : c;

    if (*cols == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there are 0 columns, do you really want '%c' to be the separator?\n", sep);
        exit(EXIT_FAILURE);
    }

    if (*rows == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there is zero rows, is there any content in the table?\n");
        exit(EXIT_FAILURE);
    }
}