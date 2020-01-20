#include "ush.h"

void printTokens(t_list *tokens) {
    t_list *tmp = tokens;
    char i = 0;
    while(tmp) {
        i = 0;
        printf("TOKEN:\n");
        while (((t_token*)tmp->data)->value[i]) {
        printf("value: %s type: %d prior: %d\n",((t_token*)tmp->data)->value[i], ((t_token*)tmp->data)->type, ((t_token*)tmp->data)->priority);
        i++;
        }
        tmp = tmp->next;
    }
    if (tokens)
        printf("\n");
}

/* execute all commands ony by one*/
void mx_execute(char **commands) {
    t_list  *tokens  = NULL;
    t_tnode *rootAst = 0;
    int     i        = 0;

    while(commands[i]) {
        tokens = mx_lexer(commands[i]);
        // if (mx_syntax_analyzer(tokens)) {
        //     // rootAst = mx_create_ast(tokens);
        //     // printf("O : %s\n", ((t_token*)rootAst->right->data)->value);
        // }
        printTokens(tokens);
        mx_clear_tokens(&tokens);
        i++;
    }
}