#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QTime>
#include <QListWidgetItem>
#include "../../DLL/teviandll.h"
#include <QSettings>
#include "jsonparser.h"

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
    void on_fileChoose_clicked();

    void on_files_itemClicked(QListWidgetItem *item);

    void on_doLogin_clicked();

    void on_processing_clicked();

    void on_login_editingFinished();

    void on_password_editingFinished();

    void on_pushButton_clicked();

    void on_readToken_clicked();

private:
    Ui::MainWindow *ui;
    void addLogRecord(QString record);

    bool loginDone;
    TevianDLL tev;
    QString token;
    QStringList images;
    QMap<QString, FaceDescription> info;
    QMap<QString, QString> urls;
    QString lastFile;
    QSettings * settings;
    jsonParser parser;

    void loginSuccess(QString token);
    void requestError(QString errorMessage);
    void detectSuccess(QByteArray rawJSON);
    void readSettings();
    void processNextImage();
};

#endif // MAINWINDOW_H
