
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#ifndef ALL_H
#define ALL_H

#ifndef SHARED

	#include <Python.h>

#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <dirent.h>
#include <gdal.h>
#include <gdal_alg.h>
#include <ogr_api.h>
#include <ogr_srs_api.h>
#include <cpl_string.h>

#define DIM 7
#define BUF_SIZE 4096

#endif

