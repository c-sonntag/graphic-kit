

CONFIG += c++14
INCLUDEPATH += $$PWD/include
DEPENDSPATH += $$PWD

#
##
SOURCES += $$files($$PWD/*.cpp, true)
HEADERS += $$files($$PWD/include/graphic_toolkit/*.h, true)
HEADERS += $$files($$PWD/include/graphic_toolkit/*.ipp, true)
FORMS += $$files($$PWD/*.ui, true)
