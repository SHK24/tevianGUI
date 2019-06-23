#ifndef TEVIANEXHANGER_H
#define TEVIANEXHANGER_H

#include <QObject>
#include "teviandll.h"
#include <iostream>

using namespace std;

class TevianExchanger : public QObject
{
    Q_OBJECT

    TevianDLL tev;

public:
    TevianExchanger();
    void requestError(QString errorMessage);
    void loginSuccess(QString token);
    void detectSuccess(QByteArray rawJSON);

    void doLogin(QString url, QString email, QString password);               //Выполнить login
    void detect(QString url, QString imagePath, QString token);               //Выполнить распознавание
};

#endif // TEVIANEXHANGER_H

