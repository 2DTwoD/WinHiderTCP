#ifndef COM_PANEL_H
#define COM_PANEL_H

#include "misc/file_work.h"

#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>

class ComPanel: public QFrame{
private:
    FileWork *fileWork;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QCheckBox* autoStartCheckBox;
public:
    explicit ComPanel(QWidget *parent);

    ~ComPanel() override;

    void lock(bool value);
    char *getIP();
    uint16_t getPort();
    QString getQIP();
    QString getQPort();
    uint16_t isAutostart();
    void readConfig();
    void saveConfig();
};
#endif //COM_PANEL_H
