#ifndef _ECPP_TYPES_H
#define _ECPP_TYPES_H

#include "string.h"
#include <ctype.h>

enum {
  TYPE_UNDEF,       /* undefined                                           */
  TYPE_KEYWORD,     /* any run of a-Z, A-Z, 0-9, and _                     */
  TYPE_LITERAL,     /* any string, character, or integer literal           */
  TYPE_OPAREN,      /* (                                                   */
  TYPE_CPAREN,      /* )                                                   */
  TYPE_OBRACKET,    /* [                                                   */
  TYPE_CBRACKET,    /* ]                                                   */
  TYPE_OBRACE,      /* {                                                   */
  TYPE_CBRACE,      /* }                                                   */
  TYPE_COMMA,       /* ,                                                   */
  TYPE_SEMICOLON,   /* ;                                                   */
  TYPE_OPERATOR,    /* anything containing any of `~!@$%^&*()_-+=|:<>?     */
  TYPE_RESERVED,    /* anything in RESERVED_KEYWORDS                       */
  TYPE_CONTROL_FLOW /* anything in CONTROL_FLOW_KEYWORDS                   */
} token_types;

extern const char *typenames[];
extern const char *DEFAULT_RESERVED_KEYWORDS[];
extern const char *DEFAULT_CONTROL_FLOW_KEYWORDS[];
extern char **RESERVED_KEYWORDS;
extern char **CONTROL_FLOW_KEYWORDS;
extern int reserved_keyword_count;
extern int control_flow_keyword_count;

#define isnumliteral(s) (str_only_contains(s,"012345678L.") && isdigit(s[0]))
#define ischrliteral(s) (strlen(s) > 1 && s[strlen(s)-1] == '\'' && s[strlen(s)-2] != '\\' && (s[0] == '\'' || s[0] == 'L'))
#define isstrliteral(s) (strlen(s) > 1 && s[strlen(s)-1] == '\"' && s[strlen(s)-2] != '\\' && (s[0] == '\"' || s[0] == 'L'))

#define isliteral(s)  (isnumliteral(s) || ischrliteral(s) || isstrliteral(s))
#define isoperator(s) (str_only_contains(s,"`~!@#$%^&*-+=|<>/?:."))
#define iskeyword(s)  (str_only_contains(token,LOWERCASE_ALPHA UPPERCASE_ALPHA "0123456789_"))   /* automatic concatenation is cool */

extern int gettype(const char *);

#endif /* ecpp's types.h */