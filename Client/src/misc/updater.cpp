#include "misc/updater.h"
#include "misc/thread_builder.h"

Updater::Updater(QObject* parent, int updateTime): updateTime(updateTime){
    currentThread = newThread<Updater>(parent, this);
}

void Updater::process() {
    while(!shtdwn) {
        emit update();
        QThread::msleep(updateTime);
    }
    emit finished();
}

void Updater::shutdown() {
    qDebug("Updater: shutdown");
    shtdwn = true;
    currentThread->wait(updateTime);
}

Updater::~Updater() {
    qDebug("Updater: destructor");
}
