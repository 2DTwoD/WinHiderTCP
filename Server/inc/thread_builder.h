#ifndef THREAD_BUILDER_H
#define THREAD_BUILDER_H

#include <QThread>
#include <iostream>

template <typename T>
bool newThread(T *obj){
    if(!std::is_base_of<QObject, T>::value){
        std::cout << "Object: " << obj << "is not a Object" << std::endl;
        return false;
    }
    auto thread = new QThread();
    obj->moveToThread(thread);
    QObject::connect(thread, &QThread::started, obj, &T::process);
    QObject::connect( obj, &T::finished, thread, &QThread::quit);
    QObject::connect( obj, &T::finished, obj, &T::deleteLater);
    QObject::connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
    return true;
}

#endif //THREAD_BUILDER_H
