#coding=utf-8

#
# Составление карт гарей лесных массивов
#
# Акинин М.В. (verzhak@gmail.com), студент группы 740М, кафедра ЭВМ, РГРТУ (Рязань, Россия)
#
# Научный руководитель: доцент, к.т.н., Конкин Ю.В.
#
# 12 февраля 2012-го года
#

from multiprocessing import Process, Queue

import wrap_sfire

my_print = None

def set(fun, lst):

	tlst = fun(len(lst))

	for u in range(0, len(lst)):

		tlst[u] = lst[u]

	return tlst

def decorator(t_fun):

	def fun(* arg, ** karg):

		global my_print

		queue = Queue()

		def t_my_print(msg):

			queue.put(msg)

		my_print = t_my_print
		wrap_sfire.set_print_fun(t_my_print)

		return (t_fun(* arg, ** karg), queue)

	return fun

@decorator
def gen_samples(sat_first_dname, sat_second_dname, shp_aoi_fname, shp_forest_fname, out_fname):

	t_out_fname = list(map(lambda x: x.encode("ascii"), out_fname))

	child = Process(target = wrap_sfire.gen_samples, args = (sat_first_dname, sat_second_dname, shp_aoi_fname, shp_forest_fname, t_out_fname))
	child.start()

	return child

@decorator
def teach(in_fname, out_fname, vec_per_class = [ 2000, 2000 ], C = [ 512, 5.998047 ], sigma = [ 128, 7.013672 ],
		tau = [ 1E-12, 1E-12 ], max_iter = [ 100000, 100000 ], epsilon = [ 1E-3, 1E-3 ]):

	t_in_fname = list(map(lambda x: x.encode("ascii"), in_fname))
	t_out_fname = list(map(lambda x: x.encode("ascii"), out_fname))
	t_vec_per_class = set(wrap_sfire.unsigned_array, vec_per_class)
	t_C = set(wrap_sfire.double_array, C)
	t_sigma = set(wrap_sfire.double_array, sigma)
	t_tau = set(wrap_sfire.double_array, tau)
	t_max_iter = set(wrap_sfire.unsigned_array, max_iter)
	t_epsilon = set(wrap_sfire.double_array, epsilon)

	child = Process(target = wrap_sfire.teach, args = (t_in_fname, t_out_fname, t_vec_per_class, t_sigma, t_C, t_tau, t_max_iter, t_epsilon))
	child.start()

	return child

@decorator
def process(sat_dname, result_fname, shp_aoi_fname, shp_forest_fname, svm_fname):

	t_svm_fname = list(map(lambda x: x.encode("ascii"), svm_fname))

	child = Process(target = wrap_sfire.process, args = (sat_dname, result_fname, shp_aoi_fname, shp_forest_fname, t_svm_fname))
	child.start()

	return child

