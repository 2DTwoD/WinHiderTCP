#ifndef COM_PANEL_H
#define COM_PANEL_H

#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>

class ComPanel: public QFrame{
private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QCheckBox* autoStartCheckBox;
public:
    explicit ComPanel(QWidget *parent);
    void lock();
    void unlock();
    char *getIP();
    uint16_t getPort();
    uint16_t isAutostart();
    void readConfig();
    void saveConfig();
};
#endif //COM_PANEL_H
