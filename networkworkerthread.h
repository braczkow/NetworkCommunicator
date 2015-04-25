#ifndef NETWORKWORKERTHREAD
#define NETWORKWORKERTHREAD

#include <QThread>
#include <QNetworkAccessManager>

class NetworkWorkerThread : public QThread
{
    Q_OBJECT

public:
    NetworkWorkerThread();

    virtual ~NetworkWorkerThread();

    void run();

    void doFinish();

signals:
    void newMessage(QString info);

private slots:
    void on_finished(QNetworkReply* reply);

    void on_finished();

public:
    bool doContinue;

private:

    //QNetworkAccessManager* m_NAManager;

};

#endif // NETWORKWORKERTHREAD

