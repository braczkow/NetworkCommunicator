#include "mainwindow.h"
#include "ui_mainwindow.h"

///#include <QThread>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QTimer>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_messagesReceived(0)
{
    ui->setupUi(this);

//    m_workerThread = new NetworkWorkerThread();
//    connect(m_workerThread, SIGNAL(newMessage(QString)),
//                              SLOT(on_newMessage(QString)));

    qDebug("In the main thread: %d", (unsigned int) QThread::currentThreadId());

    //m_workerThread->start();

    m_NAManager = new QNetworkAccessManager(this);
    connect(m_NAManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_finished(QNetworkReply*)));

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timerExceeded()));

    on_timerExceeded();

    m_timer->start(2000);

}

MainWindow::~MainWindow()
{
    delete ui;

//    m_workerThread->doFinish();

//    m_workerThread->wait();

    //delete m_workerThread;
}

void MainWindow::on_timerExceeded()
{
    qDebug("MainWindow::on_timerExceeded");

    QNetworkRequest req;
    req.setUrl(QUrl("http://localhost:8080/messageCount"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject jReq;
    QJsonArray jUsers;
    jUsers.append("braczkow");
    jUsers.append("marsza");

    jReq["users"] = jUsers;

    QJsonDocument doc(jReq);
    std::string json = doc.toJson().toStdString();

    qDebug("about to POST: %s", json.c_str());
    m_NAManager->post(req, json.c_str());

}

void MainWindow::on_finished(QNetworkReply* reply)
{
    //qDebug("MainWindow::on_finished");

    QByteArray data = reply->readAll();

    std::string replyContent = data.toStdString();
    qDebug("MainWindow::on_finished reply: %s", replyContent.c_str());

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    QString replyType = json["type"].toString();
    qDebug("MainWindow::on_finished type = %s", replyType.toStdString().c_str());

    if (replyType == "messageCount")
    {
        unsigned int currentMessagesCount = json["count"].toInt();
        qDebug("MainWindow::on_finished count: %d", currentMessagesCount);
        if (currentMessagesCount > m_messagesReceived)
        {
            m_getLastMessages(currentMessagesCount - m_messagesReceived);
        }
    }
    else if (replyType == "getLastMessages")
    {
        qDebug("on_finished type: %d", json["messages"].type());

        QJsonArray messages = json["messages"].toArray();
        m_updateMessages(messages);
    }


}

void MainWindow::m_updateMessages(QJsonArray messages)
{
    qDebug("MainWindow::m_updateMessages");

    unsigned int count = messages.count();
    m_messagesReceived += count;

    qDebug("MainWindow::m_updateMessages count: %d", count);

    QJsonArray::iterator iter = messages.end();
    for ( ; iter >= messages.begin(); --iter)
    {
        qDebug("m_updateMessages isObject: %d", (*iter).isObject());

        qDebug("m_updateMessages type: 0x%x", (*iter).type());

        QJsonObject jMessage = (*iter).toObject();

        QJsonDocument doc(jMessage);
        qDebug("m_updateMessages doc: %s", doc.toJson().toStdString().c_str());

        QJsonValue message = jMessage["message"];
        qDebug("MainWindow::m_updateMessages message: %s", message.toString().toStdString().c_str());

        QString prevText = ui->messageView->toPlainText();
        prevText += message.toString() + "\r\n";
        ui->messageView->setPlainText(prevText);
    }

}

void MainWindow::m_getLastMessages(unsigned int count)
{
    qDebug("MainWindow::m_getLastMessages");

    QNetworkRequest req;
    req.setUrl(QUrl("http://localhost:8080/getLastMessages"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject jReq;
    QJsonArray jUsers;
    jUsers.append("braczkow");
    jUsers.append("marsza");

    jReq["users"] = jUsers;

    jReq["count"] = (int) count;

    QJsonDocument doc(jReq);
    std::string json = doc.toJson().toStdString();

     m_NAManager->post(req, json.c_str());
}

void MainWindow::on_newMessage(QString info)
{
    qDebug("MainWindow::onNewMessage. Thread: %d", (unsigned int) QThread::currentThreadId());

    QString prevText = ui->messageView->toPlainText();
    prevText += "dupa\n";
    ui->messageView->setPlainText(prevText);
}

void MainWindow::on_sendMessageButton_clicked()
{
    qDebug("MainWindow::on_sendMessageButton_clicked");

    QNetworkRequest req;
    req.setUrl(QUrl("http://localhost:8080/message"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonObject jMessage;
    jMessage["message"] = QJsonValue(ui->userMessage->text().toStdString().c_str());

    QJsonArray jUsers;
    jUsers.append("braczkow");
    jUsers.append("marsza");

    jMessage["users"] = jUsers;

    QJsonDocument doc(jMessage);
    std::string json = doc.toJson().toStdString();


    qDebug("MainWindow::on_sendMessageButton_clicked: jMessage: %s", json.c_str());

    QNetworkReply *reply = m_NAManager->post(req, json.c_str());

    ui->userMessage->clear();
}
