#ifndef COM_PANEL_H
#define COM_PANEL_H

#include "misc/file_work.h"

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

    ~ComPanel() override;

    void lock(bool lockFlag);
    void lockAutoStart(bool lockFlag);
    void checkIP();
    void checkPort();
    QString getIP();
    void setIP(const QString& value);
    uint16_t getPort();
    QString getQPort();
    void setPort(const QString& value);
    uint16_t isAutostart();
    void setAutostart(const QString& value);
};
#endif //COM_PANEL_H
