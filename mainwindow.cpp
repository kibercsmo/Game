#include "mainwindow.h"
#include "phase1Widget.h"
#include "phase2Widget.h"
#include "phase3Widget.h"
#include "phase4Widget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QRandomGenerator>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , currentPhase(0)
{
    setWindowTitle("Окно, которое не хочет умирать");
    resize(600, 500);

    stackedWidget = new QStackedWidget(this);

    phase1 = new Phase1Widget();
    phase2 = new Phase2Widget();
    phase3 = new Phase3Widget();
    phase4 = new Phase4Widget();

    stackedWidget->addWidget(phase1);
    stackedWidget->addWidget(phase2);
    stackedWidget->addWidget(phase3);
    stackedWidget->addWidget(phase4);

    connect(phase1, &Phase1Widget::phaseComplete, this, &MainWindow::onPhase1Complete);
    connect(phase2, &Phase2Widget::phaseComplete, this, &MainWindow::onPhase2Complete);
    connect(phase3, &Phase3Widget::phaseComplete, this, &MainWindow::onPhase3Complete);
    connect(phase4, &Phase4Widget::phaseComplete, this, &MainWindow::onPhase4Complete);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    setActivePhase(0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setActivePhase(int phase)
{
    phase1->setActive(false);
    phase2->setActive(false);
    phase3->setActive(false);
    phase4->setActive(false);

    switch(phase) {
    case 0: phase1->setActive(true); break;
    case 1: phase2->setActive(true); break;
    case 2: phase3->setActive(true); break;
    case 3: phase4->setActive(true); break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (currentPhase == 0 || currentPhase == 1) {
        // Фаза 2: игнорируем закрытие и перемещаем окно
        if (currentPhase == 1 && phase2->isActive()) {
            event->ignore();

            // Перемещаем окно в случайную точку экрана
            QScreen *screen = QGuiApplication::primaryScreen();
            if (screen) {
                QRect screenGeometry = screen->availableGeometry();
                int maxX = screenGeometry.width() - width();
                int maxY = screenGeometry.height() - height();

                if (maxX > 0 && maxY > 0) {
                    int x = QRandomGenerator::global()->bounded(maxX);
                    int y = QRandomGenerator::global()->bounded(maxY);
                    move(x, y);
                }
            }
        }
        else if (currentPhase == 0) {
            event->ignore();
        } else {
            event->accept();
        }
    } else {
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
}

void MainWindow::goToPhase(int phase)
{
    currentPhase = phase;
    stackedWidget->setCurrentIndex(phase);
    setWindowTitle(QString("Окно, которое не хочет умирать - Фаза %1").arg(phase + 1));
    setActivePhase(phase);
}

void MainWindow::onPhase1Complete()
{
    goToPhase(1);
}

void MainWindow::onPhase2Complete()
{
    goToPhase(2);
}

void MainWindow::onPhase3Complete()
{
    goToPhase(3);
}

void MainWindow::onPhase4Complete()
{

}