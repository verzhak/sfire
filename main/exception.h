
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "print.h"

#define try __label__ catch_label, finally_label;
#define catch goto finally_label; catch_label:
#define finally finally_label:
#define to_finally goto finally_label;

#define throw(condition) \
if((condition))\
{\
	my_printf("[Exception] File %s, line %d", __FILE__, __LINE__);\
	goto catch_label;\
};

#define throw_null(pointer) throw((pointer) == NULL)

#endif

