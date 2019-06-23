#ifndef TEVIANDLL_H
#define TEVIANDLL_H

#include <QObject>
#include "loginrequest.h"
#include "detectRequest.h"
#include "teviandll_global.h"

extern "C" class TEVIANDLLSHARED_EXPORT TevianDLL : public QObject
{
    Q_OBJECT

    LoginRequest * loginReq;
    DetectRequest * detectReq;

signals:
    void loginSuccess(QString token);
    void detectSuccess(QByteArray rawJSON);
    void requestError(QString errorMessage);
public:
    TevianDLL();

    void doLogin(QString url, QString email, QString password);               //Выполнить login
    void detect(QString url, QString imagePath, QString token);               //Выполнить распознавание


};

#endif // TEVIANDLL_H
