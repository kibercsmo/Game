#ifndef PHASE4WIDGET_H
#define PHASE4WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <qpushbutton.h>

class Phase4Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Phase4Widget(QWidget *parent = nullptr);
    ~Phase4Widget();

    void setActive(bool active);
    bool isActive() const { return m_active; }

signals:
    void phaseComplete();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void jumpToRandomPosition();
    void randomResize();
    void updateWindowTitle();

private:
    QTimer *jumpTimer;
    QTimer *resizeTimer;
    QTimer *titleTimer;
    QLabel *victoryLabel;
    QPushButton *closeButton;
    QStringList titleSequence;
    int titleIndex;
    bool gameActive;
    bool m_active;

    void setupUI();
    void startCrazyEffects();
    void stopAllTimers();
    void showVictoryScreen();
};

#endif // PHASE4WIDGET_H