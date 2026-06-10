#include "phase1Widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

Phase1Widget::Phase1Widget(QWidget *parent)
    : QWidget(parent)
    , m_active(false)
{
    setupUI();
}

Phase1Widget::~Phase1Widget()
{
}

void Phase1Widget::setActive(bool active)
{
    m_active = active;
    if (runawayButton) {
        runawayButton->setEnabled(active);
        if (active) {
            runawayButton->setFocusPolicy(Qt::StrongFocus);
            QTimer::singleShot(100, this, [this]() {
                if (runawayButton && runawayButton->parentWidget()) {
                    runawayButton->move(125, 70);
                }
            });
        } else {
            runawayButton->setFocusPolicy(Qt::NoFocus);
        }
    }
}

void Phase1Widget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    instructionLabel = new QLabel("Нажми ВЫХОД, чтобы закрыть", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 30px;");

    QWidget *buttonContainer = new QWidget(this);
    buttonContainer->setFixedSize(400, 300);

    runawayButton = new RunawayButton(buttonContainer);
    runawayButton->setFocusPolicy(Qt::StrongFocus);
    runawayButton->move(125, 120);

    connect(runawayButton, &QPushButton::clicked, this, &Phase1Widget::onButtonClicked);

    hintLabel = new QLabel("Подсказка: мышка — не единственный способ управления...", this);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet("color: gray; font-size: 12px; margin: 20px;");

    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(buttonContainer, 0, Qt::AlignCenter);
    mainLayout->addWidget(hintLabel);

    setLayout(mainLayout);
}

void Phase1Widget::onButtonClicked()
{
    if (m_active) {
        emit phaseComplete();
    }
}