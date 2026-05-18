#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
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
                               const QByteArray &postData = QByteArray()){
        outputEdit->clear();
        errorsEdit->clear();
        QUrl url(userURL);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setTransferTimeout(30000);

        QNetworkReply *reply = nullptr;

        switch(method) {
        case HttpMethod::GET:
            reply = manager_->get(request);
            break;
        case HttpMethod::POST:
            reply = manager_->post(request, postData);
            break;
        case HttpMethod::PUT:
            reply = manager_->put(request, postData);
            break;
        case HttpMethod::DELETE:
            reply = manager_->deleteResource(request);
            break;
        }

        connect(reply, &QNetworkReply::finished, this, [this, reply, outputEdit, errorsEdit](){
            if (!this or !manager_) return;
            if (reply->error() != QNetworkReply::NoError) return;
            QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            if (contentType.contains("application/json")) {
                QByteArray response = reply->readAll();

                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
                jsonString.replace("\\n", "\n");
                outputEdit->setText(jsonString);

                qDebug() << "Successfully received, type: JSON";

            } else if (contentType.contains("application/json")) {
                QByteArray response = reply->readAll();
                QTextDocument *htmlDoc; htmlDoc->setHtml(response);
                outputEdit->setDocument(htmlDoc);

                qDebug() << "Successfully received, type: HTML";

            } else if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!= 200){
                //int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                QString httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
                QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
                errorsEdit->setText(getDetailedError(reply->error()));
                errorsEdit->append(httpCode);
                errorsEdit->insertPlainText(reason);
                qDebug() << "Network error:" << getDetailedError(reply->error());
                qDebug() << "HTTP status:" << httpCode << reason;
            }else errorsEdit->setText("Nothing received");

            reply->deleteLater();
        });
    }
private:
    QNetworkAccessManager *manager_;
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
