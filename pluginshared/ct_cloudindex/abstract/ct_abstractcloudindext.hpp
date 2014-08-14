#ifndef CT_ABSTRACTCLOUDINDEXT_HPP
#define CT_ABSTRACTCLOUDINDEXT_HPP

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

#include "ct_global/ct_context.h"

template<typename T>
T& CT_AbstractCloudIndexT<T>::tAt(const size_t &index) const
{
    return (*PS_REPOSITORY->globalCloud<T>())[ indexAt(index) ];
}

template<typename T>
T& CT_AbstractCloudIndexT<T>::tAt(const size_t &index, size_t &globalIndex) const
{
    indexAt(index, globalIndex);
    return (*PS_REPOSITORY->globalCloud<T>())[ globalIndex ];
}

template<typename T>
T& CT_AbstractCloudIndexT<T>::tAtGlobalIndex(const size_t &index) const
{
    return (*PS_REPOSITORY->globalCloud<T>())[ index ];
}

template<typename T>
const T& CT_AbstractCloudIndexT<T>::constTAt(const size_t &index) const
{
    return (*PS_REPOSITORY->globalCloud<T>())[ indexAt(index) ];
}

template<typename T>
const T& CT_AbstractCloudIndexT<T>::constTAt(const size_t &index, size_t &globalIndex) const
{
    indexAt(index, globalIndex);
    return (*(const CT_AbstractCloudT<T>*)PS_REPOSITORY->globalCloud<T>())[ globalIndex ];
}

template<typename T>
const T& CT_AbstractCloudIndexT<T>::constTAtGlobalIndex(const size_t &globalIndex) const
{
    return (*(const CT_AbstractCloudT<T>*)PS_REPOSITORY->globalCloud<T>())[ globalIndex ];
}


#endif // CT_ABSTRACTCLOUDINDEXT_HPP
