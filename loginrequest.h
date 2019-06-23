#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include "request.h"

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



#endif // LOGINREQUEST_H
