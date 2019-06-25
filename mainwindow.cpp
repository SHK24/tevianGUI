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

    settings = new QSettings("settings.ini");
    readSettings();
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
    ui->progressBar->setValue(ui->progressBar->value() + 1);

    ui->faceData->setText(rawJSON);
    info.insert(lastFile, parser.getFaceBox(rawJSON));

//    QFile data("json.txt");
//    data.open(QIODevice::WriteOnly);
//    data.write(rawJSON);
//    data.close();

    if(images.count() > 0)
    {
        images.removeFirst();
        processNextImage();
    }
}

void MainWindow::readSettings()
{
    ui->login->setText(settings->value("login").toString());
    ui->password->setText(settings->value("password").toString());
}

void MainWindow::processNextImage()
{
    if(images.count() > 0)
    {
        tev.detect("https://backend.facecloud.tevian.ru/api/v1/detect?demographics=true", images[0], token);
        lastFile = images[0];
    }
}

void MainWindow::on_files_itemClicked(QListWidgetItem *item)
{
    if(0) addLogRecord("Ошибка: Сведения для данного изображения недоступны");
    else {

        QString result = "";
        int faceNumber = 1;
        //foreach(FaceDescription infoItem, info.value(item->text()))
        //{
        //    result += "------" + QString(faceNumber) + "------";
        result += info[item->text()].toString();
        //    faceNumber++;
        //}

        ui->faceData->setText(result);
    }
}

void MainWindow::on_doLogin_clicked()
{
    tev.doLogin("https://backend.facecloud.tevian.ru/api/v1/login", ui->login->text(), ui->password->text());
}

void MainWindow::on_processing_clicked()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(images.count());

    processNextImage();
}

void MainWindow::on_login_editingFinished()
{
    settings->setValue("login", ui->login->text());
}

void MainWindow::on_password_editingFinished()
{
    settings->setValue("password", ui->login->text());
}

void MainWindow::on_pushButton_clicked()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}
