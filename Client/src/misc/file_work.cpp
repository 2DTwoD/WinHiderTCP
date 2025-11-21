#include "misc/file_work.h"

#include <qtextstream.h>
#include <QFile>

FileWork::FileWork(QObject *parent) : QObject(parent) {}

QString FileWork::readConfig() {
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug("FileWork: cannot open file for reading");
        return "";
    }
    qDebug("FileWork: file with config opened");
    QTextStream in(&file);
    auto result  = in.readLine();
    file.close();
    return result;
}

void FileWork::saveConfig(QString&& saveLine) {
    QString filename = "config.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << saveLine;
        qDebug("FileWork: file with config saved");
    } else {
        qDebug("FileWork: could not open file");
    }
    file.close();
}

FileWork::~FileWork() {
    qDebug("FileWork: destructor");
}
