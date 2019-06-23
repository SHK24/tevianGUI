#include "tevianexchanger.h"


TevianExchanger::TevianExchanger()
{
    connect(&tev, &TevianDLL::loginSuccess,  this, &TevianExchanger::loginSuccess);
    connect(&tev, &TevianDLL::requestError,  this, &TevianExchanger::requestError);
    connect(&tev, &TevianDLL::detectSuccess, this, &TevianExchanger::detectSuccess);
}

void TevianExchanger::requestError(QString errorMessage)
{
    cout <<errorMessage.toUtf8().data() << endl;
}

void TevianExchanger::loginSuccess(QString token)
{
    cout<<"---------Login result------"<<endl;
    cout<<"Login is OK! Your JWT token has been storaged in credentials.txt" << endl;
    QFile credentials("credentials.txt");

    if(!credentials.open(QIODevice::WriteOnly))
    {
        cout<<"Can't create credentials file!"<<endl;
        return;
    }

    credentials.write(token.toUtf8());
    credentials.close();
}

void TevianExchanger::detectSuccess(QByteArray rawJSON)
{
    cout<<rawJSON.data()<<endl;
}

void TevianExchanger::doLogin(QString url, QString email, QString password)
{
    tev.doLogin(url, email, password);
}

void TevianExchanger::detect(QString url, QString imagePath, QString token)
{
    tev.detect(url, imagePath, token);
}
