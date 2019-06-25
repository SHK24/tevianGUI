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

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    QJsonObject obj = doc.object();

    QList<QVariant> data = obj["data"].toVariant().toList();
//    QList<QVariant> faces= data["faces"].toList();
//    QList<FaceDescription> faceList;

    FaceDescription faceDesc;

    QMap<QString, QVariant> faceBbox = data[0].toMap()["bbox"].toMap();
    QMap<QString, QVariant> age      = data[0].toMap()["demographics"].toMap()["age"].toMap();

    if(faceDesc.status = faceBbox["x"].toString() != "")      faceDesc.x = faceBbox["x"].toInt();
    //if(!faceDesc.status) continue;

    if(faceDesc.status = faceBbox["y"].toString() != "")      faceDesc.y = faceBbox["y"].toInt();
    //if(!faceDesc.status) continue;

    if(faceDesc.status = faceBbox["width"].toString() != "")  faceDesc.width = faceBbox["width"].toInt();
    //if(!faceDesc.status) continue;

    if(faceDesc.status = faceBbox["height"].toString() != "") faceDesc.height = faceBbox["height"].toInt();
    //if(!faceDesc.status) continue;

    faceDesc.age = age["mean"].toFloat();
    faceDesc.variance = age["variance"].toFloat();

//    faceList.append(faceDesc);

//    foreach(QVariant face, faces)
//    {
//        QMap<QString, QVariant> faceBbox = face.toMap()["bbox"].toMap();
//        QMap<QString, QVariant> age = face.toMap()["demographics"].toMap()["age"].toMap();

//        if(faceDesc.status = faceBbox["x"].toString() != "")      faceDesc.x = faceBbox["x"].toInt();
//        if(!faceDesc.status) continue;

//        if(faceDesc.status = faceBbox["y"].toString() != "")      faceDesc.y = faceBbox["y"].toInt();
//        if(!faceDesc.status) continue;

//        if(faceDesc.status = faceBbox["width"].toString() != "")  faceDesc.width = faceBbox["width"].toInt();
//        if(!faceDesc.status) continue;

//        if(faceDesc.status = faceBbox["height"].toString() != "") faceDesc.height = faceBbox["height"].toInt();
//        if(!faceDesc.status) continue;

//        faceDesc.age = age["mean"].toFloat();
//        faceDesc.variance = age["variance"].toFloat();

//        faceList.append(faceDesc);
//    }

    return faceDesc;
}
