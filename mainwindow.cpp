#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addLogRecord("Приложение запущено!");

    loginDone = false;

    connect(&tev, &TevianDLL::loginSuccess,  this, &MainWindow::loginSuccess);
    connect(&tev, &TevianDLL::requestError,  this, &MainWindow::requestError);
    connect(&tev, &TevianDLL::detectSuccess, this, &MainWindow::detectSuccess);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileChoose_clicked()
{
    addLogRecord("Открыто окно выбора файлов...");

    QStringList fileNames = QFileDialog::getOpenFileNames(this,
            tr("Выбор файлов для обработки"), "",
            tr("JPEG (*.jpg);;"));

    int x = 0;

    addLogRecord("Файлов выбрано: " + QString::number(fileNames.count()));

    images = fileNames;
    for(int i = 0; i < fileNames.count();i++)
    {
        ui->files->addItem(fileNames[i]);
    }
}

void MainWindow::addLogRecord(QString record)
{
    ui->log->append(QTime::currentTime().toString("<hh:mm:ss>") + record);
}

void MainWindow::loginSuccess(QString token)
{
    loginDone = true;
    this->token = token;
    addLogRecord("Вход выполнен!");
}

void MainWindow::requestError(QString errorMessage)
{
    addLogRecord("Ошибка: " + errorMessage);
}

void MainWindow::detectSuccess(QByteArray rawJSON)
{
    addLogRecord("Распознавание выполнено");

    ui->faceData->setText(rawJSON);

    if(images.count() > 0)
    {
        images.removeFirst();
        on_processing_clicked();
    }
}

void MainWindow::on_files_itemClicked(QListWidgetItem *item)
{
    addLogRecord("Ошибка: Сведения для данного изображения недоступны");
}

void MainWindow::on_doLogin_clicked()
{
    tev.doLogin("https://backend.facecloud.tevian.ru/api/v1/login", ui->login->text(), ui->password->text());
}

void MainWindow::on_processing_clicked()
{
    if(images.count() > 0)
        tev.detect("https://backend.facecloud.tevian.ru/api/v1/detect?demographics=true", images[0], token);
}
