#include "cloneWindow.h"

#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QCloseEvent>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QStyle>

CloneWindow::CloneWindow(bool isSpecial, QWidget *parent)
    : QWidget(parent)
    , m_isSpecial(isSpecial)
{
    setupUI();

    setWindowFlags(Qt::Window);

    int randomWidth = QRandomGenerator::global()->bounded(250, 400);
    int randomHeight = QRandomGenerator::global()->bounded(150, 300);
    resize(randomWidth, randomHeight);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();

        int maxX = screenGeometry.width() - width() + 100;
        int maxY = screenGeometry.height() - height() + 100;

        if (maxX > 0 && maxY > 0) {
            int x = QRandomGenerator::global()->bounded(maxX);
            int y = QRandomGenerator::global()->bounded(maxY);
            move(x, y);

            qDebug() << "Clone window positioned at:" << x << y
                     << "screen size:" << screenGeometry.width() << "x" << screenGeometry.height()
                     << "window size:" << width() << "x" << height();
        } else {
            move(50, 50);
            qDebug() << "Window too large for screen, positioning at 50,50";
        }
    } else {
        move(100, 100);
        qDebug() << "No screen found, positioning at 100,100";
    }

    setWindowTitle(m_isSpecial ? "ОСОБОЕ ОКНО!" : "Окно-клон");
}

CloneWindow::~CloneWindow()
{
}

void CloneWindow::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Текст-дразнилка
    QStringList messages;
    if (m_isSpecial) {
        messages << " Я особенное окно! "
                 << " Нажми кнопку! "
                 << " Ты нашел меня! "
                 << " ПОБЕДНОЕ ОКНО! ";
    } else {
        messages << " Не это окно! "
                 << " Попробуй другое! "
                 << " Закрой меня, если сможешь! "
                 << " Меня не так просто закрыть! "
                 << " Размножение продолжается... "
                 << " Не то окно! "
                 << " Попробуй ещё! ";
    }

    int index = QRandomGenerator::global()->bounded(messages.size());
    messageLabel = new QLabel(messages[index], this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 14px; margin: 20px; font-weight: bold;");

    layout->addWidget(messageLabel);

    if (m_isSpecial) {
        surrenderButton = new QPushButton(" Сдаюсь, ты победил! ", this);
        surrenderButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "   padding: 10px;"
            "   border-radius: 8px;"
            "   border: 2px solid #45a049;"
            "}"
            "QPushButton:hover {"
            "   background-color: #45a049;"
            "   border: 2px solid #4CAF50;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #3d8b40;"
            "}"
            );
        connect(surrenderButton, &QPushButton::clicked, this, &CloneWindow::onSurrenderClicked);
        layout->addWidget(surrenderButton);
    }

    if (m_isSpecial) {
        setStyleSheet(
            "QWidget {"
            "   background-color: #90EE90;"
            "   border: 3px solid #228B22;"
            "   border-radius: 10px;"
            "}"
            );
    } else {
        QStringList colors = {"#F0F0F0", "#FFE4E1", "#E0E0E0", "#F5F5DC", "#FFDAB9"};
        int colorIndex = QRandomGenerator::global()->bounded(colors.size());
        setStyleSheet(
            "QWidget {"
            "   background-color: " + colors[colorIndex] + ";"
                                   "   border: 1px solid #CCCCCC;"
                                   "   border-radius: 5px;"
                                   "}"
            );
    }

    setLayout(layout);
}

void CloneWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Clone window closing, isSpecial:" << m_isSpecial << "position:" << pos();
    emit closed(this);
    event->accept();
}

void CloneWindow::onSurrenderClicked()
{
    qDebug() << "Surrender button clicked in special window at position:" << pos();
    emit specialSurrendered();
}