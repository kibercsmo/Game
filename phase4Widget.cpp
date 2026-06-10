#include "phase4Widget.h"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QScreen>
#include <QRandomGenerator>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>

Phase4Widget::Phase4Widget(QWidget *parent)
    : QWidget(parent)
    , titleIndex(0)
    , gameActive(true)
    , m_active(false)
    , jumpTimer(nullptr)
    , resizeTimer(nullptr)
    , titleTimer(nullptr)
    , closeButton(nullptr)
{
    titleSequence = {"Н", "НА", "НАЖ", "НАЖМ", "НАЖМИ", "НАЖМИ П", "НАЖМИ ПР", "НАЖМИ ПРО", "НАЖМИ ПРОБ", "НАЖМИ ПРОБЕ", "НАЖМИ ПРОБЕЛ"};

    setupUI();
}

Phase4Widget::~Phase4Widget()
{
    stopAllTimers();
}

void Phase4Widget::setActive(bool active)
{
    m_active = active;
    if (active && gameActive) {
        startCrazyEffects();
    } else if (!active) {
        stopAllTimers();
    }
}

void Phase4Widget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *instructionLabel = new QLabel(" ФИНАЛЬНЫЙ БОСС!", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 32px; font-weight: bold; margin: 50px; color: red;");

    victoryLabel = new QLabel(this);
    victoryLabel->setAlignment(Qt::AlignCenter);
    victoryLabel->setVisible(false);

    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(victoryLabel);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void Phase4Widget::startCrazyEffects()
{
    if (!m_active || !gameActive) return;


    stopAllTimers();

    jumpTimer = new QTimer(this);
    int jumpInterval = QRandomGenerator::global()->bounded(300, 500);
    jumpTimer->setInterval(jumpInterval);
    connect(jumpTimer, &QTimer::timeout, this, &Phase4Widget::jumpToRandomPosition);
    jumpTimer->start();

    resizeTimer = new QTimer(this);
    resizeTimer->setInterval(150);
    connect(resizeTimer, &QTimer::timeout, this, &Phase4Widget::randomResize);
    resizeTimer->start();

    titleTimer = new QTimer(this);
    titleTimer->setInterval(200);
    connect(titleTimer, &QTimer::timeout, this, &Phase4Widget::updateWindowTitle);
    titleTimer->start();
}

void Phase4Widget::jumpToRandomPosition()
{
    if (!m_active || !gameActive) {
        return;
    }
    QWidget *mainWindow = window();
    if (!mainWindow) {
        return;
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }

    QRect screenGeometry = screen->availableGeometry();
    int maxX = screenGeometry.width() - mainWindow->width();
    int maxY = screenGeometry.height() - mainWindow->height();

    if (maxX > 0 && maxY > 0) {
        int x = QRandomGenerator::global()->bounded(maxX);
        int y = QRandomGenerator::global()->bounded(maxY);
        mainWindow->move(x, y);
    } else {
        if (mainWindow->width() > screenGeometry.width() - 100) {
            mainWindow->resize(screenGeometry.width() - 100, mainWindow->height());
        }
        if (mainWindow->height() > screenGeometry.height() - 100) {
            mainWindow->resize(mainWindow->width(), screenGeometry.height() - 100);
        }
    }
}

void Phase4Widget::randomResize()
{
    if (!m_active || !gameActive) {
        return;
    }

    QWidget *mainWindow = window();
    if (!mainWindow) return;

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QRect screenGeometry = screen->availableGeometry();

    int newWidth = QRandomGenerator::global()->bounded(300, std::min(800, screenGeometry.width() - 50));
    int newHeight = QRandomGenerator::global()->bounded(200, std::min(600, screenGeometry.height() - 50));

    mainWindow->resize(newWidth, newHeight);

    QPoint currentPos = mainWindow->pos();
    int maxX = screenGeometry.width() - mainWindow->width();
    int maxY = screenGeometry.height() - mainWindow->height();

    int newX = std::max(0, std::min(currentPos.x(), maxX));
    int newY = std::max(0, std::min(currentPos.y(), maxY));

    if (newX != currentPos.x() || newY != currentPos.y()) {
        mainWindow->move(newX, newY);
    }
}

void Phase4Widget::updateWindowTitle()
{
    if (!m_active || !gameActive) return;

    QWidget *mainWindow = window();
    if (!mainWindow) return;

    if (titleIndex < titleSequence.size()) {
        mainWindow->setWindowTitle(titleSequence[titleIndex]);
        titleIndex++;
    } else {
        titleIndex = 0;
        mainWindow->setWindowTitle(titleSequence[titleIndex]);
        titleIndex++;
    }
}

void Phase4Widget::keyPressEvent(QKeyEvent *event)
{

    if (m_active && gameActive && event->key() == Qt::Key_Space) {
        stopAllTimers();
        gameActive = false;
        showVictoryScreen();
    }

    QWidget::keyPressEvent(event);
}

void Phase4Widget::stopAllTimers()
{

    if (jumpTimer) {
        if (jumpTimer->isActive()) jumpTimer->stop();
        delete jumpTimer;
        jumpTimer = nullptr;
    }
    if (resizeTimer) {
        if (resizeTimer->isActive()) resizeTimer->stop();
        delete resizeTimer;
        resizeTimer = nullptr;
    }
    if (titleTimer) {
        if (titleTimer->isActive()) titleTimer->stop();
        delete titleTimer;
        titleTimer = nullptr;
    }
}

void Phase4Widget::showVictoryScreen()
{

    QWidget *mainWindow = window();
    if (!mainWindow) return;

    mainWindow->resize(600, 500);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - mainWindow->width()) / 2;
        int y = (screenGeometry.height() - mainWindow->height()) / 2;
        mainWindow->move(x, y);
    }

    mainWindow->setWindowTitle("ПОБЕДА!");

    QLayout *layout = this->layout();
    if (layout) {
        while (QLayoutItem *item = layout->takeAt(0)) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
    }

    QVBoxLayout *victoryLayout = new QVBoxLayout(this);

    QLabel *congratsLabel = new QLabel(" ПОБЕДА! ", this);
    congratsLabel->setAlignment(Qt::AlignCenter);
    congratsLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: green; margin: 30px;");

    QLabel *messageLabel = new QLabel(
        "Вы прошли все фазы!\n"
        "Окно наконец-то сдалось.\n\n"
        "Вы настоящий мастер!",
        this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 20px; margin: 30px; color: #333;");

    closeButton = new QPushButton(" Закрыть по-настоящему ", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 15px 30px;"
        "   border-radius: 12px;"
        "   border: 2px solid #45a049;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "   border: 2px solid #4CAF50;"
        "   font-size: 20px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}"
        );

    connect(closeButton, &QPushButton::clicked, []() {
        QApplication::quit();
    });

    victoryLayout->addWidget(congratsLabel);
    victoryLayout->addWidget(messageLabel);
    victoryLayout->addWidget(closeButton, 0, Qt::AlignCenter);
    setLayout(victoryLayout);

    emit phaseComplete();
}