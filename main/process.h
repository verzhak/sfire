
#ifndef PROCESS_H
#define PROCESS_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"
#include "svm.h"
#include "various.h"

int process(char * sat_dname, char * result_fname, char * shp_aoi_fname, char * shp_forest_fname, char ** svm_fname);

#endif

