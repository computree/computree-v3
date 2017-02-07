isEmpty(CT_PREFIX) {
    CT_PREFIX = ..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = ../..
}

CONFIG(debug, debug|release) {
    CT_DESTDIR_NAME = ComputreeInstallDebug
} else {
    CT_DESTDIR_NAME = ComputreeInstallRelease
}

PLUGINSHARED_DESTDIR = $${CT_PREFIX_INSTALL}/$${CT_DESTDIR_NAME}
PLUGINSHARED_PLUGIN_DESTDIR = $${PLUGINSHARED_DESTDIR}/plugins

message(Element(s) will be installed to $${PLUGINSHARED_DESTDIR})

contains(QMAKE_TARGET.arch, x86_64) {
    DEFINES += ENVIRONMENT64
} else {
    isEmpty(QMAKE_TARGET.arch) {
        *-64 {
            DEFINES += ENVIRONMENT64
        } else {
            DEFINES += ENVIRONMENT32
        }
    } else {
        DEFINES += ENVIRONMENT32
    }
}
