#include "loginrequest.h"

LoginRequest::LoginRequest()
{
    errorMap.insert(400, "error detected in request");
    errorMap.insert(401, "invalid email or password provided");

    manager = new QNetworkAccessManager(this);
}

void LoginRequest::doRequest(QString url, QStringList requestParameters)
{
    QUrl urlt(url);
    QNetworkRequest request(urlt);
    request.setRawHeader("Content-Type", "application/json");

    QString payload = "{\"email\":\"%email%\",\"password\":\"%password%\"}";

    payload = payload.replace("%email%",    requestParameters[EMAIL]);
    payload = payload.replace("%password%", requestParameters[PASSWORD]);

    reply = manager->post(request,payload.toUtf8());

    connect(reply, &QNetworkReply::finished,this, &LoginRequest::processResponse);
    connect(&responseTimer, &QTimer::timeout, this, &Request::responseTimeoutExpired);

    responseTimer.start(3000);
}

void LoginRequest::processResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray replyBody = reply->readAll();

    int status_code = getResponseStatus(replyBody);

    statusCode(status_code);
    rawJSON(replyBody);

    if(status_code != 200)
    {
        requestError(errorMap.value(status_code));
        return;
    }

    requestSuccess(getToken(replyBody));
}

QString LoginRequest::getToken(QByteArray replyBody)
{
    QJsonParseError parseError;

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    QMap<QString, QVariant> data = doc.toVariant().toMap()["data"].toMap();
    return data["access_token"].toString();
}
