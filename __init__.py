#coding=utf-8

import plugin, sfire

def name():
	
	return "SFire"

def description():
	
	return "Mapping of burnt forest areas"

def version():
	
	return "1.0"

def qgisMinimumVersion(): 
	
	return "1.7"

def authorName():

	return "Akinin M.V. (verzhak@gmail.com)"

def classFactory(iface):
	
	return plugin.CSFirePlugin(iface)

