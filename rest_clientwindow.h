#ifndef REST_CLIENTWINDOW_H
#define REST_CLIENTWINDOW_H

#include <QMainWindow>
#include "rest_client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class REST_ClientWindow;
}
QT_END_NAMESPACE

class REST_ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    REST_ClientWindow(QWidget *parent = nullptr);
    ~REST_ClientWindow();

private:
    Ui::REST_ClientWindow *ui;
    REST_Client *m_restClient;
};
#endif // REST_CLIENTWINDOW_H
