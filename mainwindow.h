#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QTime>
#include <QListWidgetItem>
#include "teviandll.h"
#include <QSettings>
#include "jsonparser.h"

#define URL_PURPOSE 0
#define URL_BODY 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    ///Слот обработки нажатия на кнопку выбора файлов
    void on_fileChoose_clicked();

    ///Слот обработки нажатия на кнопку входа
    void on_doLogin_clicked();

    ///Слот обработки нажатия на кнопку обработки
    void on_processing_clicked();

    ///Слот обработки события завершения редактирования строки имени пользователя
    void on_login_editingFinished();

    ///Слот обработки события завершения редактирования строки пароля
    void on_password_editingFinished();

    ///Слот обработки события завершения редактирования чтения токена
    void on_readToken_clicked();

    ///Слот обработки события выбора файла из списка
    void on_files_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    ///Метод добавления записи в журнал
    void addLogRecord(QString record);

    ///Флаг выполнения авторизации
    bool loginDone;

    ///Экземпляр для обмена данными с сервером
    TevianDLL tev;

    ///Токен пользователя
    QString token;

    ///Список изображений для обработки
    QStringList images;

    ///Словарь для хранения информации об изображении
    QMap<QString, FaceDescription> info;

    ///URLы для выполнения запросов
    QMap<QString, QString> urls;

    ///Имя последнего обработанного файла
    QString lastFile;

    ///Файл хранения настроек
    QSettings * settings;

    ///Экземпляр разборщика JSON данных
    jsonParser parser;

    ///Таймер ожидания ответа
    QTimer responseTimer;

    ///Обработчик таймаута ответа
    void responseTimeout();

    ///Метод обработки сигнала об успешной авторизации
    void loginSuccess(QString token);

    ///Метод обработки сигнала об ошибке запроса
    void requestError(QString errorMessage);

    ///Метод обработки сигнала об успешной обработке изображения
    void detectSuccess(QByteArray rawJSON);

    ///Метод чтения настроек из файла
    void readSettings();

    ///Метод чтения URL из файла
    void readURLs();

    ///Метод для выполнения запроса обработки следующего изображения
    void processNextImage();
};

#endif // MAINWINDOW_H
