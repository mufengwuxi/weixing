/// @file      testUiMotionTwoBody.cpp
/// @brief     测试二体运动模型UI组件
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-31

#ifdef AST_WITH_QT
#include "AstGlobal.h"
#include "ast/UiMotionTwoBody.hpp"
#include "ast/MotionTwoBody.hpp"
#include "ast/StateCartesian.hpp"
#include "ast/CelestialBody.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

AST_NAMESPACE_BEGIN

int testUiMotionTwoBody()
{
    aInitialize();
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);

    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("测试二体运动模型UI组件");
    mainWindow.resize(800, 600);

    // 创建中央部件
    QWidget* centralWidget = new QWidget(&mainWindow);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建UiMotionTwoBody组件
    UiMotionTwoBody* uiMotionTwoBody = new UiMotionTwoBody(centralWidget);
    mainLayout->addWidget(uiMotionTwoBody);

    // 创建MotionTwoBody对象
    MotionTwoBody* motionTwoBody = MotionTwoBody::New();

    // 设置给UI组件
    uiMotionTwoBody->setMotionTwoBody(motionTwoBody);

    auto earth = aGetEarth();

    // 创建StateCartesian对象
    StateCartesian* stateCartesian = StateCartesian::New();

    // 设置初始状态
    stateCartesian->setX(6678137.0);
    stateCartesian->setY(0.0);
    stateCartesian->setZ(0.0);
    stateCartesian->setVx(0.0);
    stateCartesian->setVy(7905.36828);
    stateCartesian->setVz(0.0);
    stateCartesian->setStateEpoch(TimePoint::FromUTC(2026, 4, 3, 0, 1, 0));
    stateCartesian->setFrame(earth->makeFrameICRF());

    // 设置给MotionTwoBody
    motionTwoBody->setInitialState(stateCartesian);

    // 刷新UI
    uiMotionTwoBody->refreshUi();

    // 显示主窗口
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.resize(1200, 600);
    mainWindow.show();

    // 运行应用
    return app.exec();
}

AST_NAMESPACE_END

int main()
{
    return _AST testUiMotionTwoBody();
}
#else
#include <stdio.h>
int main(){
    puts("没有启用Qt库");
    return 0;
}
#endif
