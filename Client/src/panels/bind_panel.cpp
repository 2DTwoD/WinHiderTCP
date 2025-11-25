#include "panels/bind_panel.h"

#include <QHBoxLayout>
#include <QLabel>

BindPanel::BindPanel(QWidget *parent): QFrame(parent) {
    auto layout = new QHBoxLayout(this);
    this->setLayout(layout);

    auto winNameLabel = new QLabel("Win. name:", this);
    auto bindKeyLabel = new QLabel("Button:", this);
    winNameLineEdit = new QLineEdit("New folder", this);
    bindKeyButton = new QPushButton("Not binded", this);
    layout->addWidget(winNameLabel);
    layout->addWidget(winNameLineEdit);
    layout->addWidget(bindKeyLabel);
    layout->addWidget(bindKeyButton);
    QObject::connect(bindKeyButton, &QPushButton::clicked, this, &BindPanel::bindButtonAction);
}

BindPanel::~BindPanel() {
    qDebug("BindPanel: destructor");
}

void BindPanel::bindFinished(const QString& value) {
    if(value.isEmpty()) return;
    bindKeyButton->setText(value);
}

void BindPanel::bindButtonAction() {
    bindKeyButton->setText("binding...");
}

QPushButton *BindPanel::getBindButton() {
    return bindKeyButton;
}

void BindPanel::lock(bool lockFlag) {
    winNameLineEdit->setEnabled(!lockFlag);
    bindKeyButton->setEnabled(!lockFlag);
}

QString BindPanel::getQWinName() {
    return winNameLineEdit->text();
}

QString BindPanel::getQKey() {
    return bindKeyButton->text();
}

void BindPanel::setQWinName(const QString& value) {
    winNameLineEdit->setText(value);
}

void BindPanel::setQKey(const QString& value) {
    bindKeyButton->setText(value);
}
