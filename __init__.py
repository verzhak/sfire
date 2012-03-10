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

import plugin, sfire

def name():
	
	return "SFire"

def description():
	
	return "Mapping of burnt forest areas"

def version():
	
	return "1.0"

def qgisMinimumVersion(): 
	
	return "1.8"

def authorName():

	return "Akinin M.V. (verzhak@gmail.com)"

def classFactory(iface):
	
	return plugin.CSFirePlugin(iface)

