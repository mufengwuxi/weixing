/// @file      testUiTimeInterval.cpp
/// @brief     测试时间区间编辑控件
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-28

#ifdef AST_WITH_QT
#include "ast/UiTimeInterval.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

AST_USING_NAMESPACE

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 创建 UiTimeInterval 控件
    UiTimeInterval* uiTimeInterval = new UiTimeInterval(centralWidget);
    layout->addWidget(uiTimeInterval);

    // 创建测试按钮
    QPushButton* testButton = new QPushButton("测试", centralWidget);
    layout->addWidget(testButton);

    // 连接测试按钮的点击事件
    QObject::connect(testButton, &QPushButton::clicked, [uiTimeInterval]() {
        // 获取当前时间区间并打印
        const TimeInterval& timeInterval = uiTimeInterval->getTimeInterval();
        qDebug() << "Start Time:" << QString::fromStdString(timeInterval.getStart().toString());
        qDebug() << "Stop  Time:" << QString::fromStdString(timeInterval.getStop().toString());
    });

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("UiTimeInterval Test");
    window.resize(400, 200);
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
