#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QByteArray>
#include <QCoreApplication>
#include <QObject>
#include <QtDataVisualization>
#include <QThread>

#include "myserver.h"
#include "mygrafic3d.h"

#define PORTA 5000

MyServer::MyServer(QObject *parent): QObject(parent)
{
    controllo = 0;
    server = new QTcpServer(this);

    //non appena arrivano nuove connessioni esegue newConnection()
    connect (server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    //mette in ascolto il server sulla porta definita sopra
    if (!server->listen(QHostAddress::Any, PORTA))
    {
        qDebug()<< "Il Server non può partire!";
    }
    else
    {
        qDebug()<< "Server partito!";
    }
}


void MyServer::newConnection()
{
    //Aggancio il socket al server (bind)
    socket = server->nextPendingConnection();

    //mando un messaggio di benvenuto al client
    socket->write("Ciao client! Inserisci una stringa:");
    socket->flush();

    connect(socket, SIGNAL(readyRead()), this, SLOT(ascolto_client()));
}


void MyServer::ascolto_client()
{
    if (controllo == 0)
    {
        controllo = 1;
        mykeypass = socket->readAll();
        socket->flush();
        qDebug() << "";
        qDebug() << "Client connesso e pronto a inviare i campioni";
    }
    else
    {
        //Leggo e mostro cosa ha scritto il client
        mykeypass = socket->readAll();
        socket->flush();
        qDebug() << "mykeypass: " << mykeypass;

        identifica_messaggio();

        if (strcmp(messaggio, "C") == 0)
        {
            int campioni = atoi(socketread);
            qDebug() << "";
            qDebug() << "Numero di campioni inviati dal client: " << campioni;
            emit signal_resetarray();
        }
        else if (strcmp(messaggio, "A") == 0)
        {
            emit signal_data(socketread);
        }
        else
        {
            qDebug() << "ERRORE: stringa non riconosciuta!!!";
        }
    }
}


void MyServer::identifica_messaggio()
{
    strcpy(socketread, mykeypass);
    int i = 0;
    int j = 0;

    while (socketread[i] != ':')
    {
        messaggio[j] = socketread[i];
        i++;
        j++;
    }
    messaggio[j] = '\0';

    i++;
    i++;
    j = 0;

    while (socketread[i] != '\0')
    {
        socketread[j] = socketread[i];
        i++;
        j++;
    }
    socketread[j] = '\0';
}
