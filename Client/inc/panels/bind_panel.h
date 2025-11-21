#ifndef BIND_PANEL_H
#define BIND_PANEL_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

class BindPanel: public QFrame{
private:
    QLineEdit *winNameLineEdit;
    QPushButton *bindKeyButton;
public:
    explicit BindPanel(QWidget *parent);

//    ~BindPanel() override;
//
//    void lock();
//    void unlock();
};

#endif //BIND_PANEL_H
