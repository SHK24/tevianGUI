#ifndef REQUEST_H
#define REQUEST_H

#define EMAIL 0
#define PASSWORD 1

#define DB_ID 0

#define TOKEN 0

#define IMAGE_PATH 1

#include <QUrl>
#include <QTimer>
#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QNetworkAccessManager>

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

#endif // REQUEST_H
