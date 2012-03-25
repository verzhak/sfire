
// Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

#include "print.h"

t_print_fun my_print_fun = & simple_print;

void real_set_print_fun(t_print_fun fun)
{
	my_print_fun = fun;
}

void simple_print(char * str)
{
	printf("%s", str);
}

void my_printf(const char * format, ...)
{
	char buf[BUF_SIZE];
	va_list arg;

	va_start(arg, format);

	vsnprintf(buf, BUF_SIZE - 1, format, arg);
	(* my_print_fun)(buf);

	va_end(arg);
}

