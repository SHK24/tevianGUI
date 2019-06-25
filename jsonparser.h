#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <QVariant>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

struct FaceDescription
{
    bool status = false;
    int x,y;
    int width, height;
    float age;
    float variance;

    const QString toString()
    {
        QString descriptionStr = "";
        descriptionStr += "X:"        + QString::number(x) + "\r\n";
        descriptionStr += "Y:"        + QString::number(y) + "\r\n";
        descriptionStr += "Width:"    + QString::number(width) + "\r\n";
        descriptionStr += "Height:"   + QString::number(height) + "\r\n";
        descriptionStr += "Age:"      + QString::number(age,'f',2) + "\r\n";
        descriptionStr += "Variance:" + QString::number(variance,'f',2) + "\r\n";

        return descriptionStr;
    }
};

class jsonParser
{
public:
    jsonParser();

    QString getToken(QByteArray replyBody);
    FaceDescription getFaceBox(QByteArray replyBody);
};

#endif // JSONPARSER_H
