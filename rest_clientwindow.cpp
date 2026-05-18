#include "rest_clientwindow.h"
#include "./ui_rest_clientwindow.h"

REST_ClientWindow::REST_ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::REST_ClientWindow)
{
    ui->setupUi(this);
    m_restClient = new REST_Client(this);
    QString testURL = "https://jsonplaceholder.typicode.com/";
    connect(ui->Submit_PB, &QPushButton::clicked, this, [this]()
            {m_restClient->outputJsonFromUrl(ui->Getter_URL_LE->text(),
             ui->Receiving_TE, ui->Print_Error_TE, HttpMethod::GET);});
}

REST_ClientWindow::~REST_ClientWindow()
{
    delete ui;
}
/*
#include <QJsonDocument> // Нужен для красивого вывода JSON в текст
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 1. Создаем объект нашего REST-клиента в памяти
    m_restClient = new RestClient(this);

    // 2. Соединяем сигналы клика по кнопке со слотом отправки
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);

    // 3. Соединяем сигналы REST-клиента с обработчиками в интерфейсе
    connect(m_restClient, &RestClient::dataReceived, this, &MainWindow::handleNetworkData);
    connect(m_restClient, &RestClient::errorOccurred, this, &MainWindow::handleNetworkError);
}

MainWindow::~MainWindow() {
    delete ui;
    // m_restClient удалится автоматически, так как мы передали (this) в конструктор
}

// Вызывается при нажатии на кнопку
void MainWindow::onSendButtonClicked() {
    ui->logTextEdit->append("Отправка запроса...");

    // Готовим тестовые данные для отправки на сервер
    QJsonObject jsonPayload;
    jsonPayload["title"] = "Новый пост";
    jsonPayload["body"] = "Текст нашего сообщения";
    jsonPayload["userId"] = 1;

    // Вызываем метод отправки запроса
    QString url = "https://typicode.com";
    m_restClient->sendPostRequest(url, jsonPayload);
}

// Вызывается автоматически, когда пришел успешный ответ от сервера
void MainWindow::handleNetworkData(const QJsonObject &json) {
    // Превращаем JSON в красивую строку с отступами
    QJsonDocument doc(json);
    QString prettyJson = doc.toJson(QJsonDocument::Indented);

    // Выводим текст в интерфейс
    ui->logTextEdit->append("Данные успешно получены:");
    ui->logTextEdit->append(prettyJson);
}

// Вызывается автоматически, если произошла ошибка сети или парсинга
void MainWindow::handleNetworkError(const QString &error) {
    ui->logTextEdit->append("Произошла ошибка: " + error);
}
*/
