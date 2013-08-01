TEMPLATE = subdirs

SUBDIRS =  GSModule \
	   cliExample \
           guiExample

cliExample.depends = GSModule
guiExample.depends = GSModule
