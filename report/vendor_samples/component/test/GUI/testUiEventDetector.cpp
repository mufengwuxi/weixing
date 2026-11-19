/// @file      testUiEventDetector.cpp
/// @brief     交互式测试 UiEventDetector — 事件检测器属性编辑
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-18

#ifdef AST_WITH_QT
#include "ast/UiEventDetectorEditor.hpp"
#include "ast/UiEventDetector.hpp"
#include "ast/EventDetector.hpp"
#include "ast/DetectorDuration.hpp"
#include "ast/DetectorPeriapsis.hpp"
#include "ast/DetectorUserSelect.hpp"
#include "ast/RTTIAPI.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

using namespace ast;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // ---- 构造测试数据 ----
    auto* det1 = aNewObject<DetectorDuration>();
    det1->setName("DurationDet");
    det1->setDuration(3600.0);

    auto* det2 = aNewObject<DetectorPeriapsis>();
    det2->setName("PeriapsisDet");

    auto* det3 = aNewObject<DetectorUserSelect>();
    det3->setName("UserSelectDet");

    // ---- 主窗口 ----
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(9, 9, 9, 9);
    rootLayout->setSpacing(6);

    // ---- 检测器选择 ----
    auto* topBar = new QHBoxLayout();
    topBar->addWidget(new QLabel(QStringLiteral("选择检测器:"), central));
    auto* selector = new QComboBox(central);
    selector->addItem(QStringLiteral("DetectorDuration"), 0);
    selector->addItem(QStringLiteral("DetectorPeriapsis"), 1);
    selector->addItem(QStringLiteral("DetectorUserSelect"), 2);
    topBar->addWidget(selector);
    topBar->addStretch();
    rootLayout->addLayout(topBar);

    // ---- UiEventDetectorEditor ----
    auto* editor = new UiEventDetectorEditor(central);
    rootLayout->addWidget(editor, 1);

    // ---- 信息标签 ----
    auto* infoLabel = new QLabel(central);
    infoLabel->setWordWrap(true);
    infoLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    infoLabel->setMinimumHeight(60);
    infoLabel->setMaximumHeight(120);
    rootLayout->addWidget(infoLabel);

    // ---- 操作按钮 ----
    auto* btnRow = new QHBoxLayout();
    auto* printBtn = new QPushButton(QStringLiteral("打印属性"), central);
    auto* verifyBtn = new QPushButton(QStringLiteral("验证 getDetector"), central);
    btnRow->addWidget(printBtn);
    btnRow->addWidget(verifyBtn);
    btnRow->addStretch();
    rootLayout->addLayout(btnRow);

    // ---- 当前选中检测器 ----
    EventDetector* currentDet = det1;

    QObject::connect(selector, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&](int idx) {
            switch (idx) {
            case 0: currentDet = det1; break;
            case 1: currentDet = det2; break;
            case 2: currentDet = det3; break;
            }
            editor->setDetector(currentDet);
        });

    QObject::connect(printBtn, &QPushButton::clicked, [&]() {
        auto* det = editor->getDetector();
        if (!det) {
            infoLabel->setText(QStringLiteral("detector 为空"));
            return;
        }
        QString info;
        info += QStringLiteral("类型: %1\n").arg(QString::fromStdString(det->getType()->name()));
        info += QStringLiteral("激活: %1  重复: %2  阈值: %3  目标值: %4\n")
            .arg(det->active() ? "是" : "否")
            .arg(det->repeatCount())
            .arg(det->threshold())
            .arg(det->goal());
        if (auto* dur = aobject_cast<DetectorDuration*>(det))
            info += QStringLiteral("时长: %1 s\n").arg(dur->duration());
        infoLabel->setText(info);
        qDebug() << info;
    });

    QObject::connect(verifyBtn, &QPushButton::clicked, [&]() {
        auto* got = editor->getDetector();
        infoLabel->setText(got == currentDet
            ? QString::fromUtf8("✓ getDetector() 返回正确对象")
            : QString::fromUtf8("✗ getDetector() 返回错误对象"));
    });

    editor->setDetector(det1);
    printBtn->click();

    window.setCentralWidget(central);
    window.setWindowTitle(QStringLiteral("UiEventDetector — 交互测试"));
    window.resize(500, 500);
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
