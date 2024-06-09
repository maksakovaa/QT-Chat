#pragma once
#include "sha256d.h"

template<class T> class Base : public QObject
{
protected:
    QVector<T>* base;
public:
    Base();
    ~Base();
    void addInBase(T& newElem);
    void resetBase();
    size_t getCount();
};

template<class T> Base<T>::Base()
{
    base = new QVector<T>;
}

template<class T> Base<T>::~Base()
{
    delete base;
}

template<class T> void Base<T>::addInBase(T& newElem)
{
    base->push_back(std::move(newElem));
}

template<class T> void Base<T>::resetBase()
{
    base->clear();
}

template<class T> size_t Base<T>::getCount()
{
    return base->size();
}
