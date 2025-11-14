#include "updater.h"
#include "thread_builder.h"

Updater::Updater(){
    newThread(this);
}

void Updater::process() {
    while(!shtdwn) {
        emit update();
        QThread::currentThread()->msleep(500);
    }
    emit finished();
}

void Updater::shutdown() {
    shtdwn = true;
}
