#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
//#include <QPlainTextDocumentLayout>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QTextEdit>
using namespace Qt::StringLiterals;
enum class HttpMethod { GET, POST, PUT, DELETE };

class REST_Client : public QObject
{
    Q_OBJECT
public:
    explicit REST_Client(QObject *parent = nullptr);
    //~REST_Client(){delete manager_;}

    void outputJsonFromUrl(const QString &userURL, QTextEdit *outputEdit, QTextEdit *errorsEdit,
                           HttpMethod method = HttpMethod::GET,
                           const QByteArray &postData = QByteArray())
    {
        switch (method) {
        case HttpMethod::GET:
            handleRequest_Get(userURL, outputEdit, errorsEdit);
            break;

        case HttpMethod::POST:
            handleRequest_Post(userURL, outputEdit, errorsEdit, postData);
            break;

        case HttpMethod::PUT:
            handleRequest_Put(userURL, outputEdit, errorsEdit, postData);
            break;

        case HttpMethod::DELETE:
            handleRequest_Delete(userURL, outputEdit, errorsEdit);
            break;
        }
    }

private:
    QNetworkAccessManager *manager_;
    void handleRequest_Get   (const QString &userURL, QTextEdit *outputEdit, QTextEdit *errorsEdit){
        if (!manager_) return;
        outputEdit->clear();
        errorsEdit->clear();
        QUrl url(userURL);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setTransferTimeout(10000);

        QNetworkReply *reply = nullptr;
        reply = manager_->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, outputEdit, errorsEdit](){
            if (!manager_) return;
            QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            // БЛОК ОШИБОК
            if (reply->error() != QNetworkReply::NoError or reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!= 200){ //Код успеха
            QString httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
            QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            QString errorMsg = getDetailedError(reply->error()) + "\n" +
                               httpCode + " " + reason;

            errorsEdit->setText(errorMsg);
            qDebug() << "Network error:" << getDetailedError(reply->error());
            qDebug() << "HTTP status:" << httpCode << reason;

            // БЛОК JSON
            } else if (contentType.contains("application/json")) {
                QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
                QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
                jsonString.replace("\\n", "\n");

                outputEdit->setText(jsonString);
                errorsEdit->setText("Response type: JSON");
                qDebug() << "Successfully received, type: JSON";

            // БЛОК HTML
            } else if (contentType.contains("text/html")) {
                auto *htmlDoc = new QTextDocument(outputEdit);
                htmlDoc->setHtml(reply->readAll());
                //htmlDoc->setDocumentLayout(new QPlainTextDocumentLayout(htmlDoc));
                htmlDoc->clearUndoRedoStacks();

                outputEdit->setDocument(htmlDoc);
                errorsEdit->setText("Response type: HTML");
                qDebug() << "Successfully received, type: HTML";

            } else errorsEdit->setText("Unknown response format");

            reply->deleteLater();
        });
    }
    void handleRequest_Post  (const QString &url, QTextEdit *output, QTextEdit *errors, const QByteArray &postData = QByteArray()){}
    void handleRequest_Put   (const QString &url, QTextEdit *output, QTextEdit *errors, const QByteArray &postData = QByteArray()){}
    void handleRequest_Delete(const QString &url, QTextEdit *output, QTextEdit *errors){}
    QString getDetailedError(QNetworkReply::NetworkError code) {
        switch(code) {
        case QNetworkReply::ConnectionRefusedError:
            return "Connection refused - сервер не слушает порт";
        case QNetworkReply::RemoteHostClosedError:
            return "Remote host closed connection - сервер оборвал соединение";
        case QNetworkReply::HostNotFoundError:
            return "Host not found - DNS не может разрешить имя";
        case QNetworkReply::TimeoutError:
            return "Timeout - нет ответа за отведённое время";
        case QNetworkReply::OperationCanceledError:
            return "Operation canceled - запрос отменён";
        case QNetworkReply::SslHandshakeFailedError:
            return "SSL handshake failed - проблемы с сертификатами";
        case QNetworkReply::TemporaryNetworkFailureError:
            return "Temporary network failure - сеть недоступна";
        case QNetworkReply::NetworkSessionFailedError:
            return "Network session failed - нет интернет-соединения";
        case QNetworkReply::BackgroundRequestNotAllowedError:
            return "Background request not allowed - ограничения ОС";
        case QNetworkReply::TooManyRedirectsError:
            return "Too many redirects - зацикливание редиректов";
        case QNetworkReply::InsecureRedirectError:
            return "Insecure redirect - редирект с HTTPS на HTTP";
        case QNetworkReply::ProxyConnectionRefusedError:
            return "Proxy connection refused";
        case QNetworkReply::ProxyConnectionClosedError:
            return "Proxy connection closed";
        case QNetworkReply::ProxyNotFoundError:
            return "Proxy not found";
        case QNetworkReply::ProxyTimeoutError:
            return "Proxy timeout";
        case QNetworkReply::ProxyAuthenticationRequiredError:
            return "Proxy authentication required";
        case QNetworkReply::ContentAccessDenied:
            return "Content access denied - 403";
        case QNetworkReply::ContentOperationNotPermittedError:
            return "Content operation not permitted";
        case QNetworkReply::ContentNotFoundError:
            return "Content not found - 404";
        case QNetworkReply::AuthenticationRequiredError:
            return "Authentication required - 401";
        default:
            return QString("Unknown error (code %1)").arg(code);
        }
    }
};

#endif // REST_CLIENT_H
/*
        if (method == "GET") {
            reply = manager_->get(request);
        } else if (method == "POST") {
            reply = manager_->post(request, postData);
        } else if (method == "PUT") {
            reply = manager_->put(request, postData);
        } else if (method == "DELETE") {
            reply = manager_->deleteResource(request);
        } else return;
signals:
    // Отправляет готовый JSON-объект наружу
    void dataReceived(const QJsonObject &json);
    // Дополнительный сигнал для уведомления об ошибках
    void errorOccurred(const QString &errorStr);
private slots:
    void onResult(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            // Парсим полученный JSON
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject json = jsonDoc.object();
            emit dataReceived(json);
            qDebug() << "Successfully received:" << json["body"].toString();
        } else {
            emit errorOccurred(reply->errorString());
            qDebug() << "Network error:" << reply->errorString();
        }

        reply->deleteLater();
        //QCoreApplication::quit(); // Завершаем приложение (для примера)
    }*/
