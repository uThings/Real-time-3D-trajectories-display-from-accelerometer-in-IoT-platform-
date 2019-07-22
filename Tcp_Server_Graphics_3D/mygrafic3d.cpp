#include <stdio.h>
#include <stdlib.h>

#include <QtDataVisualization>
#include <QtWidgets>
#include <QDialog>
#include <QtCore>
#include <QObject>

#include "mygrafic3d.h"
#include "myserver.h"

#define DIM 10
#define EPSILON 0.002
#define G 9.81
#define RANGE 20
#define t 1

using namespace QtDataVisualization;

mygrafic3d::mygrafic3d(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QAbstract3DSeries::MeshSphere),
      m_smooth(true),
      m_series(new QScatter3DSeries),
      m_array(new QScatterDataArray)
{
    //Modifiche visive
    m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    m_graph->axisX()->setTitle("Y");
    m_graph->axisY()->setTitle("Z");
    m_graph->axisZ()->setTitle("X");

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);

    m_graph->axisX()->setRange(-RANGE, RANGE);
    m_graph->axisY()->setRange(-RANGE, RANGE);
    m_graph->axisZ()->setRange(-RANGE, RANGE);

    //creo un QScatterDataProxy e il QScatter3DSeries associato
    QScatterDataProxy *proxy = new QScatterDataProxy;
    m_series = new QScatter3DSeries(proxy);
    m_series->setItemLabelFormat(QStringLiteral("@zTitle: @zLabel @xTitle: @xLabel @yTitle: @yLabel"));
    m_series->setMeshSmooth(m_smooth);
    m_graph->addSeries(m_series);

    connect(&server, &MyServer::signal_resetarray, this, &mygrafic3d::slot_resetarray);
    connect(&server, &MyServer::signal_data, this, &mygrafic3d::slot_data);
}


mygrafic3d::~mygrafic3d()
{
    delete m_graph;
}


void mygrafic3d::slot_resetarray()
{
    m_series->dataProxy()->resetArray(new QScatterDataArray);
}


void mygrafic3d::slot_data(const QByteArray &stringa)
{
    strcpy(dato, stringa);
    estrai_dati();
    addData();
}


void mygrafic3d::estrai_dati()
{
    char asseX[DIM], asseY[DIM], asseZ[DIM];
    int i = 0;
    int j = 0;

    struct valori X = {0, 0, 0};
    struct valori Y = {0, 0, 0};
    struct valori Z = {0, 0, 0};

    while (dato[i] != ' ')
    {
        asseX[j] = dato[i];
        j++;
        i++;
    }
    asseX[j] = '\0';
    i++;
    j = 0;

    while (dato[i] != ' ')
    {
        asseY[j] = dato[i];
        j++;
        i++;
    }
    asseY[j] = '\0';
    i++;
    j = 0;

    while (dato[i] != ' ')
    {
        asseZ[j] = dato[i];
        j++;
        i++;
    }
    asseZ[j] = '\0';

    acc_x = std::stof(asseX);
    acc_y = std::stof(asseY);
    acc_z = std::stof(asseZ);

    qDebug() << "Xacc: " << acc_x << "Yacc: " << acc_y << "Zacc: " << acc_z;

    xpos = calcolo_posizione_da_accelerazioni (acc_x, X.accelerazione, X.velocita, X.posizione, 1);
    ypos = calcolo_posizione_da_accelerazioni (acc_y, Y.accelerazione, Y.velocita, Y.posizione, 1);
    zpos = calcolo_posizione_da_accelerazioni (acc_z, Z.accelerazione, Z.velocita, Z.posizione, -1);

    qDebug() << "Xpos: " << xpos << " Ypos: " << ypos << " Zpos: " << zpos;
    qDebug() << "------------------------------------------------------";
}


void mygrafic3d::addData()
{
    QScatterDataArray *array = new QScatterDataArray(*m_series->dataProxy()->array());
    m_series->dataProxy()->resetArray(Q_NULLPTR);
    array->append(QScatterDataItem(QVector3D(ypos, zpos, xpos)));
    m_series->dataProxy()->resetArray(array);
}


float mygrafic3d::calcolo_posizione_da_accelerazioni (float acc_letta, float acc_prec, float vel_prec, float pos_prec, int verso)
{
    /*se il valore la differenza è maggiore di EPSILON mi muovo quindi calcolo i nuovi valori
      altrimenti non mi muovo quindi mantengo i vecchi valori*/

    float acc_nuova, delta_acc;

    acc_nuova = acc_letta * G;
    delta_acc = (acc_nuova - acc_prec) * verso;
    acc_prec = delta_acc;

    if (fabs(delta_acc) > EPSILON)
    {
       vel_prec = moto_uniformemente_accelerato_velocita (acc_prec, vel_prec);
       pos_prec = moto_uniformemente_accelerato_posizione (acc_prec, vel_prec, pos_prec);
    }
    else
    {
       vel_prec = 0.0;
       pos_prec = pos_prec;
    }

    return pos_prec;
}


float mygrafic3d::moto_uniformemente_accelerato_velocita (float a, float v0)
{
    float v;

    /*calcola la velocità*/
    v = v0 + a * t;

    /*aggiorno lo stato*/
    v0 = v;

    return v;
}


float mygrafic3d::moto_uniformemente_accelerato_posizione (float a, float v0, float s0)
{
    float s;

    /*calcola la posizione*/
    s = s0 + v0 * t + 0.5 * a * t * t;

    /*aggiorno lo stato*/
    s0 = s;

    return s;
}
