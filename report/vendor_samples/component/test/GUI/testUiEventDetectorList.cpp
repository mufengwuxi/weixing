/// @file      testUiEventDetectorList.cpp
/// @brief     交互式测试 UiEventDetectorList — 事件检测器列表管理
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-18

#ifdef AST_WITH_QT
#include "ast/UiEventDetectorList.hpp"
#include "ast/Propagate.hpp"
#include "ast/HPOP.hpp"
#include "ast/EventDetector.hpp"
#include "ast/DetectorPeriapsis.hpp"
#include "ast/DetectorDuration.hpp"
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

    auto* hpop = aNewObject<HPOP>();
    prop->setPropagator(hpop);

    // 预置几个检测器
    {
        auto* det1 = aNewObject<DetectorPeriapsis>();
        det1->setName("Periapsis");
        auto* det2 = aNewObject<DetectorDuration>();
        det2->setName("Duration");
        prop->setEventDetectors({SharedPtr<EventDetector>(det1), SharedPtr<EventDetector>(det2)});
    }

    // ---- 主窗口 ----
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* rootLayout = new QVBoxLayout(central);

    // ---- UiEventDetectorList ----
    auto* editor = new UiEventDetectorList(central);
    editor->setPropagate(prop);

    // ---- 信息标签 ----
    auto* infoLabel = new QLabel(central);
    infoLabel->setWordWrap(true);
    infoLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    infoLabel->setMinimumHeight(60);

    // ---- 操作按钮 ----
    auto* btnGroup = new QGroupBox(QStringLiteral("操作"), central);
    auto* btnLayout = new QHBoxLayout(btnGroup);

    auto* printBtn = new QPushButton(QStringLiteral("打印检测器列表"), central);
    auto* verifyBtn = new QPushButton(QStringLiteral("验证 getPropagate"), central);

    btnLayout->addWidget(printBtn);
    btnLayout->addWidget(verifyBtn);
    btnLayout->addStretch();

    // ---- 组装界面 ----
    rootLayout->addWidget(new QLabel(QStringLiteral("UiEventDetectorList — 事件检测器列表管理"), central));
    rootLayout->addWidget(editor);
    rootLayout->addWidget(infoLabel);
    rootLayout->addWidget(btnGroup);

    // ---- 连接 ----
    QObject::connect(printBtn, &QPushButton::clicked, [prop, infoLabel]() {
        QString info;
        info += QStringLiteral("Propagate 名称: %1\n").arg(QString::fromStdString(prop->getName()));

        const auto& detectors = prop->eventDetectors();
        info += QStringLiteral("检测器数量: %1\n").arg((int)detectors.size());

        for (size_t i = 0; i < detectors.size(); ++i)
        {
            if (detectors[i])
                info += QStringLiteral("  [%1] %2\n")
                    .arg((int)i)
                    .arg(QString::fromStdString(detectors[i]->getType()->name()));
        }

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
    window.setWindowTitle(QStringLiteral("UiEventDetectorList — 交互测试"));
    window.resize(1000, 800);
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
