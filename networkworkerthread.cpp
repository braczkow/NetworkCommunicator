#include <networkworkerthread.h>

#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

NetworkWorkerThread::NetworkWorkerThread() :
    doContinue(true)
{
    qDebug("NetworkWorkerThread ctor thread: %d", (unsigned int) QThread::currentThreadId());
}

NetworkWorkerThread::~NetworkWorkerThread()
{

}


void NetworkWorkerThread::doFinish()
{
    doContinue = false;
}


void NetworkWorkerThread::run()
{
    QNetworkAccessManager manager(this);

    connect(&manager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(on_finished(QNetworkReply*)));

    while (doContinue)
    {
        qDebug("In a worker thread: %d", (unsigned int) QThread::currentThreadId());

//        QNetworkRequest req;
//        req.setUrl(QUrl("http://localhost:8080/message"));
//        req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

//        QJsonObject jReq;
//        QJsonArray jUsers;
//        jUsers.append("braczkow");
//        jUsers.append("marsza");

//        jReq["users"] = jUsers;

//        QJsonDocument doc(jReq);
//        std::string json = doc.toJson().toStdString();

//        qDebug("about to POST: %s", json.c_str());
//        QNetworkReply* reply = manager.post(req, json.c_str());

        sleep(1);

        //emit newMessage("it's a new message");
    }

    //delete m_NAManager;
}

void NetworkWorkerThread::on_finished(QNetworkReply* reply)
{
    qDebug("NetworkWorkerThread::on_finished; threadId: %d", QThread::currentThreadId());

}

void NetworkWorkerThread::on_finished()
{
    qDebug("NetworkWorkerThread::on_finished(void); threadId: %d", QThread::currentThreadId());

}

