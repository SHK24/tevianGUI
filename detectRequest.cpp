#include "detectrequest.h"

DetectRequest::DetectRequest()
{
    manager = new QNetworkAccessManager(this);
}

void DetectRequest::doRequest(QString url, QStringList requestParameters)
{
    QUrl urlt(url);
    QNetworkRequest request(url);

    QString bearer = "Bearer " + requestParameters[TOKEN];

    QNetworkReply * reply;

    request.setRawHeader("Content-Type", "image/jpeg");
    request.setRawHeader("Authorization", bearer.toUtf8());


    QFile imageFile(requestParameters[IMAGE_PATH]);

    if(!imageFile.open(QIODevice::ReadOnly))
    {
        cantOpenFile();
        return;
    }

    QByteArray imageData = imageFile.readAll();

    request.setRawHeader("Content-Length", QString::number(imageFile.size()).toUtf8());
    reply = manager->post(request, imageData);

    connect(reply, &QNetworkReply::finished,this, &DetectRequest::processResponse);
    connect(&responseTimer, &QTimer::timeout, this, &Request::responseTimeoutExpired);

    responseTimer.start(3000);
}

FaceBox DetectRequest::getFaceBox(QByteArray replyBody)
{
    QJsonParseError parseError;

    QJsonDocument doc = QJsonDocument::fromJson(replyBody, &parseError);

    QJsonObject obj = doc.object();

    QMap<QString, QVariant> jsonReply = doc.toVariant().toMap();

    QMap<QString, QVariant> data = doc.toVariant().toMap()["data"].toMap();
    QMap<QString, QVariant> face_bbox = data["face_bbox"].toMap();

    FaceBox faceBox;

    faceBox.x      = face_bbox["x"].toInt();
    faceBox.y      = face_bbox["y"].toInt();
    faceBox.height = face_bbox["height"].toInt();
    faceBox.width  = face_bbox["width"].toInt();

    return faceBox;
}

void DetectRequest::processResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray replyBody = reply->readAll();

    int status_code = getResponseStatus(replyBody);

    statusCode(status_code);


    if(status_code != 200)
    {
        (errorMap.value(status_code));
        return;
    }
    rawJSON(replyBody);
    //requestSuccess(getFaceBox(replyBody));
}
