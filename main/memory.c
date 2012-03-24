
#include "memory.h"

void * alloc_pointer[BUF_SIZE];
size_t alloc_size[BUF_SIZE];
unsigned alloc_num = 0;

void * sfire_alloc(unsigned type_size, unsigned dim, ...)
{
	try;

		void * pof, * dof, * t_dof, * ret = NULL;
		unsigned u, v, num, step, size, dim_1 = dim - 1, dim_2 = dim - 2, dim_size[dim];
		va_list val;

		throw((alloc_num == BUF_SIZE));

		va_start(val, dim);

		for(u = 0, size = 0, num = 1; u < dim; u++)
		{
			num *= (dim_size[u] = va_arg(val, unsigned));
			size += (u == dim_1 ? type_size : sizeof(void *)) * num;
		}

		va_end(val);

		// Python'овский malloc() (malloc() через Python.h), по всей видимости, не поддерживает glibc'овский аллокатор - куча иногда переполняется
		throw_null((ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)));

		alloc_pointer[alloc_num] = ret;
		alloc_size[alloc_num] = size;
		alloc_num++;

		for(u = 0, num = 1, dof = ret; u < dim_1; u++)
		{
			num *= dim_size[u];
			pof = dof;
			t_dof = (dof += num * sizeof(void *));
			step = (u == dim_2 ? type_size : sizeof(void *)) * dim_size[u + 1];

			for(v = 0; v < num; v++, pof += sizeof(void *), t_dof += step)
				memcpy(pof, & t_dof, sizeof(void *));
		}

	catch;

		sfire_free(ret);
		ret = NULL;

	finally;

	return ret;
}

void sfire_free(void * mem)
{
	if(mem != NULL && alloc_num)
	{
		unsigned u;

		for(u = 0; u < alloc_num; u++)
			if(alloc_pointer[u] == mem)
			{
				munmap(alloc_pointer[u], alloc_size[u]);

				alloc_pointer[u] = alloc_pointer[alloc_num - 1];
				alloc_size[u] = alloc_size[alloc_num - 1];
				alloc_num--;
			}
	}
}

