TEMPLATE = subdirs

SUBDIRS =  QtGSM \
	   sportIdentReader \
           mainApp

mainApp.depends = QtGSM
mainApp.depends = sportIdentReader
