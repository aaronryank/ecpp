#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "extern.h"

const char *typenames[] = {"undefined","keyword","literal","oparen","cparen","obracket","cbracket","obrace","cbrace","comma","semicolon","operator","reserved","control flow"};

const char *DEFAULT_RESERVED_KEYWORDS[] = {
  "auto",
  "char",
  "const",
  "double",
  "enum",
  "extern",
  "float",
  "int",
  "long",
  "register",
  "short",
  "signed",
  "static",
  "struct",
  "typedef",
  "union",
  "unsigned",
  "void",
  "volatile",
  "fortran",
  "asm"
};

int reserved_keyword_count = 21;

const char *DEFAULT_CONTROL_FLOW_KEYWORDS[] = {
  "break",
  "case",
  "continue",
  "default",
  "do",
  "else",
  "for",
  "goto",
  "if",
  "return",
  "switch",
  "while"
};

int control_flow_keyword_count = 12;

char **RESERVED_KEYWORDS;
char **CONTROL_FLOW_KEYWORDS;

void setup_rules(void)
{
    RESERVED_KEYWORDS = malloc(sizeof(char **)*100);
    CONTROL_FLOW_KEYWORDS = malloc(sizeof(char **)*100);
    int i;

    for (i = 0; i < reserved_keyword_count; i++)
        RESERVED_KEYWORDS[i] = strdup(DEFAULT_RESERVED_KEYWORDS[i]);

    for (i = 0; i < control_flow_keyword_count; i++)
        CONTROL_FLOW_KEYWORDS[i] = strdup(DEFAULT_CONTROL_FLOW_KEYWORDS[i]);
}

void modify_ecpp_rules(const char *rule, const char *modification)
{
    if (!strcmp(rule,"control_flow")) {
        add_control_flow_keyword(modification);
    }
    else if (!strcmp(rule,"reserved")) {
        add_reserved_keyword(modification);
    }
}

void add_control_flow_keyword(const char *newword)
{
    CONTROL_FLOW_KEYWORDS[control_flow_keyword_count++] = strdup(newword);
}

void add_reserved_keyword(const char *newword)
{
    RESERVED_KEYWORDS[reserved_keyword_count++] = strdup(newword);
}
