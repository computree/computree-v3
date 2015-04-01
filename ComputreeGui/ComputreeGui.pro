include(../common.pri)
include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

CONFIG -= plugin

TARGET = CompuTreeGui
TEMPLATE = app
QT += opengl
QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR  = .ui
RCC_DIR = .rcc

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent


HEADERS += $${PLUGIN_SHARED_DIR}/interfaces.h \
    dm_graphicsviewsynchronizedgroup.h \
    dm_graphicsviewsynchronizedgroupoptions.h \
    dm_itemdrawablemodelmanager.h \
    dm_multipleitemdrawablemodelmanager.h \
    dm_itemmodelviewsynchronizedgroup.h \
    dm_abstractviewsynchronizedgroup.h \
    dm_itemmodelviewsynchronizedgroupoptions.h \
    dm_actionsmanager.h \
    dm_actionshandler.h \
    imainprogressdialog.h \
    dm_abstractinfo.h \
    dm_iteminfoforgraphics.h \
    dm_iprogresslistener.h \
    dm_progresslistenermanager.h \
    dm_fastestincrementoptimizer.h \
    dm_colorvbomanager.h \
    dm_vertexvbomanager.h \
    dm_opengltools.h

# OTHER
SOURCES += main.cpp \
    dm_document.cpp \
    dm_graphicsview.cpp \
    dm_graphicsviewoptions.cpp \
    dm_documentview.cpp \
    dm_documentmanagerview.cpp \
    dm_documentmanager.cpp \
    dm_guimanager.cpp \
    dm_mainwindow.cpp \
    dm_asynchroneprogress.cpp \
    dm_itemdrawablemanageroptions.cpp \
    dm_graphicsviewcamera.cpp \
    dm_graphicsviewsynchronizedgroup.cpp \
    dm_graphicsviewsynchronizedgroupoptions.cpp \
    dm_itemdrawablemodelmanager.cpp \
    dm_multipleitemdrawablemodelmanager.cpp \
    dm_itemmodelviewsynchronizedgroup.cpp \
    dm_abstractviewsynchronizedgroup.cpp \
    dm_itemmodelviewsynchronizedgroupoptions.cpp \
    dm_actionsmanager.cpp \
    dm_actionshandler.cpp \
    dm_abstractinfo.cpp \
    dm_iteminfoforgraphics.cpp \
    dm_progresslistenermanager.cpp \
    dm_fastestincrementoptimizer.cpp \
    dm_colorvbomanager.cpp \
    dm_vertexvbomanager.cpp \
    dm_opengltools.cpp

HEADERS += \
    dm_document.h \
    dm_graphicsview.h \
    dm_graphicsviewoptions.h \
    dm_documentview.h \
    dm_documentmanagerview.h \
    dm_documentmanager.h \
    dm_guimanager.h \
    dm_mainwindow.h \
    dm_asynchroneprogress.h \
    dm_itemdrawablemanageroptions.h \
    dm_graphicsviewcamera.h \
    dm_context.h
    
TRANSLATIONS += languages/computreegui_fr.ts \
                languages/computreegui_en.ts

include(view/view.pri)
include(qtcolorpicker/qtcolorpicker.pri)
include(muParser/muparser.pri)
include(tools/tools.pri)

INCLUDEPATH += ./
INCLUDEPATH += ../QGLViewer

INCLUDEPATH += $${PLUGIN_SHARED_DIR}
INCLUDEPATH += $${EXPORTER_SHARED_DIR}

INCLUDEPATH += $${COMPUTREE_CORE_DIR}
INCLUDEPATH += $${COMPUTREE_CORE_DIR}/src

RESOURCES += resource/icones.qrc

macx {
    LIBS += $${DESTDIR}/libCompuTreeCore*.dylib
} else {
    unix{
        LIBS += $${DESTDIR}/libCompuTreeCore*.so
    }

    win32 {
        win32-g++ {
        LIBS += $${DESTDIR}/libCompuTreeCore*.a
        }

        win32-msvc* {
        LIBS += $${DESTDIR}/CompuTreeCore*.lib
        }
    }
}

