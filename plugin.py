#coding=utf-8

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *
import resources_rc, dialog_sfire, dialog_about

import sfire

class CSFireDialog(QDialog):

	def __init__(self, tfname):

		QDialog.__init__(self)

		translator = QTranslator()
		translator.load(tfname)
		QApplication.installTranslator(translator)

		self.__ui = dialog_sfire.Ui_SFireDialog()
		self.__ui.setupUi(self)
		self.__ui.retranslateUi(self)

		self.__widgets = [self.__ui.high_1_load, self.__ui.low_1_load, self.__ui.aoi_1_load, self.__ui.forest_1_load,
				self.__ui.c3_1_save, self.__ui.c2_1_save, self.__ui.c1_1_save, self.__ui.op_1, self.__ui.c3_2_load,
				self.__ui.c2_2_load, self.__ui.c1_2_load, self.__ui.svm_123_2_save, self.__ui.svm_12_2_save, self.__ui.op_2,
				self.__ui.prepare_3_load, self.__ui.aoi_3_load, self.__ui.forest_3_load, self.__ui.svm_123_3_load, self.__ui.svm_12_3_load,
				self.__ui.dst_3_save, self.__ui.op_3, self.__ui.is_load_3]

	def __message(self, msg):

		self.__ui.console.addItem(msg)

		if self.__ui.console.count() > 18:

			item = self.__ui.console.takeItem(0)
			self.__ui.console.removeItemWidget(item)

		self.__ui.console.repaint()
		self.repaint()

	def __wait(self, child, queue):

		msg = ""
		self.__ui.console.clear()

		while msg not in ["Success", "Fail"]:

			try:

				msg = queue.get(timeout = 0.001)
				self.__message(msg)

			except:

				self.__ui.console.repaint()
				self.repaint()

		return (msg == "Success")

	def high_1_load(self):
		
		self.__ui.high_1.setText(QFileDialog.getExistingDirectory(self, QApplication.translate("CSFireDialog", "Older satellite image"), "."))
		self.__op_enable(1)

	def low_1_load(self):

		self.__ui.low_1.setText(QFileDialog.getExistingDirectory(self, QApplication.translate("CSFireDialog", "Later satellite image"), "."))
		self.__op_enable(1)

	def aoi_1_load(self):
		
		self.__ui.aoi_1.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "AOI"), ".",
			QApplication.translate("CSFireDialog", "ESRI Shapefile (*.shp)")))
		self.__op_enable(1)

	def forest_1_load(self):

		self.__ui.forest_1.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "Forest map"), ".",
			QApplication.translate("CSFireDialog", "ESRI Shapefile (*.shp)")))
		self.__op_enable(1)

	def __examples_save_dialog(self, name, edit):

		dialog = QFileDialog(self, name, ".", QApplication.translate("CSFireDialog", "Examples file (*.bin)"))
		
		dialog.setAcceptMode(QFileDialog.AcceptSave)
		dialog.setDefaultSuffix("bin")

		if dialog.exec_():

			edit.setText(dialog.selectedFiles()[0])

		else:

			edit.setText("")

		self.__op_enable(1)

	def c3_1_save(self):

		self.__examples_save_dialog(QApplication.translate("CSFireDialog", "Forest class"), self.__ui.c3_1)

	def c2_1_save(self):

		self.__examples_save_dialog(QApplication.translate("CSFireDialog", "Smell of burning class"), self.__ui.c2_1)

	def c1_1_save(self):

		self.__examples_save_dialog(QApplication.translate("CSFireDialog", "Not forest, not smell of burning class"), self.__ui.c1_1)

	def c3_2_load(self):

		self.__ui.c3_2.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "Forest class"), ".",
			QApplication.translate("CSFireDialog", "Examples file (*.bin)")))
		self.__op_enable(2)

	def c2_2_load(self):

		self.__ui.c2_2.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "Smell of burning class"), ".",
			QApplication.translate("CSFireDialog", "Examples file (*.bin)")))
		self.__op_enable(2)

	def c1_2_load(self):

		self.__ui.c1_2.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "Not forest, not smell of burning class"), ".",
			QApplication.translate("CSFireDialog", "Examples file (*.bin)")))
		self.__op_enable(2)

	def __svm_save_dialog(self, name, edit):

		dialog = QFileDialog(self, name, ".", QApplication.translate("CSFireDialog", "SVM file (*.svm)"))
		
		dialog.setAcceptMode(QFileDialog.AcceptSave)
		dialog.setDefaultSuffix("svm")

		if dialog.exec_():

			edit.setText(dialog.selectedFiles()[0])

		else:

			edit.setText("")

		self.__op_enable(2)

	def svm_123_2_save(self):

		self.__svm_save_dialog(QApplication.translate("CSFireDialog", "SVM, that separates the forest class and other classes"), self.__ui.svm_123_2)

	def svm_12_2_save(self):

		self.__svm_save_dialog(QApplication.translate("CSFireDialog", "SVM, that separates the other classes"), self.__ui.svm_12_2)

	def prepare_3_load(self):

		self.__ui.prepare_3.setText(QFileDialog.getExistingDirectory(self, QApplication.translate("CSFireDialog", "Satellite image"), ".",
			QFileDialog.ShowDirsOnly))
		self.__op_enable(3)

	def aoi_3_load(self):

		self.__ui.aoi_3.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "AOI"), ".",
			QApplication.translate("CSFireDialog", "ESRI Shapefile (*.shp)")))
		self.__op_enable(3)

	def forest_3_load(self):

		self.__ui.forest_3.setText(QFileDialog.getOpenFileName(self, QApplication.translate("CSFireDialog", "Forest map"), ".",
			QApplication.translate("CSFireDialog", "ESRI Shapefile (*.shp)")))
		self.__op_enable(3)

	def svm_123_3_load(self):

		self.__ui.svm_123_3.setText(QFileDialog.getOpenFileName(self,
			QApplication.translate("CSFireDialog", "SVM, that separates the forest class and other classes"), ".",
			QApplication.translate("CSFireDialog", "SVM file (*.svm)")))
		self.__op_enable(3)

	def svm_12_3_load(self):

		self.__ui.svm_12_3.setText(QFileDialog.getOpenFileName(self,
			QApplication.translate("CSFireDialog", "SVM, that separates the other classes"), ".",
			QApplication.translate("CSFireDialog", "SVM file (*.svm)")))
		self.__op_enable(3)

	def dst_3_save(self):

		dialog = QFileDialog(self, QApplication.translate("CSFireDialog", "Resulting mask"), ".",
				QApplication.translate("CSFireDialog", "GeoTIFF (*.tif *tiff)"))
		
		dialog.setAcceptMode(QFileDialog.AcceptSave)
		dialog.setDefaultSuffix("tif")

		if dialog.exec_():

			self.__ui.dst_3.setText(dialog.selectedFiles()[0])

		else:

			self.__ui.dst_3.setText("")

		self.__op_enable(3)

	def __op_begin(self):

		self.__widgets_status = []

		for w in self.__widgets:

			self.__widgets_status.append(w.isEnabled())
			w.setEnabled(False)

	def __op_end(self):

		for i in range(0, len(self.__widgets)):

			self.__widgets[i].setEnabled(self.__widgets_status[i])

	def __op_enable(self, ind):

		if ind == 1:

			self.__ui.op_1.setEnabled(not (self.__ui.high_1.text().isEmpty() or self.__ui.low_1.text().isEmpty() or self.__ui.aoi_1.text().isEmpty() or \
					self.__ui.forest_1.text().isEmpty() or self.__ui.c3_1.text().isEmpty() or self.__ui.c2_1.text().isEmpty() or \
					self.__ui.c1_1.text().isEmpty()))

		elif ind == 2:

			self.__ui.op_2.setEnabled(not (self.__ui.c3_2.text().isEmpty() or self.__ui.c2_2.text().isEmpty() or self.__ui.c1_2.text().isEmpty() or \
					self.__ui.svm_123_2.text().isEmpty() or self.__ui.svm_12_2.text().isEmpty()))

		elif ind == 3:

			self.__ui.op_3.setEnabled(not (self.__ui.prepare_3.text().isEmpty() or self.__ui.aoi_3.text().isEmpty() or \
					self.__ui.forest_3.text().isEmpty() or self.__ui.svm_123_3.text().isEmpty() or self.__ui.svm_12_3.text().isEmpty() or \
					self.__ui.dst_3.text().isEmpty()))

	def op_1(self):

		self.__op_begin()

		try:

			(child, queue) = sfire.gen_samples(
				sat_first_dname = str(self.__ui.high_1.text().toUtf8()),
				sat_second_dname = str(self.__ui.low_1.text().toUtf8()),
				shp_aoi_fname = str(self.__ui.aoi_1.text().toUtf8()),
				shp_forest_fname = str(self.__ui.forest_1.text().toUtf8()),
				out_fname = [str(self.__ui.c1_1.text().toUtf8()), str(self.__ui.c2_1.text().toUtf8()), str(self.__ui.c3_1.text().toUtf8())])

			self.__wait(child, queue)
			
		except:

			self.__message("Fail")

		self.__op_end()

	def op_2(self):

		self.__op_begin()

		try:

			(child, queue) = sfire.teach(
				in_fname = [str(self.__ui.c1_2.text().toUtf8()), str(self.__ui.c2_2.text().toUtf8()), str(self.__ui.c3_2.text().toUtf8())],
				out_fname = [str(self.__ui.svm_123_2.text().toUtf8()), str(self.__ui.svm_12_2.text().toUtf8())])

			self.__wait(child, queue)

		except:

			self.__message("Fail")

		self.__op_end()

	def op_3(self):

		self.__op_begin()

		try:

			(child, queue) = sfire.process(
				sat_dname = str(self.__ui.prepare_3.text().toUtf8()), 
				result_fname = str(self.__ui.dst_3.text().toUtf8()),
				shp_aoi_fname = str(self.__ui.aoi_3.text().toUtf8()),
				shp_forest_fname = str(self.__ui.forest_3.text().toUtf8()),
				svm_fname = [str(self.__ui.svm_123_3.text().toUtf8()), str(self.__ui.svm_12_3.text().toUtf8())])

			if self.__wait(child, queue) and self.__ui.is_load_3.isChecked():

				path = self.__ui.dst_3.text()
				layer = QgsRasterLayer(path, QFileInfo(path).baseName())
				
				if not layer.isValid():
					
					raise

				QgsMapLayerRegistry.instance().addMapLayer(layer)

		except:

			self.__message("Fail")

		self.__op_end()

