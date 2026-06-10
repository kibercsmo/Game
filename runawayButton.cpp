#include "runawayButton.h"
#include <QRandomGenerator>
#include <QWidget>
#include <QDebug>
#include <cmath>

RunawayButton::RunawayButton(QWidget *parent)
    : QPushButton(parent)
{
    setText("ВЫХОД");
    setFixedSize(150, 60);

    setStyleSheet(
        "QPushButton {"
        "   background-color: red;"
        "   color: white;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:focus {"
        "   background-color: darkred;"
        "   outline: 2px solid yellow;"
        "}"
        "QPushButton:hover {"
        "   background-color: darkred;"
        "}"
        );
}

RunawayButton::~RunawayButton()
{
}

void RunawayButton::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Mouse entered button - moving away!";
    moveToRandomPosition();
}

void RunawayButton::moveToRandomPosition()
{
    if (!parentWidget()) return;

    QWidget *parent = parentWidget();
    int parentWidth = parent->width();
    int parentHeight = parent->height();

    int buttonWidth = width();
    int buttonHeight = height();

    int margin = 20;
    int maxX = parentWidth - buttonWidth - margin;
    int maxY = parentHeight - buttonHeight - margin;

    if (maxX <= margin || maxY <= margin) return;

    QPoint cursorPos = QCursor::pos();
    QPoint localCursorPos = parent->mapFromGlobal(cursorPos);

    int newX, newY;
    bool tooClose;
    int attempts = 0;

    do {
        tooClose = false;
        newX = margin + QRandomGenerator::global()->bounded(maxX - margin);
        newY = margin + QRandomGenerator::global()->bounded(maxY - margin);

        QRect buttonRect(newX, newY, buttonWidth, buttonHeight);
        if (buttonRect.contains(localCursorPos)) {
            tooClose = true;
        }

        QPoint currentPos = pos();
        int dx = newX - currentPos.x();
        int dy = newY - currentPos.y();
        int distance = std::sqrt(dx*dx + dy*dy);

        if (distance < 100 && (maxX > 200 && maxY > 200)) {
            tooClose = true;
        }

        if (newX < margin || newY < margin ||
            newX + buttonWidth > parentWidth - margin ||
            newY + buttonHeight > parentHeight - margin) {
            tooClose = true;
        }

        attempts++;
        if (attempts > 20) {
            if (localCursorPos.x() < parentWidth / 2) {
                newX = parentWidth - buttonWidth - margin;
            } else {
                newX = margin;
            }
            if (localCursorPos.y() < parentHeight / 2) {
                newY = parentHeight - buttonHeight - margin;
            } else {
                newY = margin;
            }
            break;
        }

    } while (tooClose);

    move(newX, newY);
}