
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

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

