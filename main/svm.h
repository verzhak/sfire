
// Машина опорных векторов (SVM; Support Vector Machine)

#ifndef SVM_H
#define SVM_H

#include "all.h"
#include "exception.h"
#include "memory.h"
#include "print.h"

// ############################################################################

typedef struct t_s_svm
{

	unsigned dim, num;
	double sigma, b, * w, ** s;

} s_svm;

// ############################################################################ 

double kernel_radial(const double * v1, const double * v2, unsigned dim, double sigma);

// ############################################################################ 

s_svm * svm_init(unsigned dim, double sigma);
void svm_destroy(s_svm * svm);

s_svm * svm_load(const char * fname);
int svm_save(const char * fname, s_svm * svm);

double svm_run(s_svm * svm, const double * vec);
unsigned svm_test(s_svm * svm, double ** vec, char * d, unsigned vec_num);

// Обучение SVM по алгоритму последовательной минимальной оптимизации с применением эвристики Fan'а, Chen'а и Lin'я
int svm_teach_smo(s_svm * svm, double ** vec, char * d, unsigned vec_num, double C, double tau, unsigned max_iter, double epsilon);

// ############################################################################

#endif

