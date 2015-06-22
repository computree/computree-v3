#ifndef CT_INDEXCLOUDCOLORSTDMAPT_HPP
#define CT_INDEXCLOUDCOLORSTDMAPT_HPP

#include "ct_colorcloud/ct_indexcloudcolorstdmapt.h"

template<typename T>
CT_IndexCloudColorStdMapT<T>::CT_IndexCloudColorStdMapT() : CT_CloudIndexStdMapT<T, CT_Color>(), CT_AbstractModifiableIndexCloudColorMap()
{
}

template<typename T>
void CT_IndexCloudColorStdMapT<T>::insertIndexAndColor(const size_t &newIndex, const CT_Color &color)
{
    // insert a new element, if the element already exist it will be ignored
    this->internalData()->insert( std::pair<ct_index_type,CT_Color>(newIndex, color) );
}

template<typename T>
CT_Color* CT_IndexCloudColorStdMapT<T>::colorAtGlobalIndex(const size_t &index, const CT_Color *defaultValue) const
{
    std::map< ct_index_type, CT_Color > *collect = this->internalData();
    typename std::map< ct_index_type,CT_Color >::const_iterator it = collect->find(index);

    if(it != collect->end())
        return (CT_Color*)&(it->second);

    return (CT_Color*)defaultValue;
}

template<typename T>
CT_Color* CT_IndexCloudColorStdMapT<T>::colorAt(const size_t &index, const CT_Color *defaultValue) const
{
    std::map< ct_index_type, CT_Color > *collect = this->internalData();
    typename std::map< ct_index_type,CT_Color >::const_iterator it = collect->begin();
    std::advance(it, index);

    if(it != collect->end())
        return (CT_Color*)&(it->second);

    return (CT_Color*)defaultValue;
}

template<typename T>
void CT_IndexCloudColorStdMapT<T>::copyColors(CT_AbstractColorCloud *cc) const
{
    std::map< ct_index_type, CT_Color > *collect = this->internalData();
    typename std::map< ct_index_type,CT_Color >::const_iterator it = collect->begin();
    typename std::map< ct_index_type,CT_Color >::const_iterator end = collect->end();

    while(it != end)
    {
        const CT_Color &colBackup = it->second;
        CT_Color &col = cc->colorAt(it->first);

        colBackup.copy(col);

        ++it;
    }
}

template<typename T>
void CT_IndexCloudColorStdMapT<T>::copyColorsOfKeys(CT_AbstractColorCloud *cc, const std::vector<size_t> &indexes, bool eraseKeys)
{
    std::vector<size_t>::const_iterator itV = indexes.begin();
    std::vector<size_t>::const_iterator endV = indexes.end();

    std::map< ct_index_type, CT_Color > *collect = this->internalData();

    while(itV != endV)
    {
        typename std::map< ct_index_type,CT_Color >::iterator it = collect->find(*itV);

        if(it != collect->end())
        {
            const CT_Color &colBackup = it->second;
            CT_Color &col = cc->colorAt(it->first);

            colBackup.copy(col);

            if(eraseKeys)
                collect->erase(it);
        }

        ++itV;
    }
}

template<typename T>
void CT_IndexCloudColorStdMapT<T>::copyColorsOfCloudIndex(CT_AbstractColorCloud *cc, const QList<CT_AbstractCloudIndex*> &lci, bool eraseKeys)
{
    std::map<ct_index_type, CT_Color > *collect = this->internalData();

    QListIterator<CT_AbstractCloudIndex*> itC(lci);

    while(itC.hasNext())
    {
        CT_AbstractCloudIndexT<T> *ci = dynamic_cast<CT_AbstractCloudIndexT<T>*>(itC.next());

        if(ci != NULL)
        {
            CT_CloudIndexIteratorT<T> itK(ci);

            while(itK.hasNext())
            {
                typename std::map< ct_index_type,CT_Color >::iterator it = collect->find(itK.next().cIndex());

                if(it != collect->end())
                {
                    const CT_Color &colBackup = it->second;
                    CT_Color &col = cc->colorAt(it->first);

                    colBackup.copy(col);

                    if(eraseKeys)
                        collect->erase(it);
                }
            }
        }
    }
}


template<typename T>
void CT_IndexCloudColorStdMapT<T>::removeIndex(const size_t &index)
{
    CT_CloudIndexStdMapT<T, CT_Color>::removeIndex(index);
}


template<typename T>
void CT_IndexCloudColorStdMapT<T>::clear()
{
    CT_CloudIndexStdMapT<T, CT_Color>::clear();
}

template<typename T>
CT_AbstractCloud* CT_IndexCloudColorStdMapT<T>::copy() const
{
    CT_IndexCloudColorStdMapT<T> *index = new CT_IndexCloudColorStdMapT<T>();

    std::map< ct_index_type, CT_Color > *myCollect = this->internalData();
    std::map< ct_index_type, CT_Color > *collect = index->internalData();
    collect->insert(myCollect->begin(), myCollect->end());

    return index;
}

#endif // CT_INDEXCLOUDCOLORSTDMAPT_HPP
