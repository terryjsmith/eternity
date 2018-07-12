#-------------------------------------------------
#
# Project created by QtCreator 2018-07-10T19:47:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS GLEW_STATIC "GIGA_API="

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    eternityopenglwidget.cpp \
    ../ThirdParty/glew/src/glew.c

HEADERS  += mainwindow.h \
    eternityopenglwidget.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86'
win32:CONFIG(release, debug|release): DEPENDPATH += 'C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86'

win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86'
win32:CONFIG(debug, debug|release): DEPENDPATH += 'C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86'

# windows stuff
win32:CONFIG(release, debug|release): LIBS += OpenGL32.lib WS2_32.lib User32.lib WinMM.Lib AdvAPI32.Lib Crypt32.Lib shell32.lib Gdi32.lib Dbghelp.lib ShLwApi.Lib
else:win32:CONFIG(debug, debug|release): LIBS += OpenGL32.lib WS2_32.lib User32.lib WinMM.Lib AdvAPI32.Lib Crypt32.Lib shell32.lib Gdi32.lib Dbghelp.lib ShLwApi.Lib

# main eternity engine
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Bin/ eternity.lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Bin/ eternity.lib
else:unix: LIBS += -L$$PWD/../../Bin/ -leternity -framework Cocoa -framework IOKit  -framework CoreVideo -framework OpenGL -framework Security

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

# bullet physics
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ThirdParty/bullet/lib/ -lBulletCollision -lBulletDynamics -lLinearMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ThirdParty/bullet/lib/ -lBulletCollision -lBulletDynamics -lLinearMath
else:unix: LIBS += -L$$PWD/../ThirdParty/bullet/lib/  -lBulletCollision -lBulletDynamics -lLinearMath

INCLUDEPATH += $$PWD/../ThirdParty/bullet/include
DEPENDPATH += $$PWD/../ThirdParty/bullet/include

# glew for opengl functions
INCLUDEPATH += $$PWD/../ThirdParty/glew/include
DEPENDPATH += $$PWD/../ThirdParty/glew/include

# glfw for input
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ThirdParty/glfw/lib/ -lglfw3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ThirdParty/glfw/lib/ -lglfw3
else:unix: LIBS += -L$$PWD/../ThirdParty/glfw/lib/ -lglfw3

INCLUDEPATH += $$PWD/../ThirdParty/glfw/include
DEPENDPATH += $$PWD/../ThirdParty/glfw/include

# glm for math
INCLUDEPATH += $$PWD/../ThirdParty/glm/include
DEPENDPATH += $$PWD/../ThirdParty/glm/include

# SOIL for loading textures
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ThirdParty/soil/lib/ SOIL.lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ThirdParty/soil/lib/ SOIL.lib
else:unix: LIBS += -L$$PWD/../ThirdParty/soil/lib/ -lSOIL

INCLUDEPATH += $$PWD/../ThirdParty/soil/include
DEPENDPATH += $$PWD/../ThirdParty/soil/include

# V8 for JavaScript
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ThirdParty/v8/lib/ -lv8_base -lv8_libbase v8_libplatform.lib -lv8_libsampler -lv8_external_snapshot -licui18n -licuuc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ThirdParty/v8/lib/ -lv8_base -lv8_libbase v8_libplatform.lib -lv8_libsampler -lv8_external_snapshot -licui18n -licuuc
else:unix: LIBS += -L$$PWD/../ThirdParty/v8/lib/ -lv8_base -lv8_libbase -lv8_libplatform -lv8_libsampler -lv8_external_snapshot -licui18n -licuuc

INCLUDEPATH += $$PWD/../ThirdParty/v8/include
DEPENDPATH += $$PWD/../ThirdParty/v8/include
