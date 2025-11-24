#ifndef BIND_PANEL_H
#define BIND_PANEL_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include "hider/win_work.h"

class BindPanel: public QFrame {
Q_OBJECT

private:
    QLineEdit *winNameLineEdit;
    QPushButton *bindKeyButton;
public:
    explicit BindPanel(QWidget *parent);

    ~BindPanel() override;

    QPushButton *getBindButton();

    void lock(bool lockFlag);

    QString getQWinName();

    void setQWinName(const QString& value);

    QString getQKey();

    void setQKey(const QString& value);

public slots:

    void bindButtonAction();

    void bindFinished(const QString& result);

signals:

    void startBind();
};

#endif //BIND_PANEL_H
