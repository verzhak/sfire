
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef PRINT_H
#define PRINT_H

#include "all.h"

typedef void (* t_print_fun)(char *);

extern t_print_fun my_print_fun;

void real_set_print_fun(t_print_fun fun);
void simple_print(char * str);
void my_printf(const char * format, ...);

#endif

