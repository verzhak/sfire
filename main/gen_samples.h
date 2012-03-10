
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef GEN_SAMPLES_H
#define GEN_SAMPLES_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"
#include "various.h"

int gen_samples(char * sat_first_dname, char * sat_second_dname, char * shp_aoi_fname, char * shp_forest_fname, char ** out_fname);

#endif

