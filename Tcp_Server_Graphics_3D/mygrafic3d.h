#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QtCore>

#include "myserver.h"

using namespace QtDataVisualization;

struct valori
{
   float accelerazione;
   float velocita;
   float posizione;
};

class mygrafic3d : public QObject
{
    Q_OBJECT

public:

    mygrafic3d(Q3DScatter *scatter);
    ~mygrafic3d();

    char dato[50];
    float acc_x, acc_y, acc_z;
    float xpos, ypos, zpos;

    void estrai_dati();
    void addData();

    float calcolo_posizione_da_accelerazioni (float acc_letta, float acc_prec, float vel_prec, float pos_prec, int verso);
    float moto_uniformemente_accelerato_velocita (float a, float v0);
    float moto_uniformemente_accelerato_posizione (float a, float v0, float s0);

    MyServer server;

public slots:

    void slot_data(const QByteArray &);
    void slot_resetarray();

private:

    QScatter3DSeries *m_series;
    Q3DScatter *m_graph;
    QScatterDataArray *m_array;
    QAbstract3DSeries::Mesh m_style;
    int m_fontSize;
    bool m_smooth;
};

#endif // MAINWINDOW_H
