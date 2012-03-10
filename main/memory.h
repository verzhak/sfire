
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "all.h"
#include "exception.h"

void * sfire_alloc(unsigned type_size, unsigned dim, ...);
void sfire_free(void * mem);

#endif

