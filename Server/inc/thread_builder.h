#ifndef THREAD_BUILDER_H
#define THREAD_BUILDER_H

#include <QThread>

template <typename T>
QThread* newThread(QObject *parent, T *threadObj, bool start = true){
    if(!std::is_base_of<QObject, T>::value){
        qDebug("newThread: incoming object is not a QObject");
        return nullptr;
    }
    auto thread = new QThread(parent);
    threadObj->moveToThread(thread);
    QObject::connect(thread, &QThread::started, threadObj, &T::process, Qt::DirectConnection);
    QObject::connect(threadObj, &T::finished, thread, &QThread::quit, Qt::DirectConnection);
    QObject::connect(threadObj, &T::finished, threadObj, &T::deleteLater, Qt::DirectConnection);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater,
                     Qt::DirectConnection);
    if(start) thread->start();
    return thread;
}

#endif //THREAD_BUILDER_H
