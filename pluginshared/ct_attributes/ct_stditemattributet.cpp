#include "ct_stditemattributet.h"

template<>
CT_StdItemAttributeT<QString>::CT_StdItemAttributeT(const QString &categoryName): CT_AbstractItemAttributeT<QString>(categoryName) {
   m_data = QString();
}

template class CT_StdItemAttributeT<QString>;
