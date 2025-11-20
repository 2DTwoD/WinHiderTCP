#include "updater.h"
#include "thread_builder.h"

Updater::Updater(QObject* parent){
    currentThread = newThread<Updater>(parent, this);
}

void Updater::process() {
    while(!shtdwn) {
        emit update();
        QThread::msleep(500);
    }
    emit finished();
}

void Updater::shutdown() {
    qDebug("Updater: shutdown");
    shtdwn = true;
    currentThread->wait(1000);
}

Updater::~Updater() {
    qDebug("Updater: destructor");
}
