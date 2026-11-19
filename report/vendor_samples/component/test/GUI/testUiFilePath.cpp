/// @file      testUiFilePath.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-04

#ifdef AST_WITH_QT
#include "ast/UiFilePath.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

AST_USING_NAMESPACE

int main(int argc, char* argv[])
{
    printf("testUiFilePath.cpp\n");
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 创建 UiFilePath 组件
    UiFilePath* uiFilePath = new UiFilePath(centralWidget);
    uiFilePath->setFilter("SPK文件 (*.bsp);;所有文件 (*.*)");
    layout->addWidget(uiFilePath);

    // 创建测试按钮
    QPushButton* testButton = new QPushButton("测试", centralWidget);
    layout->addWidget(testButton);

    QObject::connect(testButton, &QPushButton::clicked, [uiFilePath]() {
        QString path = uiFilePath->path();
        qDebug() << "Current path:" << path;
    });


    QObject::connect(uiFilePath, &UiFilePath::pathChanged, [uiFilePath]() {
        qDebug() << "after pathChanged:" << uiFilePath->path();
    });

    // 设置默认值
    uiFilePath->setPath("");

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("UiFilePath Test");
    window.resize(600, 100);
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
