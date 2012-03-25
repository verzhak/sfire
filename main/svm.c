
// Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

#include "svm.h"

double kernel(const double * v1, const double * v2, unsigned dim, double sigma)
{
	// Радиальная базисная функция в качестве ядра скалярного произведения

	unsigned u;
	double d, sum = 0;

	for(u = 0; u < dim; u++)
	{
		d = v1[u] - v2[u];
		sum += d * d;
	}

	return exp(- sum / (2 * sigma * sigma));
}

// ############################################################################ 

s_svm * svm_init(unsigned dim, double sigma)
{
	try;

		s_svm * svm = NULL;
		
		throw_null((svm = sfire_alloc(sizeof(svm), 1, 1)));
		// throw_null((svm = malloc(sizeof(svm))));

		svm->dim = dim;
		svm->sigma = sigma;
		svm->w = NULL;
		svm->s = NULL;

	catch;

		svm_destroy(svm);

		svm = NULL;

	finally;

	return svm;
}

void svm_destroy(s_svm * svm)
{
	if(svm != NULL)
	{
		sfire_free(svm->w);
		sfire_free(svm->s);
		sfire_free(svm);
		// free(svm);
	}
}

s_svm * svm_load(const char * fname)
{
	try;

		s_svm * svm = NULL;
		unsigned v, u, dim, num, notused;
		double sigma, * w = NULL, ** s = NULL;
		FILE * fl = NULL;

		throw_null((fl = fopen(fname, "r")));

		fscanf(fl, "sigma = %lf\ndim = %u\n", & sigma, & dim);

		throw_null((svm = svm_init(dim, sigma)));

		fscanf(fl, "num = %u\nb = %lf\n\n", & num, & svm->b);

		svm->num = num;

		throw_null((svm->w = sfire_alloc(sizeof(double), 1, num)));
		throw_null((svm->s = (double **) sfire_alloc(sizeof(double), 2, num, dim)));

		w = svm->w;
		s = svm->s;

		for(u = 0; u < num; u++)
		{
			fscanf(fl, "%u\n\n\tw = %lf\n\n", & notused, & w[u]);

			for(v = 0; v < dim; v++)
				fscanf(fl, "\ts[%u] = %lf\n", & notused, & s[u][v]);

			fscanf(fl, "\n");
		}

	catch;

		svm_destroy(svm);

		svm = NULL;

	finally;

		if(fl != NULL)
			fclose(fl);

	return svm;
}

int svm_save(const char * fname, s_svm * svm)
{
	try;

		int ret = 0;
		unsigned v, u, dim = svm->dim, num = svm->num;
		double * w = svm->w, ** s = svm->s;
		FILE * fl = NULL;

		throw_null((fl = fopen(fname, "w")));

		fprintf(fl, "sigma = %.20lf\n", svm->sigma);
		fprintf(fl, "dim = %u\n", dim);
		fprintf(fl, "num = %u\n", num);
		fprintf(fl, "b = %.20lf\n\n", svm->b);

		for(u = 0; u < num; u++)
		{
			fprintf(fl, "%u\n\n\tw = %.20lf\n\n", u, w[u]);

			for(v = 0; v < dim; v++)
				fprintf(fl, "\ts[%u] = %.20lf\n", v, s[u][v]);

			fprintf(fl, "\n");
		}

	catch;

		ret = -1;

	finally;

		if(fl != NULL)
			fclose(fl);

	return ret;
}

double svm_run(s_svm * svm, const double * vec)
{
	unsigned u, dim = svm->dim, num = svm->num;
	double sigma = svm->sigma, sum = - svm->b, * w = svm->w, ** s = svm->s;

	for(u = 0; u < num; u++)
		sum += w[u] * kernel(s[u], vec, dim, sigma);

	if(sum > 1 - 1E-10)
		sum = 1;
	else if(sum < -1 + 1E-10)
		sum = -1;
	else if(fabs(sum) < 1E-10)
		sum = 0;

	return sum;
}

unsigned svm_test(s_svm * svm, double ** vec, char * d, unsigned vec_num)
{
	double res;
	unsigned u, yes = 0;

	for(u = 0; u < vec_num; u++)
	{
		res = svm_run(svm, vec[u]);
		
		if(
			(res > 0 && d[u] == 1)
			||
			(res < 0 && d[u] == -1)
		  )
			yes++;
	}

	return yes;
}

