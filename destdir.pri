isEmpty(CT_PREFIX) {
    CT_PREFIX = ..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = ../..
}

CONFIG(debug, debug|release) {
    PLUGINSHARED_DESTDIR = $${CT_PREFIX_INSTALL}/ComputreeInstallDebug
} else {
    PLUGINSHARED_DESTDIR = $${CT_PREFIX_INSTALL}/ComputreeInstallRelease
}

PLUGINSHARED_PLUGIN_DESTDIR = $${PLUGINSHARED_DESTDIR}/plugins

message(Element(s) will be installed to $${PLUGINSHARED_DESTDIR})
