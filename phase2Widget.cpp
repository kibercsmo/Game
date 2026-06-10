#include "phase2Widget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

Phase2Widget::Phase2Widget(QWidget *parent)
    : QWidget(parent)
    , m_active(false)
{
    setupUI();
    installEventFilter(this);
}

Phase2Widget::~Phase2Widget()
{
}

void Phase2Widget::setActive(bool active)
{
    m_active = active;
    if (secretInput) {
        secretInput->hide();
        secretInput->clear();
    }
}

void Phase2Widget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    instructionLabel = new QLabel("Ладно, просто закрой окно", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 24px; margin: 50px;");

    hintLabel = new QLabel("Подсказка: Ctrl+Q", this);
    hintLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    hintLabel->setStyleSheet("color: #F0F0F0; font-size: 12px;");

    secretInput = new QLineEdit(this);
    secretInput->setPlaceholderText("Введите кодовое слово...");
    secretInput->setStyleSheet("font-size: 16px; padding: 10px;");
    secretInput->hide();

    connect(secretInput, &QLineEdit::returnPressed, this, &Phase2Widget::checkCodeWord);

    mainLayout->addWidget(instructionLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(hintLabel);
    mainLayout->addWidget(secretInput);

    setLayout(mainLayout);
}

bool Phase2Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (m_active && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Q) {
            showSecretInput();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void Phase2Widget::showSecretInput()
{
    if (m_active) {
        secretInput->show();
        secretInput->setFocus();
        secretInput->clear();
    }
}

void Phase2Widget::checkCodeWord()
{
    if (!m_active) return;

    QString text = secretInput->text().toLower();
    if (text == "пошёл отседова") {
        emit phaseComplete();
    } else {
        secretInput->clear();
        secretInput->setPlaceholderText("Не-а, попробуй ещё");
        QTimer::singleShot(2000, [this]() {
            if (secretInput && m_active) {
                secretInput->setPlaceholderText("Введите кодовое слово...");
            }
        });
    }
}