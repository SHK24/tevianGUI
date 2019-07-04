#include "jsonparser.h"


jsonParser::jsonParser()
{

}

QString jsonParser::getToken(QByteArray replyBody)
{
    QJsonParseError parseError;

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    QMap<QString, QVariant> data = doc.toVariant().toMap()["data"].toMap();
    return data["access_token"].toString();
}

FaceDescription jsonParser::getFaceBox(QByteArray replyBody)
{
    QJsonParseError parseError;
    QList<FaceDescription> faceList;
    FaceDescription faceDesc;

    if(replyBody.count() == 0)
    {
        faceDesc.status = false;
        return faceDesc;
    }

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    if(parseError.error != 0)
    {
        faceDesc.status = false;
        return faceDesc;
    }

    QJsonObject obj = doc.object();

    QList<QVariant> data = obj["data"].toVariant().toList();

    QMap<QString, QVariant> faceBbox = data[0].toMap()["bbox"].toMap();
    QMap<QString, QVariant> age      = data[0].toMap()["demographics"].toMap()["age"].toMap();

    if(faceDesc.status = faceBbox["x"].toString() != "")      faceDesc.x = faceBbox["x"].toInt();

    if(faceDesc.status = faceBbox["y"].toString() != "")      faceDesc.y = faceBbox["y"].toInt();

    if(faceDesc.status = faceBbox["width"].toString() != "")  faceDesc.width = faceBbox["width"].toInt();

    if(faceDesc.status = faceBbox["height"].toString() != "") faceDesc.height = faceBbox["height"].toInt();

    faceDesc.age = age["mean"].toFloat();
    faceDesc.variance = age["variance"].toFloat();

    return faceDesc;
}
