#ifndef DETECTREQUEST_H
#define DETECTREQUEST_H

#include <QFile>
#include "request.h"

struct FaceBox
{
    int x,y;
    int width, height;
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

#endif // DETECTREQUEST_H
