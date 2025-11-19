#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater: public QObject{
Q_OBJECT
private:
    bool shtdwn{false};
public:
    explicit Updater(QObject* parent);

    ~Updater() override;

    void shutdown();

public slots:
    void process();
signals:
    void update();
    void finished();
};
#endif //UPDATER_H
