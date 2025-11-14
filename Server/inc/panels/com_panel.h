#ifndef COM_PANEL_H
#define COM_PANEL_H

#include <QFrame>
#include <QLineEdit>

class ComPanel: public QFrame{
private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
public:
    explicit ComPanel(QWidget *parent);
    void lock();
    void unlock();
    char *getIP();
    uint16_t getPort();
};
#endif //COM_PANEL_H
