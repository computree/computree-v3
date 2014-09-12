isEmpty(CT_PREFIX) {
    CT_PREFIX = ..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = ../..
}

PLUGINSHARED_DESTDIR = $${CT_PREFIX_INSTALL}/ComputreeInstall
PLUGINSHARED_PLUGIN_DESTDIR = $${PLUGINSHARED_DESTDIR}/plugins

message(Element(s) will be installed to $${PLUGINSHARED_DESTDIR})
