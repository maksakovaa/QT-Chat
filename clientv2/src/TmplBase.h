#pragma once
#include <QVector>
#include <QString>
#include "qnet.h"
#include <QObject>

template<class T> class Base: public QObject
{
public:
    Base();
    ~Base();
    void addInBase(T newElem);
    size_t getCount();
    void setNetPtr(QNet *ptr);
protected:
    QVector<T>* base;
public:
    QNet* Online;
};

template<class T> Base<T>::Base()
{
    base = new QVector<T>;
}

template<class T> Base<T>::~Base()
{
    delete base;
}

template<class T> void Base<T>::addInBase(T newElem)
{
    base->push_back(std::move(newElem));
}

template<class T> size_t Base<T>::getCount()
{
    return base->size();
}

template<class T> void Base<T>::setNetPtr(QNet *ptr)
{
    Online = ptr;
}
