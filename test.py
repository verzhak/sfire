#!/usr/bin/python2
#coding=utf-8

# Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

from sys import argv

import sfire

############################################################################
# Обработка аргументов командной строки

out_prefix = "/home/amv/RGRTU/evm/ns/fires/test/"

if len(argv) == 3:

	# Работа
	data_prefix = "/home/amv/trash/sda4/data/satellite/Landsat/TM/Мещера/"

else:

	# Дом
	data_prefix = "/home/amv/RGRTU/evm/data/satellite/Landsat/TM/Мещера/"

############################################################################

if argv[1] == "1":

	# Формирование обучающей выборки векторов (обучающего набора пикселей)

	(child, queue) = sfire.gen_samples(sat_first_dname = data_prefix + "/LT51760222007167MOR00/", sat_second_dname = data_prefix + "/LT51760222011210MOR00/",
						shp_aoi_fname = out_prefix + "/aoi/aoi.shp", shp_forest_fname = out_prefix + "/forest/forest.shp",
						out_fname = [ out_prefix + "/teach_0.bin", out_prefix + "/teach_1.bin", out_prefix + "/teach_2.bin" ])

elif argv[1] == "2":

	# Обучение машин опорных векторов

	(child, queue) = sfire.teach(in_fname = [ out_prefix + "/teach_0.bin", out_prefix + "/teach_1.bin", out_prefix + "/teach_2.bin" ],
						out_fname = [ out_prefix + "/svm_012.svm", out_prefix + "/svm_01.svm" ])

elif argv[1] == "3":

	# Обработка спутниковых снимков

	sat = "LT51760222006244MOR00" # 01.09.2006 - LandSat 5
#	sat = "LT51760222007167MOR00" # 16.06.2007 - LandSat 5
#	sat = "LT51760222010175MOR00" # 24.06.2010 - LandSat 5
#	sat = "LT51760222011210MOR00" # 29.07.2011 - LandSat 5

	sat_dname = data_prefix + "/" + sat + "/"
	result_fname = out_prefix + "/" + sat + ".tif"
		
	(child, queue) = sfire.process(sat_dname, result_fname, shp_aoi_fname = out_prefix + "/aoi/aoi.shp", shp_forest_fname = out_prefix + "/forest/forest.shp",
			svm_fname = [ out_prefix + "/svm_012.svm", out_prefix + "/svm_01.svm" ])

	print("---------------------->", sat_dname)

else:

	raise Exception("Неизвестный первый аргумент")

while child.is_alive():

	try:

		msg = queue.get(timeout = 1)
		print(msg)

	except:

		pass

