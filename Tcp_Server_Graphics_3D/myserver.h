#ifndef MYSERVER_H
#define MYSERVER_H

#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtDataVisualization>
#include <QVector>

class MyServer : public QObject
{
    Q_OBJECT

public:

    explicit MyServer(QObject *parent = nullptr);
    QByteArray mykeypass;
    int controllo;
    char socketread[50];
    char messaggio[50];

public slots:

    void newConnection();
    void ascolto_client();
    void identifica_messaggio();

signals:

    void signal_data(const QByteArray &);
    void signal_resetarray();

private:

    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYSERVER_H
