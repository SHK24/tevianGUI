#ifndef TEVIANDLL_H
#define TEVIANDLL_H

#include <QObject>

#include "teviandll_global.h"

#define EMAIL 0
#define PASSWORD 1

#define DB_ID 0

#define TOKEN 0

#define IMAGE_PATH 1

#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QNetworkAccessManager>

struct FaceBox
{
    int x,y;
    int width, height;
};

class Request : public QObject
{
    Q_OBJECT

protected:
    QNetworkAccessManager * manager;
    QNetworkReply * reply;
    QTimer responseTimer;

    QMap<int, QString> errorMap;

public:
    Request();
    virtual void doRequest(QString url, QStringList requestParameters);
    virtual void processResponse();

    void responseTimeoutExpired();
    int getResponseStatus(QByteArray replyBody);
signals:
//    void requestSuccess();
    void requestError(QString errorMessage);
    void statusCode(int code);
    void rawJSON(QByteArray json);
};

class DetectRequest : public Request
{
    Q_OBJECT

    FaceBox getFaceBox(QByteArray replyBody);

public:
    DetectRequest();

    void doRequest(QString url, QStringList requestParameters);
    void processResponse();
signals:
    void cantOpenFile();
    void requestSuccess(FaceBox faceBox);
};

class LoginRequest : public Request
{
    Q_OBJECT

    QString getToken(QByteArray replyBody);
    QMap<int, QString> errorMap;
signals:
    void requestSuccess(QString token);

public:
    LoginRequest();
    void doRequest(QString url, QStringList requestParameters);
    void processResponse();
};

extern "C" class TEVIANDLLSHARED_EXPORT TevianDLL : public QObject
{
    Q_OBJECT

    LoginRequest * loginReq;
    DetectRequest * detectReq;

signals:
    void loginSuccess(QByteArray rawJSON);
    void detectSuccess(QByteArray rawJSON);
    void requestError(QString errorMessage);
public:
    TevianDLL();

    void doLogin(QString url, QString email, QString password);               //Выполнить login
    void detect(QString url, QString imagePath, QString token);               //Выполнить распознавание


};

#endif // TEVIANDLL_H
