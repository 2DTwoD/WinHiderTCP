#ifndef THREAD_BUILDER_H
#define THREAD_BUILDER_H

#include <QThread>
#include <iostream>

template <typename T>
bool newThread(QObject *parent, T *threadObj){
    if(!std::is_base_of<QObject, T>::value){
        qDebug("newThread, incoming object is not a QObject");
        return false;
    }
    auto thread = new QThread(parent);
    threadObj->moveToThread(thread);
    QObject::connect(thread, &QThread::started, threadObj, &T::process);
    QObject::connect(threadObj, &T::finished, thread, &QThread::quit);
    QObject::connect(threadObj, &T::finished, threadObj, &T::deleteLater);
    QObject::connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
    return true;
}

#endif //THREAD_BUILDER_H
