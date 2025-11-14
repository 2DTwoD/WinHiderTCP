#include "token.h"

Token::Token(): isValid(true), key(WM_NULL), wname("") {
}

Token::Token(Token &tokenObject): isValid(tokenObject.isValid), key(tokenObject.key),
                                  wname(tokenObject.wname){
}

Token::Token(Token &&tokenObject) noexcept: isValid(tokenObject.isValid), key(tokenObject.key),
                                            wname(std::move(tokenObject.wname)) {
}

Token &Token::operator=(Token &tokenObject) {
    isValid = tokenObject.isValid;
    key = tokenObject.key;
    wname = tokenObject.wname;
    return *this;
}

Token &Token::operator=(Token &&tokenObject) {
    isValid = tokenObject.isValid;
    key = tokenObject.key;
    wname = std::move(tokenObject.wname);
    return *this;
}