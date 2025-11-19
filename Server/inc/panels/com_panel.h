#ifndef COM_PANEL_H
#define COM_PANEL_H

#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>
#include "file_work.h"

class ComPanel: public QFrame{
private:
    FileWork *fileWork;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QCheckBox* autoStartCheckBox;
public:
    explicit ComPanel(QWidget *parent);

    ~ComPanel() override;

    void lock();
    void unlock();
    char *getIP();
    uint16_t getPort();
    QString getQIP();
    QString getQPort();
    uint16_t isAutostart();
    void readConfig();
    void saveConfig();
};
#endif //COM_PANEL_H
