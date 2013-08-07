TEMPLATE = subdirs
SUBDIRS =  example \
    sportIdentReader \
    sportIdentReader/SportIdentMessageParserTest

example.depends = sportIdentReader
