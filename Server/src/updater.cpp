#include "updater.h"
#include "thread_builder.h"

Updater::Updater(QObject* parent){
    newThread(parent, this);
}

void Updater::process() {
    while(!shtdwn) {
        emit update();
        QThread::currentThread()->msleep(500);
    }
    emit finished();
}

void Updater::shutdown() {
    qDebug("Updater: shutdown");
    shtdwn = true;
}

Updater::~Updater() {
    qDebug("Updater: destructor");
}
