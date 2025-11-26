#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <QMutex>
#include <winsock2.h>


class Token{
private:
    QMutex mutex;
    bool valid;
    QString key;
    QString wname;
public:
    Token();

    virtual ~Token();

    explicit Token(bool isValid);
    Token(Token& tokenObject);
    Token(Token&& tokenObject) noexcept;
    Token& operator=(const Token& tokenObject);
    Token& operator=(Token&& tokenObject) noexcept ;
    void setValid(bool value);
    void setKey(QString&& value);
    void setWName(QString&& value);
    bool isValid();
    QString getKey();
    QString getName();
    void print();
};

#endif //TOKEN_H
