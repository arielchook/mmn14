#ifndef DIRECTIVES_H
#define DIRECTIVESH

bool handle_define(char *symbolStmt, int lineNumber);
bool handle_data(char *dataStmt, int lineNumber);
bool handle_string(char *stringStmt, int lineNumber);
/* . entry should be handled in 2nd pass
bool handle_entry(char *entryStmt, int lineNumber);*/
bool handle_extern(char *externStmt, int lineNumber);

#endif