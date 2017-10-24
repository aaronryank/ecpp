#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *_typenames[] = {"undefined","keyword","literal","oparen","cparen","obracket","cbracket","obrace","cbrace","comma","semicolon","operator","reserved","control"};

void print_list(token_t *head)
{
    token_t *current = head;

    while (current) {
        printf("%3d: %10s: %s\n",current->line,_typenames[current->type],current->val);
        current = current->next;
    }
}

void printline(int line, token_t *head)
{
    token_t *current = head;
    int count = 0;

    while (current) {
        if (line == current->line) {
            printf("%s ",current->val);
            count++;
        }
        else if (line < current->line) {
            break;
        }

        current = current->next;
    }

    if (count)
        putchar('\n');
}

void rewind_token(token_t **current)
{
    if (!(*current))
        return;

    while ((*current)->prev)
        *current = (*current)->prev;
}

void push(token_t **current, char *val, int type, int line)
{
    //printf("%10s: %s\n",_typenames[type],val);

    if (!(*current)) {
        (*current) = malloc(sizeof(token_t));
        (*current)->val = strdup(val);
        (*current)->type = type;
        (*current)->line = line;
        (*current)->next = NULL;
        (*current)->prev = NULL;
        return;
    }

    while ((*current)->next) {
        *current = (*current)->next;
    }

    (*current)->next = malloc(sizeof(token_t));
    (*current)->next->val = strdup(val);
    (*current)->next->type = type;
    (*current)->next->line = line;
    (*current)->next->next = NULL;
    (*current)->next->prev = (*current);

    rewind_token(current);
}

void add_at_current(token_t **current, char *val, int type, int line)
{
    if (!(*current)) {
        (*current) = malloc(sizeof(token_t));
        (*current)->val = strdup(val);
        (*current)->type = type;
        (*current)->line = line;
        (*current)->next = NULL;
        (*current)->prev = NULL;
        return;
    }

    token_t *newnode = malloc(sizeof(token_t));

    newnode->val = strdup(val);
    newnode->type = type;
    newnode->line = line;
    newnode->prev = (*current)->prev;
    (*current)->prev = newnode;
    newnode->prev->next = newnode;
    newnode->next = (*current);
}

token_struct *pop(token_t **head)
{
    token_struct *retval = malloc(sizeof(token_struct));
    token_t *next_node = NULL, *prev_node = NULL;

    if (*head == NULL) {
        return NULL;
    }

    retval->type = (*head)->type;
    retval->line = (*head)->line;
    retval->val  = strdup((*head)->val);

    prev_node = (*head)->prev;
    next_node = (*head)->next;
    free(*head);
    (*head) = next_node;
    (*head)->prev = prev_node;

    if (prev_node)
        (*head)->prev->next = (*head);

    return retval;
}

token_struct *remove_last(token_t *head)
{
    token_struct *retval = malloc(sizeof(token_struct));

    /* if there is only one item in the list, remove it */
    if (head->next == NULL) {
        retval->type = head->type;
        retval->line = head->line;
        retval->val  = strdup(head->val);
        free(head);
        return retval;
    }

    /* get to the last node in the list */
    token_t *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    /* now current points to the last item of the list, so let's remove current->next */
    retval->val = strdup(current->next->val);
    retval->type = current->next->type;
    retval->line = current->next->line;
    free(current->next);
    current->next = NULL;
    return retval;
}

token_struct *remove_by_index(token_t **head, int n)
{
    int j;
    token_struct *retval = malloc(sizeof(token_struct));
    token_t *current = *head;
    token_t *temp_node = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (j = 0; j < n-1; j++) {
        if (current->next == NULL) {
            return NULL;
        }
        current = current->next;
    }

    temp_node = current->next;
    retval->val = strdup(temp_node->val);
    retval->type = temp_node->type;
    retval->line = temp_node->line;
    current->next = temp_node->next;
    free(temp_node);

    return retval;
}

token_struct *return_index(token_t *head, int n)
{
    token_struct *retval = malloc(sizeof(token_struct));
    token_t *current = head;

    if (n == 0) {
        retval->type = head->type;
        retval->val = strdup(head->val);
        retval->line = head->line;
        return retval;
    }

    while (n--) {
        if (current->next == NULL) {
            return NULL;
        }
        current = current->next;
    }

    retval->val = strdup(current->val);
    retval->type = current->type;
    retval->line = current->line;
    return retval;
}
