# To use GDAL =>
# Do the declaration in file use_gdal.ini in this order with quotes for all lines :

# example for ubuntu 13.10 or osx (remove # symbols) :
#"/usr/local/bin/gdal"

# example for windows (remove # symbols) WARNING : slash must be like this / :
#"C:/gdal"

exists(../use_gdal.ini) {
    message( ------- use_gdal.ini detected ------ )
    TMP_CAT =

    unix | mac {
        TMP_CAT = $$system( cat ../use_gdal.ini )
    }

    win32 {
        TMP_CAT = $$system( type ..\use_gdal.ini )
    }

    #GDAL path
    GDAL_REPLACE = \"
    GDAL_REPLACEBY =
    GDAL_PATH = $$member(TMP_CAT, 0)
    GDAL_PATH = $$replace(GDAL_PATH, $$GDAL_REPLACE, $$GDAL_REPLACEBY)

    exists($$GDAL_PATH) {

        message( ------- Path of GDAL found : $$GDAL_PATH ------- )

        GDAL_INCLUDE_PATH = $$GDAL_PATH/include

        #unix
        unix {
            GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal*.so
        }

        mac {
            GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal*.dylib
        }

        # windows
        win32 {
            msvc {
                GDAL_LIB_PATH = $$GDAL_PATH/lib/gdal*.lib
            }

            mingw {
                GDAL_LIB_PATH = $$GDAL_PATH/lib/libgdal.a
            }
        }

        message(GDAL INCLUDE DIRECTORY : $$GDAL_INCLUDE_PATH)
        message(GDAL LIB PATH : $$GDAL_LIB_PATH)

        INCLUDEPATH += $$GDAL_INCLUDE_PATH
        LIBS += $$GDAL_LIB_PATH
        LIBS += -lws2_32
        LIBS += -lz
        LIBS += -liconv

    } else {
        message( ------- Path of GDAL NOT found : $$GDAL_PATH ------- )
    }

} else {
    message( ------- use_gdal.ini NOT detected ------ )
}
