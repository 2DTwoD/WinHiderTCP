#ifndef FILE_WORK_H
#define FILE_WORK_H

#include <QObject>

class FileWork: public QObject{
public:
    explicit FileWork(QObject *parent);

    ~FileWork() override;

    QString readConfig();

    void saveConfig(QString&& saveLine);
};

#endif //FILE_WORK_H