class CAbout(QDialog):

	def __init__(self, tfname):

		QDialog.__init__(self)

		translator = QTranslator()
		translator.load(tfname)
		QApplication.installTranslator(translator)

		self.__ui = dialog_about.Ui_AboutDialog()
		self.__ui.setupUi(self)

class CSFirePlugin():
	
	def __init__(self, iface):
		
		############################################################################ 
		# Язык (en или ru)

		lname = str(QLocale.system().name())[0:2]

		if lname not in ["en", "ru"]:

			lname = "en"

		self.__tfname = ":/plugins/sfire/i10n/translation/i10n_" + lname + ".qm"

		############################################################################ 

		translator = QTranslator()
		translator.load(self.__tfname)
		QApplication.installTranslator(translator)

		self.iface = iface
		self.plugin_name = QApplication.translate("CSFireDialog", "Mapping of burnt forest areas")
		self.item = {
				"sfire" : {"action" : None, "run" : self.run_sfire, "ename" : "sfire",
					"name" : QApplication.translate("CSFireDialog", "Mapping of burnt forest areas")},
				"about" : {"action" : None, "run" : self.run_about, "ename" : "about", "name" : QApplication.translate("CSFireDialog", "About plugin")}
				}

	def initGui(self):
		
		for item in self.item:

			self.item[item]["action"] = QAction(QIcon(":/plugins/sfire/icons/" + self.item[item]["ename"] + ".png"), self.item[item]["name"],
					self.iface.mainWindow())
			self.item[item]["action"].setWhatsThis(self.item[item]["name"])
			self.item[item]["action"].setStatusTip(self.item[item]["name"])
			QObject.connect(self.item[item]["action"], SIGNAL("triggered()"), self.item[item]["run"])

			self.iface.addToolBarIcon(self.item[item]["action"])
			self.iface.addPluginToMenu(self.plugin_name, self.item[item]["action"])
		
		QObject.connect(self.iface.mapCanvas(), SIGNAL("renderComplete(QPainter *)"), self.renderTest)
		
	def unload(self):
		
		for item in self.item:

			self.iface.removePluginMenu(self.plugin_name, self.item[item]["action"])
			self.iface.removeToolBarIcon(self.item[item]["action"])
		
		QObject.disconnect(self.iface.mapCanvas(), SIGNAL("renderComplete(QPainter *)"), self.renderTest)

	def run_sfire(self):
		
		CSFireDialog(self.__tfname).exec_()

	def run_about(self):
		
		CAbout(self.__tfname).exec_()
		
	def renderTest(self, painter):
		
		pass

