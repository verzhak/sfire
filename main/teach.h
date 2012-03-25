
// Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

#ifndef TEACH_H
#define TEACH_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"
#include "svm.h"

int teach(char ** in_fname, char ** out_fname, unsigned * vec_per_class, double * sigma, double * C, double * tau, unsigned * max_iter, double * epsilon);

#endif

