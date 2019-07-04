#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <QVariant>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

///Структура описывающая параметры лица найденного на изображении
struct FaceDescription
{
    ///Признак действительности данных
    bool status = false;

    ///Координаты x и y левого верхнего угла прямоугольника
    int x,y;

    ///Ширина и высота прямоугольника
    int width, height;

    ///Возраст
    float age;

    ///СКО возраста
    float variance;

    ///Метод преобразования данных структуры в строку
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
    ///Конструктор
    jsonParser();

    ///Функция выделения токена из ответа
    QString getToken(QByteArray replyBody);

    ///Функция выделения параметров лица из ответа
    FaceDescription getFaceBox(QByteArray replyBody);
};

#endif // JSONPARSER_H
