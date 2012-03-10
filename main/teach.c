
/*
 * Составление карт гарей лесных массивов
 *
 * Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
 *
 * Научный руководитель: доцент, к.т.н., Конкин Ю.В.
 *
 * 12 февраля 2012-го года
 */

#include "teach.h"

int sfire_read(char ** fname, double *** vec, unsigned ** d, unsigned vec_per_class)
{
	try;

		int ret = 0;
		uint8_t buf[DIM];
		unsigned u, v, q, t, from, step, next, num, vec_per_class_3 = vec_per_class * 3;
		FILE * fl = NULL;

		* vec = NULL;
		* d = NULL;

		throw_null((* vec = sfire_alloc(sizeof(double), 2, vec_per_class_3, DIM)));
		throw_null((* d = sfire_alloc(sizeof(unsigned), 1, vec_per_class_3)));
	
		for(u = 0, q = 0; u < 3; u++)
		{
			my_printf("Read: %s", fname[u]);

			throw_null((fl = fopen(fname[u], "rb")));

			fseek(fl, 0, SEEK_END);
			num = ftell(fl) / DIM;
			fseek(fl, 0, SEEK_SET);

			for(v = from = 0, step = num / vec_per_class; v < vec_per_class; v++, q++, from += step)
			{
				next = from + lrand48() % step;

				fseek(fl, next * DIM, SEEK_SET);
				fread(buf, 1, DIM, fl);

				for(t = 0; t < DIM; t++)
					(* vec)[q][t] = buf[t];

				(* d)[q] = u;
			}

			fclose(fl);
			fl = NULL;
		}

	catch;

		if(fl != NULL)
			fclose(fl);

		if(* vec == NULL)
			sfire_free(* vec);

		if(* d == NULL)
			sfire_free(* d);

		ret = -1;

	finally;

	return ret;
}

int teach(char ** in_fname, char ** out_fname, unsigned * vec_per_class, double * sigma, double * C, double * tau, unsigned * max_iter, double * epsilon)
{
	try;

		int ret = 0;
		char * rd[2][2];
		double ** vec[2];
		unsigned u, v, vec_per_class_2, vec_per_class_3, yes, * d[2];
		s_svm * svm[2] = { NULL, NULL };

		my_printf("Begin");

		srand48(time(NULL));

		for(u = 0; u < 2; u++)
		{
			vec_per_class_3 = vec_per_class[u] * 3;
			vec_per_class_2 = vec_per_class[u] * 2;

			throw(sfire_read(in_fname, & vec[u], & d[u], vec_per_class[u]));
			throw_null((rd[u][0] = sfire_alloc(sizeof(char), 1, vec_per_class_3)));
			throw_null((rd[u][1] = sfire_alloc(sizeof(char), 1, vec_per_class_2)));

			for(v = 0; v < vec_per_class_2; v++)
			{
				if(d[u][v] == 2)
				{
					rd[u][0][v] = 1;
					rd[u][1][v] = -1;
				}
				else if(d[u][v] == 1)
				{
					rd[u][0][v] = -1;
					rd[u][1][v] = 1;
				}
				else
					rd[u][0][v] = rd[u][1][v] = -1;
			}

			for(; v < vec_per_class_3; v++)
			{
				if(d[u][v] == 2)
					rd[u][0][v] = 1;
				else
					rd[u][0][v] = -1;
			}
		}

		for(u = 0; u < 2; u++)
		{
			my_printf("SVM %u: begin", u + 1);

			svm[u] = svm_init(DIM, sigma[u]);
			throw(svm_teach_smo(svm[u], vec[0], rd[0][u], vec_per_class[0] * (3 - u), C[u], tau[u], max_iter[u], epsilon[u]));

			my_printf("SVM %u: end", u + 1);
		}

		for(v = 0; v < 2; v++)
		{
			yes = svm_test(svm[v], vec[1], rd[1][v], vec_per_class[1] * (3 - v));

			my_printf("SVM %u (testing): %u from %u ( %lf %% )", v + 1, yes, vec_per_class[1] * (3 - v), (yes * 100.0) / (vec_per_class[1] * (3 - v)));
		}

		for(u = 0; u < 2; u++)
			throw(svm_save(out_fname[u], svm[u]));

		my_printf("Success");

	catch;

		my_printf("Fail");

	finally;

		for(u = 0; u < 2; u++)
			svm_destroy(svm[u]);

		for(u = 0; u < 2; u++)
		{
			sfire_free(vec[u]);
			sfire_free(d[u]);
			sfire_free(rd[u][0]);
			sfire_free(rd[u][1]);
		}
	
	return ret;
}

