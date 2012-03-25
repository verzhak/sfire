
// Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

#ifndef PRINT_H
#define PRINT_H

#include "all.h"

typedef void (* t_print_fun)(char *);

extern t_print_fun my_print_fun;

void real_set_print_fun(t_print_fun fun);
void simple_print(char * str);
void my_printf(const char * format, ...);

#endif

