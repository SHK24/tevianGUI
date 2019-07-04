#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addLogRecord("Приложение запущено!");

    loginDone = false;

    ///Подключение к сигналам о приеме данных
    connect(&tev, &TevianDLL::loginSuccess,  this, &MainWindow::loginSuccess);
    connect(&tev, &TevianDLL::requestError,  this, &MainWindow::requestError);
    connect(&tev, &TevianDLL::detectSuccess, this, &MainWindow::detectSuccess);

    ///Инициализация файла настроек
    settings = new QSettings("settings.ini", QSettings::IniFormat);


    ///Чтение текущих параметров из файла
    readSettings();

    ///Чтение URL
    readURLs();


    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::on_fileChoose_clicked()
{
    addLogRecord("Открыто окно выбора файлов...");

    ///Получение списка файлов для обработки из диалога выбора
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
            tr("Выбор файлов для обработки"), "",
            tr("JPEG (*.jpg);;"));

    addLogRecord("Файлов выбрано: " + QString::number(fileNames.count()));

    ///Сохранение списка выбранных изображений
    images = fileNames;

    ///Очистка виджета
    ui->files->clear();

    ///Заполнение виджета новыми значениями
    for(int i = 0; i < fileNames.count();i++)
    {
        ui->files->addItem(fileNames[i]);
    }
}

void MainWindow::addLogRecord(QString record)
{
    ui->log->append(QTime::currentTime().toString("<hh:mm:ss>") + record);
}

void MainWindow::responseTimeout()
{
    addLogRecord("Ошибка: время ожидания ответа превышено");
    responseTimer.stop();
}

void MainWindow::loginSuccess(QString token)
{
    loginDone = true;
    this->token = token;

    ///Сохранение токена в файл
    QFile credentials("credentials.txt");
    credentials.open(QIODevice::WriteOnly);
    credentials.write(token.toUtf8());
    credentials.close();

    ui->groupBox->setEnabled(true);

    addLogRecord("Вход выполнен!");
    responseTimer.stop();
}

void MainWindow::requestError(QString errorMessage)
{
    addLogRecord("Ошибка: " + errorMessage);
}

void MainWindow::detectSuccess(QByteArray rawJSON)
{
    addLogRecord("Распознавание выполнено");

    ///Отображение прогресса распознавания
    ui->progressBar->setValue(ui->progressBar->value() + 1);

    ///Вывод сырых данных (отладка)
    ui->faceData->setText(rawJSON);

    ///Вставка полученной информации в словарь
    info.insert(lastFile, parser.getFaceBox(rawJSON));

    ///Если есть еще изображения - послать следующий запрос
    if(images.count() > 0)
    {
        ///удаление самой верхней записи
        images.removeFirst();

        ///Запрос на обработку следующего изображения
        processNextImage();
    }
    responseTimer.stop();
}

void MainWindow::readSettings()
{
    settings->beginGroup("USER_DATA");
    ui->login->setText(settings->value("login").toString());
    ui->password->setText(settings->value("password").toString());
    settings->endGroup();
}

void MainWindow::readURLs()
{
    QFile urlFile("urls.txt");

    ///Попытка чтения URL из файла
    if(!urlFile.open(QIODevice::ReadOnly))
    {
        addLogRecord("Ошибка: файл с URL не найден. Будут использованы URL по умолчанию");
        this->urls.insert("LOGIN_URL", "https://backend.facecloud.tevian.ru/api/v1/login");
        this->urls.insert("DETECT_URL", "https://backend.facecloud.tevian.ru/api/v1/detect?demographics=true");
        return;
    }

    ///Чтение всех данных из фала и формирование списка URL
    QString urlBody = urlFile.readAll();
    QStringList urls = urlBody.split("\r\n");

    QStringList words;


    ///Проход по списку URL
    foreach(QString url, urls)
    {
        ///Формат записи URL в файле "назначение\tURL"
        words = url.split('\t');

        ///Если в строке меньше двух "слов" - неверный формат
        if(words.count() < 2)
        {
            addLogRecord("Ошибка: неправильный формат URL в файле");
            continue;
        }

        ///Если одно из слов пустое - неверный формат
        if((words[URL_PURPOSE] == "") || (words[URL_BODY] == ""))
        {
            addLogRecord("Ошибка: неправильный формат URL в файле");
            continue;
        }

        ///Добавление URL в список
        this->urls.insert(words[URL_PURPOSE], words[URL_BODY]);
    }

    addLogRecord("Файл с URL считан успешно!");

    return;
}

void MainWindow::processNextImage()
{
    ///Если список изображений не пуст - выслать запрос и завести таймер, при это запомнить имя последнего файла
    if(images.count() > 0)
    {
        tev.detect(urls.value("DETECT_URL"), images[0], token);
        responseTimer.start(10000);
        lastFile = images[0];
    }
    ///Если изображения закончились - имитировать клик на первую запись и выдать сообщение в лог
    else {
        ui->files->itemClicked(ui->files->item(0));
        addLogRecord("Обработка выполнена!");
    }
}

void MainWindow::on_doLogin_clicked()
{
    tev.doLogin(urls.value("LOGIN_URL"), ui->login->text(), ui->password->text());
    responseTimer.start(10000);
}

void MainWindow::on_processing_clicked()
{
    ///Если список выбранных изображений не пуст - начать обработку, иначе - сообщение об ошибке
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
    settings->beginGroup("USER_DATA");
    settings->setValue("login", ui->login->text());
    settings->endGroup();
}

void MainWindow::on_password_editingFinished()
{
    settings->beginGroup("USER_DATA");
    settings->setValue("password", ui->password->text());
    settings->endGroup();
}

///Чтение токена из файла
void MainWindow::on_readToken_clicked()
{
    QFile credentials("credentials.txt");
    credentials.open(QIODevice::ReadOnly);
    QString token = credentials.readAll();
    credentials.close();


    if(token.count() != 0)
    {
        this->token = token;
        ui->groupBox->setEnabled(true);

        addLogRecord("Токен обнаружен!");
    }
    else
    {
        addLogRecord("Токен не обнаружен!");
    }
}

///Обработчик нажатия на запись в списке файлов
void MainWindow::on_files_itemClicked(QListWidgetItem *item)
{
    ///Если нет сведений для изображения - ошибка, в ином случае - отобразить информацию на картинке
    if(!info.values(item->text()).count()) addLogRecord("Ошибка: Сведения для данного изображения недоступны");
    else {

        QString result = "";

        QPixmap pixmap(item->text());
        ui->widget->setImage(pixmap);

        QString infoText = "Age: " + QString::number(info[item->text()].age, 'f', 2);

        ui->widget->setRectangle(info[item->text()].x, info[item->text()].y, info[item->text()].width, info[item->text()].height, infoText);

        result += info[item->text()].toString();

        ui->faceData->setText(result);
    }
}
