#include "request.h"

void Request::doRequest(QString url, QStringList requestParameters)
{

}

void Request::processResponse()
{

}

int Request::getResponseStatus(QByteArray replyBody)
{
    QJsonParseError parseError;

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    QJsonObject obj = doc.object();

    QMap<QString, QVariant> jsonReply = doc.toVariant().toMap();

    return jsonReply["status_code"].toInt();
}

void Request::responseTimeoutExpired()
{

}

Request::Request()
{
    errorMap.insert(400, "error detected in request");
    errorMap.insert(401, "authorization with valid credentials required");
    errorMap.insert(404, "page not found");

}
