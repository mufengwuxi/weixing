/// @file      testSurface3D.cpp
/// @brief     Test Qwt3D SurfacePlot with raw data, no matplot/backend
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-20

#if defined(AST_WITH_QWT) && defined(AST_WITH_QT)
#include <QApplication>
#include <qwt3d_surfaceplot.h>

using namespace Qwt3D;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    SurfacePlot plot;
    plot.resize(800, 600);
    plot.show();  // 必须先 show() 初始化 OpenGL 上下文，再加载数据

    plot.setTitle("Qwt3D SurfacePlot — Direct Test");

    int cols = 50, rows = 50;
    double minx = -3.0, maxx = 3.0;
    double miny = -3.0, maxy = 3.0;

    std::vector<Triple*> triples(cols);
    std::vector<std::vector<Triple>> tStore(cols, std::vector<Triple>(rows));
    for (int c = 0; c < cols; ++c) {
        double x = minx + (maxx - minx) * c / (cols - 1);
        for (int r = 0; r < rows; ++r) {
            double y = miny + (maxy - miny) * r / (rows - 1);
            tStore[c][r] = Triple(x, y, sin(x) * cos(y));
        }
        triples[c] = tStore[c].data();
    }

    plot.loadFromData(triples.data(), cols, rows);
    plot.setFloorStyle(NOFLOOR);
    plot.setRotation(30, 0, 15);
    plot.setZoom(0.9);

    return app.exec();
}
#else
#include <stdio.h>
int main(int argc, char* argv[])
{
    puts("没有启用QWT和QT库");
    return 0;
}
#endif