int svm_teach_smo(s_svm * svm, double ** vec, char * d, unsigned vec_num, double C, double tau, unsigned max_iter, double epsilon)
{
	try;

		int i, j, ret = 0;
		unsigned u, v, iter, num, dim = svm->dim;
		double a, b, max, min, sum, sigma = svm->sigma, prev_lambda[2], * w, ** s, * lambda = NULL, * grad = NULL, ** Q = NULL;

		throw((tau <= 0));
		throw((epsilon <= 0));
		throw_null((lambda = sfire_alloc(sizeof(double), 1, vec_num)));
		throw_null((grad = sfire_alloc(sizeof(double), 1, vec_num)));
		throw_null((Q = (double **) sfire_alloc(sizeof(double), 2, vec_num, vec_num)));

		// ############################################################################ 

		for(u = 0; u < vec_num; u++)
			lambda[u] = 0;

		for(u = 0; u < vec_num; u++)
		{
			grad[u] = -1;

			for(v = 0; v < u; v++)
				grad[u] += lambda[v] * Q[u][v];

			Q[u][u] = d[u] * d[u] * kernel(vec[u], vec[u], dim, sigma);
			grad[u] += lambda[u] * Q[u][u];

			for(v = u + 1; v < vec_num; v++)
			{
				Q[v][u] = Q[u][v] = d[v] * d[u] * kernel(vec[v], vec[u], dim, sigma);
				grad[u] += lambda[v] * Q[u][v];
			}
		}

		// ############################################################################ 

		for(iter = 0; iter < max_iter; iter++)
		{
			// ############################################################################ 
			// Выбор

			i = -1;

			for(u = 0; u < vec_num; u++)
				if(
					(
						(lambda[u] < C && d[u] == 1)
						||
						(lambda[u] > 0 && d[u] == -1)
					)
					&&
					(
						i == -1
						||
						- d[u] * grad[u] > max
					)
				  )
				{
					i = u;
					max = - d[u] * grad[u];
				}

			if(i == -1)
				break;

			j = -1;

			for(u = 0; u < vec_num; u++)
				if(u != i)
				{
					if((a = Q[u][u] + Q[i][i] - 2 * Q[u][i] * d[u] * d[i]) <= 0)
						a = tau;

					b = max + d[u] * grad[u];

					if(
						(
							(lambda[u] < C && d[u] == -1)
							||
							(lambda[u] > 0 && d[u] == 1)
						)
						&&
						(
							j == -1
							||
							- b * b / a < min
						)
						&&
						max > - d[u] * grad[u]
					  )
					{
						j = u;
						min = - b * b / a;
					}
				}

			if(j == -1 || min >= max || max - min < epsilon)
				break;

			// ############################################################################ 
			// Оптимизация

			prev_lambda[0] = lambda[i];
			prev_lambda[1] = lambda[j];

			if((a = Q[i][i] + Q[j][j] - 2 * Q[i][j] * d[i] * d[j]) <= 0)
				a = tau;

			b = - d[i] * grad[i] + d[j] * grad[j];

			sum = d[i] * lambda[i] + d[j] * lambda[j];
			lambda[i] += d[i] * b / a;
			lambda[j] -= d[j] * b / a;

			if(lambda[i] > C)
				lambda[i] = C;
			else if(lambda[i] < 0)
				lambda[i] = 0;

			lambda[j] = d[j] * (sum - d[i] * lambda[i]);

			if(lambda[j] > C)
				lambda[j] = C;
			else if(lambda[j] < 0)
				lambda[j] = 0;

			lambda[i] = d[i] * (sum - d[j] * lambda[j]);

			// ############################################################################ 
			// Пересчет градиента

			for(u = 0; u < vec_num; u++)
				grad[u] += (lambda[i] - prev_lambda[0]) * Q[u][i] + (lambda[j] - prev_lambda[1]) * Q[u][j];

			// ############################################################################ 

			if(! ((iter + 1) % 500))
				my_printf("Iteration %u from %u ( %lf %% )", iter + 1, max_iter, (100.0 * (iter + 1.0)) / max_iter);
		}

		// ############################################################################ 
		// Расчет w, b и s
		
		num = 0;

		for(u = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
				num++;

		svm->num = num;

		throw_null((w = svm->w = sfire_alloc(sizeof(double), 1, num)));
		throw_null((s = svm->s = (double **) sfire_alloc(sizeof(double), 2, num, dim)));

		for(u = 0, v = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
			{
				w[v] = lambda[u] * d[u];
				memcpy(s[v], vec[u], dim * sizeof(double));

				v++;
			}

		b = 0;

		for(u = 0; u < vec_num; u++)
			if(lambda[u] > 1E-10)
			{
				for(v = 0; v < num; v++)
					b += w[v] * kernel(s[v], vec[u], dim, sigma);

				b -= d[u];
			}
		
		svm->b = b / num;
		
		// ############################################################################ 
		
	catch;

		ret = -1;

	finally;

		sfire_free(lambda);
		sfire_free(grad);
		sfire_free(Q);

	return ret;
}

