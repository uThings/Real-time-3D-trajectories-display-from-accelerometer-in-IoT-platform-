#include <QApplication>
#include <QtDataVisualization>
#include <QtWidgets>
#include <QtGui>

#include "myserver.h"
#include "mygrafic3d.h"

int main(int argc, char *argv[])
{      
    QApplication app(argc, argv);

    Q3DScatter graph;

    if (!graph.hasContext())
    {
        qWarning() << QStringLiteral("Impossibile inizializzare il contesto OpenGL!");
        return -1;
    }

    graph.setFlags(graph.flags() ^ Qt::FramelessWindowHint);
    graph.resize(800, 500);
    graph.setTitle("TcpServerGraphic3D : Brunato Lorenzo");

    mygrafic3d mygrafic3d (&graph);
    MyServer server;

    graph.show();

    return app.exec();
}
