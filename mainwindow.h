#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "networkworkerthread.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NetworkWorkerThread;
class QNetworkAccessManager;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendMessageButton_clicked();

    void on_newMessage(QString info);

    void on_timerExceeded();

    void on_finished(QNetworkReply*);

private:

    void m_getLastMessages(unsigned int count);

    void m_updateMessages(QJsonArray messages);

    Ui::MainWindow *ui;

    QTimer* m_timer;

    NetworkWorkerThread* m_workerThread;

    QNetworkAccessManager* m_NAManager;

    unsigned int m_messagesReceived;
};

#endif // MAINWINDOW_H
