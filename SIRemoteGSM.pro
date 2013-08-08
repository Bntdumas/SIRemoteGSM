TEMPLATE = subdirs

SUBDIRS =  QtGSM \
	   sportIdentReader \
           mainApp

CONFIG += testcase

mainApp.depends = QtGSM
mainApp.depends = sportIdentReader
