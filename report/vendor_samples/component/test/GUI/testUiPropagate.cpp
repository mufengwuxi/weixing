/// @file      testUiPropagate.cpp
/// @brief     交互式测试 UiPropagate — 预报器选择、时间设置
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-18

#ifdef AST_WITH_QT
#include "ast/UiPropagate.hpp"
#include "ast/Propagate.hpp"
#include "ast/HPOP.hpp"
#include "ast/RTTIAPI.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

using namespace ast;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // ---- 构造测试数据 ----
    auto* prop = aNewObject<Propagate>();
    prop->setName("TestPropagate");
    prop->setMinPropTime(0);
    prop->setMaxPropTime(86400);
    prop->setUseMaxPropTime(true);

    auto* hpop = aNewObject<HPOP>();
    prop->setPropagator(hpop);

    // ---- 主窗口 ----
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* rootLayout = new QVBoxLayout(central);

    // ---- UiPropagate 编辑器 ----
    auto* editor = new UiPropagate(central);
    editor->setPropagate(prop);

    // ---- 信息标签 ----
    auto* infoLabel = new QLabel(central);
    infoLabel->setWordWrap(true);
    infoLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    infoLabel->setMinimumHeight(80);

    // ---- 操作按钮 ----
    auto* btnGroup = new QGroupBox(QStringLiteral("操作"), central);
    auto* btnLayout = new QHBoxLayout(btnGroup);

    auto* printBtn = new QPushButton(QStringLiteral("打印预报参数"), central);
    auto* verifyBtn = new QPushButton(QStringLiteral("验证 getPropagate"), central);

    btnLayout->addWidget(printBtn);
    btnLayout->addWidget(verifyBtn);
    btnLayout->addStretch();

    // ---- 组装界面 ----
    rootLayout->addWidget(new QLabel(QStringLiteral("UiPropagate 编辑器 — 预报器设置测试"), central));
    rootLayout->addWidget(editor);
    rootLayout->addWidget(infoLabel);
    rootLayout->addWidget(btnGroup);

    // ---- 连接 ----
    QObject::connect(printBtn, &QPushButton::clicked, [prop, infoLabel]() {
        QString info;
        info += QStringLiteral("Propagate 名称: %1\n").arg(QString::fromStdString(prop->getName()));
        info += QStringLiteral("最小预报时间: %1 s\n").arg(prop->minPropTime());
        info += QStringLiteral("最大预报时间: %1 s\n").arg(prop->maxPropTime());
        info += QStringLiteral("启用最大预报时间: %1\n").arg(prop->useMaxPropTime() ? "是" : "否");

        if (auto* hpop = prop->propagator())
            info += QStringLiteral("预报器: HPOP (已绑定)");
        else
            info += QStringLiteral("预报器: 未绑定");

        infoLabel->setText(info);
        qDebug() << info;
    });

    QObject::connect(verifyBtn, &QPushButton::clicked, [editor, prop, infoLabel]() {
        auto* got = editor->getPropagate();
        if (got == prop)
            infoLabel->setText(QString::fromUtf8("✓ getPropagate() 返回正确对象"));
        else
            infoLabel->setText(QString::fromUtf8("✗ getPropagate() 返回错误对象"));
    });

    // 初始显示信息
    printBtn->click();

    window.setCentralWidget(central);
    window.setWindowTitle(QStringLiteral("UiPropagate — 交互测试"));
    window.resize(500, 550);
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
