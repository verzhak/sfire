
# Программный код, находящийся в данном файле, распространяется по лицензии GPLv3

SO_FNAME = _wrap_sfire.so

MODULE_FNAME = sfire.py

WRAP_IFACE_FNAME = sfire.swg
WRAP_PYTHON_FNAME = wrap_sfire.py
WRAP_C_FNAME = wrap.c

RESOURCE_DNAME = resources/

SRC = $(WRAP_C_FNAME) main/gen_samples.c main/teach.c main/process.c main/memory.c main/print.c main/svm.c main/various.c
FLAGS = -Wall -pipe -shared -fPIC $(shell gdal-config --cflags)
INCLUDE = -I.
LIB = $(shell gdal-config --libs)

plugin: lib

	pyuic4 -o dialog_sfire.py $(RESOURCE_DNAME)/dialog/sfire.ui
	pyuic4 -o dialog_about.py $(RESOURCE_DNAME)/dialog/about.ui
	pyrcc4 -py2 -o resources_rc.py $(RESOURCE_DNAME)/resources.qrc

lib: clean

	swig -python -outdir $(shell pwd) -o $(WRAP_C_FNAME) $(WRAP_IFACE_FNAME)
	gcc $(FLAGS) $(INCLUDE) $(LIB) $(shell python2-config --libs) $(shell python2-config --cflags) $(shell python2-config --includes) $(SRC) -o $(SO_FNAME)

clean:

	rm -f $(SO_FNAME) $(WRAP_C_FNAME) $(WRAP_PYTHON_FNAME) *.pyc __pycache__/*
	rm -f *pyc dialog_sfire.py dialog_about.py resources_rc.py

distclean: clean

	rm -Rf __pycache__

