/// @file      testUiStateCartesian.cpp
/// @brief     测试 UiStateCartesian 类
/// @details   测试轨道状态编辑界面的功能
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-28

#ifdef AST_WITH_QT
#include "ast/UiStateCartesian.hpp"
#include "ast/StateCartesian.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

AST_USING_NAMESPACE

int main(int argc, char* argv[])
{
    printf("testUiStateCartesian.cpp\n");
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 创建 UiStateCartesian 组件
    UiStateCartesian* uiStateCartesian = new UiStateCartesian(centralWidget);
    layout->addWidget(uiStateCartesian);

    // 创建测试按钮
    QPushButton* testButton = new QPushButton("测试", centralWidget);
    layout->addWidget(testButton);

    // 创建 StateCartesian 对象
    StateCartesian* stateCartesian = StateCartesian::New();
    stateCartesian->setX(10000000);
    stateCartesian->setY(20000000);
    stateCartesian->setZ(30000000);
    stateCartesian->setVx(1000);
    stateCartesian->setVy(2000);
    stateCartesian->setVz(3000);

    // 绑定 StateCartesian 对象
    uiStateCartesian->setStateCartesian(stateCartesian);

    QObject::connect(uiStateCartesian, &UiStateCartesian::stateCartesianChanged, [](StateCartesian* newState) {
        if (newState)
        {
            qDebug() << "StateCartesian Changed:" << newState->x() << newState->y() << newState->z() << newState->vx() << newState->vy() << newState->vz();
        }
    });

    QObject::connect(testButton, &QPushButton::clicked, [uiStateCartesian, stateCartesian]() {
        // 测试 getStateCartesian 方法
        StateCartesian* updatedState = uiStateCartesian->getStateCartesian();
        if (updatedState)
        {
            qDebug() << "Updated StateCartesian:";
            qDebug() << "X:" << updatedState->x();
            qDebug() << "Y:" << updatedState->y();
            qDebug() << "Z:" << updatedState->z();
            qDebug() << "Vx:" << updatedState->vx();
            qDebug() << "Vy:" << updatedState->vy();
            qDebug() << "Vz:" << updatedState->vz();
        }
    });

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("UiStateCartesian Test");
    window.resize(800, 400);
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
