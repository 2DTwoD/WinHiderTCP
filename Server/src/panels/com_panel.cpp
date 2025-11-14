#include "panels/com_panel.h"

#include <QLabel>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

ComPanel::ComPanel(QWidget *parent): QFrame(parent) {
    auto layout = new QHBoxLayout(this);
    this->setLayout(layout);

    auto ipLabel = new QLabel("IP:", this);
    auto portLabel = new QLabel("Port:", this);
    ipLineEdit = new QLineEdit("127.0.0.1", this);
    portLineEdit = new QLineEdit("55555", this);

    layout->addWidget(ipLabel);
    layout->addWidget(ipLineEdit);

    QRegularExpression ipRE(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.){3}(25[0-5]|(2[0-4]|1\d|[1-9]|)\d)$)");
    auto ipValidator = new QRegularExpressionValidator(ipRE, this);
    ipLineEdit->setValidator(ipValidator);

    QRegularExpression portRE("\\d+");
    auto portValidator = new QRegularExpressionValidator(portRE, this);
    portLineEdit->setValidator(portValidator);

    layout->addWidget(portLabel);
    layout->addWidget(portLineEdit);
}

void ComPanel::lock() {
    ipLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void ComPanel::unlock() {
    ipLineEdit->setEnabled(true);
    portLineEdit->setEnabled(true);
}

char *ComPanel::getIP() {
    if(ipLineEdit->text().split('.').size() != 4){
        ipLineEdit->setText("127.0.0.1");
    }
    return ipLineEdit->text().toLocal8Bit().data();
}

uint16_t ComPanel::getPort() {
    uint16_t res = portLineEdit->text().toUShort();
    if(!res) res = 55555;
    portLineEdit->setText(QString::number(res));
    return res;
}