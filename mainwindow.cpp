#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addLogRecord("Приложение запущено!");

    loginDone = false;
    urlExist = false;

    connect(&tev, &TevianDLL::loginSuccess,  this, &MainWindow::loginSuccess);
    connect(&tev, &TevianDLL::requestError,  this, &MainWindow::requestError);
    connect(&tev, &TevianDLL::detectSuccess, this, &MainWindow::detectSuccess);

    settings = new QSettings("settings.ini");

    readSettings();
    urlExist = readURLs();
    //ui->widget->setRectangle(100,100,200,200,"Тестовый текст");

    this->showMaximized();
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

    QFile credentials("credentials.txt");
    credentials.open(QIODevice::WriteOnly);
    credentials.write(token.toUtf8());
    credentials.close();

    ui->groupBox->setEnabled(true);

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

bool MainWindow::readURLs()
{
    QFile urlFile("urls.txt");

    if(!urlFile.open(QIODevice::ReadOnly))
    {
        addLogRecord("Ошибка: файл с URL не найден. Будут использованы URL по умолчанию");
        this->urls.insert("LOGIN_URL", "https://backend.facecloud.tevian.ru/api/v1/login");
        this->urls.insert("DETECT_URL", "https://backend.facecloud.tevian.ru/api/v1/detect?demographics=true");
        return false;
    }

    QString urlBody = urlFile.readAll();
    QStringList urls = urlBody.split("\r\n");

    QStringList words;

    foreach(QString url, urls)
    {
        words = url.split('\t');

        if(words.count() < 2)
        {
            addLogRecord("Ошибка: неправильный формат URL в файле");
            continue;
        }

        if((words[URL_PURPOSE] == "") || (words[URL_BODY] == ""))
        {
            addLogRecord("Ошибка: неправильный формат URL в файле");
            continue;
        }

        this->urls.insert(words[URL_PURPOSE], words[URL_BODY]);
    }

    addLogRecord("Файл с URL считан успешно!");

    return true;
}

void MainWindow::processNextImage()
{
    if(images.count() > 0)
    {
        tev.detect(urls.value("DETECT_URL"), images[0], token);
        lastFile = images[0];
    }
    else {
        addLogRecord("Обработка выполнена!");
    }
}

void MainWindow::on_files_itemClicked(QListWidgetItem *item)
{
    if(!info.values(item->text()).count()) addLogRecord("Ошибка: Сведения для данного изображения недоступны");
    else {

        QString result = "";

        QPixmap pixmap(item->text());
        ui->widget->setImage(pixmap);

        QString infoText = "Age: " + QString::number(info[item->text()].age, 'f', 2);

        ui->widget->setRectangle(info[item->text()].x, info[item->text()].y, info[item->text()].width, info[item->text()].height, infoText);

        result += info[item->text()].toString();
        //    faceNumber++;
        //}

        ui->faceData->setText(result);
    }
}

void MainWindow::on_doLogin_clicked()
{
    tev.doLogin(urls.value("LOGIN_URL"), ui->login->text(), ui->password->text());
}

void MainWindow::on_processing_clicked()
{
    if(images.count() > 0)
    {
        ui->progressBar->setValue(0);
        ui->progressBar->setMaximum(images.count());

        processNextImage();
    }
    else {
        addLogRecord("Ошибка: изображения не выбраны!");
    }
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


void MainWindow::on_readToken_clicked()
{
    QFile credentials("credentials.txt");
    credentials.open(QIODevice::ReadOnly);
    QString token = credentials.readAll();
    credentials.close();


    if(token.count() != 0)
    {
        this->token = token;
        if(urlExist)ui->groupBox->setEnabled(true);

        addLogRecord("Токен обнаружен!");
    }
    else
    {
        addLogRecord("Токен не обнаружен!");
    }
}