win32 {
  ###### Uncomment this line to compile a debug version of your application
  #CONFIG *= debug_and_release

  # Seems to be needed for Visual Studio with Intel compiler
  DEFINES *= WIN32

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
  }

  # LIB_NAME
  LIB_NAME = QGLViewer
  LIB_DIR = $${DESTDIR}
  build_pass:CONFIG(debug, debug|release) {
    LIB_NAME = $$join(LIB_NAME,,,d)
  }
  LIB_NAME = $$join(LIB_NAME,,,2) #TODO 2

  win32-g++: LIB_FILE_NAME = lib$${LIB_NAME}.a
  !win32-g++: LIB_FILE_NAME = $${LIB_NAME}.lib

  !exists( $${LIB_DIR}/$${LIB_FILE_NAME} ) {
    message( Unable to find $${LIB_FILE_NAME} in $${LIB_DIR} )
  }

  win32-g++ {
    # The actual directory where the library/framework was found
    # LIB_DIR_ABSOLUTE_PATH = $$system(cd $${LIB_DIR} && cd)

    isEmpty( QGLVIEWER_STATIC ) {
      LIBS *= -L$${LIB_DIR} -l$${LIB_NAME}
    } else {
      LIBS *= $${LIB_DIR}/$${LIB_FILE_NAME}
    }
  }

  !win32-g++ {
    # Use the Qt DLL version. Only needed with Qt3
    !contains( QT_VERSION, "^4.*" ) {
      DEFINES *= QT_DLL QT_THREAD_SUPPORT
    }

    LIBS *= -L$${LIB_DIR} -l$${LIB_NAME}

  }
}

### Unix configuration ###
unix {

  CONFIG -= debug debug_and_release
  CONFIG += debug

  isEmpty( PREFIX ) {
    # Try same INCLUDE_DIR and LIB_DIR parameters than for the make install.
    PREFIX=/usr
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer*.so*
  LIB_DIR = $${DESTDIR}
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer.$${QMAKE_EXTENSION_SHLIB}
  }
  hpux {
    LIB_NAME = libQGLViewer*.sl*
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIB_NAME = libQGLViewer*.a
  }

  !exists( $${LIB_DIR}/$${LIB_NAME} ) {
    message( Unable to find $${LIB_NAME} in $${LIB_DIR} )
    #error( You should run qmake LIB_DIR=/path/to/QGLViewer/$${LIB_NAME} )
  }

  # The actual directory where the library/framework was found
  LIB_DIR_ABSOLUTE_PATH = $$system(cd $${LIB_DIR};pwd)


  # INCLUDE_DIR
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = ../
  }

  !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $${INCLUDE_DIR} )
    error( Use qmake INCLUDE_DIR=/path/to/QGLViewerDir )
  }


  macx|darwin-g++ {
    # On Mac, the lib path can be specified in the executable using install_name_tool
    contains( LIB_NAME, ".*QGLViewer.framework.*" ) {
      # If framework was not found in a standard directory
      !contains( LIB_DIR, ".*/Library/Frameworks/*$" ) {
        QMAKE_LFLAGS += -F$${LIB_DIR}
        !plugin:QMAKE_POST_LINK=install_name_tool -change QGLViewer.framework/Versions/2/QGLViewer $${LIB_DIR_ABSOLUTE_PATH}/QGLViewer.framework/Versions/2/QGLViewer $${TARGET}.app/Contents/MacOS/$${TARGET}
      }
      LIBS += -framework QGLViewer
    } else {
        !plugin:QMAKE_POST_LINK=install_name_tool -change libQGLViewer.2.dylib $${LIB_DIR_ABSOLUTE_PATH}/libQGLViewer.2.dylib $${TARGET}.app/Contents/MacOS/$${TARGET}
        LIBS *= -L$${LIB_DIR} -lQGLViewer
    }
  } else {
    isEmpty(QMAKE_LFLAGS_RPATH) {
      !plugin:QMAKE_LFLAGS += -Wl,-rpath,$${LIB_DIR_ABSOLUTE_PATH}
    } else {
      !plugin:QMAKE_RPATHDIR *= $${LIB_DIR_ABSOLUTE_PATH}
    }
    LIBS *= -L$${LIB_DIR} -lQGLViewer

    # Qt 4.8 removed the GLU dependency
    QMAKE_LIBS_OPENGL *= -lGLU
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIBS *= $${LIB_DIR}/$${LIB_NAME}
  }

  macx|darwin-g++ {
    !contains( QT_VERSION, "^4.*" ) {
      # Qt3 only
      LIBS *= -lobjc
      CONFIG -= thread
    }
  }

  # Remove debugging options in release mode makes files much smaller
  release:QMAKE_CFLAGS_RELEASE -= -g
  release:QMAKE_CXXFLAGS_RELEASE -= -g
}

OTHER_FILES +=

