/// @file      testUiQuantity.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-27

#ifdef AST_WITH_QT
#include "ast/UiQuantity.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

AST_USING_NAMESPACE

int main(int argc, char* argv[])
{
    printf("testUiQuantity.cpp\n");
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 创建 UiQuantity 组件
    UiQuantity* uiQuantity = new UiQuantity(centralWidget);
    layout->addWidget(uiQuantity);

    // 创建测试按钮
    QPushButton* testButton = new QPushButton("测试", centralWidget);
    layout->addWidget(testButton);

    QObject::connect(testButton, &QPushButton::clicked, [uiQuantity]() {
        Quantity q = uiQuantity->getQuantity();
        qDebug() << "Current quantity:" << q.toString().c_str();
    });


    QObject::connect(uiQuantity, &UiQuantity::quantityChanged, [uiQuantity]() {
        qDebug() << "after quantityChanged:" << uiQuantity->getQuantity().toString().c_str();
    });

    // 设置默认值
    uiQuantity->setQuantity(Quantity(100, units::m));

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("UiQuantity Test");
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
