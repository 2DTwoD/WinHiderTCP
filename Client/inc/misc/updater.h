#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QThread>

class Updater: public QObject{
Q_OBJECT
private:
    QThread* currentThread{nullptr};
    bool shtdwn{false};
    int updateTime;
public:
    Updater(QObject* parent, int updateTime = 500);
    ~Updater() override;

    void shutdown();
public slots:
    void process();
signals:
    void update();
    void finished();
};
#endif //UPDATER_H
