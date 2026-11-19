/// @file      testUiSTKEphemerisFileWriter.cpp
/// @brief     测试 STK 星历文件导出界面
/// @details   构造样例星历并绑定到 UiSTKEphemerisFileWriter，
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-09-07

#ifdef AST_WITH_QT
#include "ast/UiSTKEphemerisFileWriter.hpp"
#include "ast/EphemerisLagrangeVar.hpp"
#include "ast/Frame.hpp"
#include "ast/BuiltinFrame.hpp"
#include "ast/TimePoint.hpp"
#include "ast/Vector.hpp"
#include "ast/RunTime.hpp"
#include "ast/AstGUIAPI.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

AST_USING_NAMESPACE

namespace{

const char* kTestOutFile = "test_ui_stk_export.e";

/// @brief 构造一个绕地球的圆轨道可变步长星历
/// @return 星历对象（调用方负责释放）
EphemerisLagrangeVar* makeSampleEphemeris()
{
    Frame* frame = aFrameEarthICRF();
    auto* eph = new EphemerisLagrangeVar(frame);
    eph->setEpoch(TimePoint::FromUTC(2026, 9, 7, 4, 0, 0));
    eph->setInterpolateOrder(5);

    const double r   = 7000000.0;   // 轨道半径 [m]
    const double v   = 7500.0;      // 轨道速度 [m/s]
    const double dt  = 60.0;        // 采样步长 [s]

    std::vector<double>   times;
    std::vector<Vector3d> posList, velList;
    for(int i = 0; i < 10; ++i)
    {
        const double t   = i * dt;
        const double ang = v * t / r;   // 圆轨道角位移
        times.push_back(t);
        posList.push_back(Vector3d{ r*std::cos(ang), r*std::sin(ang), 0.0 });
        velList.push_back(Vector3d{ -v*std::sin(ang), v*std::cos(ang), 0.0 });
    }
    eph->setTimes(times);
    eph->setPositions(posList);
    eph->setVelocities(velList);
    return eph;
}

/// @brief 校验导出的 STK 文件是否包含关键头尾标记
/// @param filepath 文件路径
/// @return 校验通过
bool verifyStkFile(const QString& filepath)
{
    std::ifstream f(filepath.toStdString());
    if(!f)
        return false;
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return content.find("stk.v.11.0")       != std::string::npos
        && content.find("BEGIN Ephemeris")  != std::string::npos
        && content.find("END Ephemeris")    != std::string::npos
        && content.find("CentralBody")      != std::string::npos;
}

}

int main(int argc, char* argv[])
{
    printf("testUiSTKEphemerisFileWriter.cpp\n");
    aInitialize();   // 初始化运行环境（天体、坐标系等），供 STK 导出解析中心天体
    aQAppInit(argc, argv);

    QMainWindow window;
    QWidget* central = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // 创建导出界面并绑定样例星历
    auto* form = new UiSTKEphemerisFileWriter(central);
    form->setEphemeris(makeSampleEphemeris());
    layout->addWidget(form);

    // 手动导出 + 校验按钮
    QPushButton* exportBtn = new QPushButton("Export + 校验", central);
    layout->addWidget(exportBtn);
    QObject::connect(exportBtn, &QPushButton::clicked, [form]() {
        errc_t rc = form->exportTo(kTestOutFile);
        bool ok = (rc == eNoError) && verifyStkFile(kTestOutFile);
        printf("export rc=%d, file ok=%d\n", (int)rc, (int)ok);
        std::remove(kTestOutFile);
    });

    window.setCentralWidget(central);
    window.setWindowTitle("UiSTKEphemerisFileWriter Test");
    window.resize(720, 520);
    window.show();

    int ret = qApp->exec();   // 进入事件循环，等待用户手动操作
    aUninitialize();
    return ret;
}
#else
#include <stdio.h>
int main(){
    puts("没有启用Qt库");
    return 0;
}
#endif
