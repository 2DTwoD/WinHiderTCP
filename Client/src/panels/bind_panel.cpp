#include "panels/bind_panel.h"

#include <QHBoxLayout>
#include <QLabel>

BindPanel::BindPanel(QWidget *parent): QFrame(parent) {
    auto layout = new QHBoxLayout(this);
    this->setLayout(layout);

    auto winNameLabel = new QLabel("Название окна:", this);
    auto bindKeyLAbel = new QLabel("Кнопка действия:", this);
    winNameLineEdit = new QLineEdit("New folder", this);
    bindKeyButton = new QPushButton("WM_NULL", this);
//    portLineEdit->setFixedWidth(40);
//    autoStartCheckBox = new QCheckBox("Autostart", this);
//
//    QRegularExpression ipRE(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.){3}(25[0-5]|(2[0-4]|1\d|[1-9]|)\d)$)");
//    auto ipValidator = new QRegularExpressionValidator(ipRE, this);
//    ipLineEdit->setValidator(ipValidator);
//
//    QRegularExpression portRE("\\d+");
//    auto portValidator = new QRegularExpressionValidator(portRE, this);
//    portLineEdit->setValidator(portValidator);
//
//    layout->addWidget(ipLabel);
//    layout->addWidget(ipLineEdit);
//    layout->addWidget(portLabel);
//    layout->addWidget(portLineEdit);
//    layout->addWidget(autoStartCheckBox);
//    readConfig();
}
