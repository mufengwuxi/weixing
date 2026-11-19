/// @file      testUiStateKeplerian.cpp
/// @brief     测试 UiStateKeplerian 类
/// @details   测试开普勒轨道根数状态编辑界面的功能
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-31

#ifdef AST_WITH_QT
#include "ast/UiStateKeplerian.hpp"
#include "ast/StateKeplerian.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/RunTime.hpp"
#include "ast/Literals.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

AST_USING_NAMESPACE

int main(int argc, char* argv[])
{
    aInitialize();
    printf("testUiStateKeplerian.cpp\n");
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 创建 UiStateKeplerian 组件
    UiStateKeplerian* uiStateKeplerian = new UiStateKeplerian(centralWidget);
    layout->addWidget(uiStateKeplerian);

    // 创建测试按钮
    QPushButton* testButton = new QPushButton("测试", centralWidget);
    layout->addWidget(testButton);

    // 创建 StateKeplerian 对象
    StateKeplerian* stateKeplerian = StateKeplerian::New();
    stateKeplerian->setSMA(7000000_km); // 半长轴 7000 km
    stateKeplerian->setEcc(0.1);     // 偏心率 0.1
    stateKeplerian->setInc(30.0_deg);    // 倾角 30 度
    stateKeplerian->setRAAN(45.0_deg);   // 升交点赤经 45 度
    stateKeplerian->setArgPeri(60.0_deg); // 近地点幅角 60 度
    stateKeplerian->setTrueAnomaly(90.0_deg); // 真近点角 90 度
    stateKeplerian->setFrame(aGetEarth()->makeFrameICRF()); // 设置参考系为 地球ICRF

    // 绑定 StateKeplerian 对象
    uiStateKeplerian->setStateKeplerian(stateKeplerian);

    QObject::connect(uiStateKeplerian, &UiStateKeplerian::stateKeplerianChanged, [](StateKeplerian* newState) {
        if (newState)
        {
            qDebug() << "StateKeplerian Changed:";
            qDebug() << "SMA:" << newState->getSMA();
            qDebug() << "Ecc:" << newState->getEcc();
            qDebug() << "Inc:" << newState->getInc();
            qDebug() << "RAAN:" << newState->getRAAN();
            qDebug() << "ArgPeri:" << newState->getArgPeri();
            qDebug() << "TrueAnomaly:" << newState->getTrueAnomaly();
        }
    });

    QObject::connect(testButton, &QPushButton::clicked, [uiStateKeplerian, stateKeplerian]() {
        // 测试 getStateKeplerian 方法
        StateKeplerian* updatedState = uiStateKeplerian->getStateKeplerian();
        if (updatedState)
        {
            qDebug() << "Updated StateKeplerian:";
            qDebug() << "SMA:" << updatedState->getSMA();
            qDebug() << "Ecc:" << updatedState->getEcc();
            qDebug() << "Inc:" << updatedState->getInc();
            qDebug() << "RAAN:" << updatedState->getRAAN();
            qDebug() << "ArgPeri:" << updatedState->getArgPeri();
            qDebug() << "TrueAnomaly:" << updatedState->getTrueAnomaly();
        }
    });

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("UiStateKeplerian Test");
    window.resize(1000, 600);
    window.show();

    return app.exec();
}
#else
#include <stdio.h>
int main(){
    puts("没有启用Qt库");
    return 0;
}
#endif
