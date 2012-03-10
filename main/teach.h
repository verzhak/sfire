
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef TEACH_H
#define TEACH_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"
#include "svm.h"

int teach(char ** in_fname, char ** out_fname, unsigned * vec_per_class, double * sigma, double * C, double * tau, unsigned * max_iter, double * epsilon);

#endif

