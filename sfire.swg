
%module wrap_sfire

%include "carrays.i"

%array_class(double, double_array);
%array_class(unsigned, unsigned_array);

%typemap(in) char **
{
	int u, size;

	size = PyList_Size($input);
	$1 = (char **) malloc(size * sizeof(char *));

	for(u = 0; u < size; u++)
		$1[u] = PyString_AsString(PyList_GetItem($input, u));
}

%typemap(freearg) char **
{
	free((void *) $1);
}

%{

	extern int gen_samples(char * sat_first_dname, char * sat_second_dname, char * shp_aoi_fname, char * shp_forest_fname, char ** out_fname);
	extern int teach(char ** in_fname, char ** out_fname, unsigned * vec_per_class, double * t_sigma, double * C,
		double * tau, unsigned * max_iter, double * epsilon);
	extern int process(char * sat_dname, char * result_fname, char * shp_aoi_fname, char * shp_forest_fname, char ** svm_fname);

%}

extern int gen_samples(char * sat_first_dname, char * sat_second_dname, char * shp_aoi_fname, char * shp_forest_fname, char ** out_fname);
extern int teach(char ** in_fname, char ** out_fname, unsigned * vec_per_class, double * t_sigma, double * C,
	double * tau, unsigned * max_iter, double * epsilon);
extern int process(char * sat_dname, char * result_fname, char * shp_aoi_fname, char * shp_forest_fname, char ** svm_fname);

// ############################################################################ 

// Печать через callback
// Код на C вызывает функцию my_print_fun(tstr) из кода на Python 2

%{

	typedef void (* t_print_fun)(char *);

	extern t_print_fun my_print_fun;

	extern void real_set_print_fun(t_print_fun fun);
	extern void set_print_fun(PyObject * fun);

%}

extern t_print_fun my_print_fun;

extern void real_set_print_fun(t_print_fun fun);
extern void set_print_fun(PyObject * fun);

%{

	static PyObject * py_print_fun = NULL;

	static void run_print_fun(char * str)
	{
		PyObject * fun = py_print_fun, * arglist = Py_BuildValue("(s)", str);
		PyObject * result = PyEval_CallObject(fun, arglist);

		Py_DECREF(arglist);
		Py_XDECREF(result);
	}

	void set_print_fun(PyObject * fun)
	{
		Py_XDECREF(py_print_fun);
		Py_XINCREF(fun);

		py_print_fun = fun;
		real_set_print_fun(& run_print_fun);
	}

%}

