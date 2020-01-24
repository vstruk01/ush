#include "ush.h"
// SIGFAULT
static t_list* find_max_priority(t_list* tokens) {
    t_list *tmp = tokens;
    t_list* maxNode = tmp;

    if (tmp == 0)
        return 0;
    int maxPriority = ((t_token*)tmp->data)->priority;
    while (tmp) {
        if (((t_token*)tmp->data)->priority > maxPriority) {
            maxPriority = ((t_token*)tmp->data)->priority;
            maxNode = tmp;
        }
        tmp = tmp->next;
    }
    if (maxNode == 0)
        return 0;
    
    return maxNode;
}

t_list* del_token(t_list **tokens, t_list *max) {
    t_list *tmp = *tokens;
    t_list *next = 0;

    if (*tokens == max) {
        next = (*tokens)->next;
        free(*tokens);
        *tokens = 0;
    }
    while (tmp->next) {
        if (strcmp(((t_token*)tmp->next->data)->value[0], 
                    ((t_token*)max->data)->value[0]) == 0) {
            next = tmp->next->next;
            free(tmp->next);
            tmp->next = 0;
            break;
        }
        tmp = tmp->next;
    }
    return next;
}

t_tnode* mx_create_ast(t_list** tokens, t_tnode *prev) {
    t_list *max = find_max_priority(*tokens);
    t_list *tmp = *tokens;
    t_tnode *root = mx_create_tnode(max->data);
    t_list *next = del_token(tokens, max);

    root->parent = prev;
    
    while (tmp) {
        // printf("TOK = %s\n", ((t_token*)tmp->data)->value[0]);
        tmp = tmp->next;
    }

    tmp = next;
    while (tmp) {
        // printf("NEXT = %s\n", ((t_token*)tmp->data)->value[0]);
        tmp = tmp->next;
    }

    if (next != 0) {
        //  printf("right ");
        root->right = mx_create_ast(&next, root);
    }
    if (*tokens != 0) {
        // printf("left ");
        root->left = mx_create_ast(tokens, root);
    }
    return root;
}
