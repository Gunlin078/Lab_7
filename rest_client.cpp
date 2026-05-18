#include "rest_client.h"

REST_Client::REST_Client(QObject *parent)
    : QObject{parent}
    , manager_(new QNetworkAccessManager(this))
{
    /*
    connect(manager_, &QNetworkAccessManager::finished,
            this, &REST_Client::onResult);

    QUrl url("https://jsonplaceholder.typicode.com/posts/1");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager_->get(request);*/
}
