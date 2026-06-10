#ifndef PHASE1WIDGET_H
#define PHASE1WIDGET_H

#include <QWidget>
#include <QLabel>
#include "runawayButton.h"

class Phase1Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Phase1Widget(QWidget *parent = nullptr);
    ~Phase1Widget();

    void setActive(bool active);
    bool isActive() const { return m_active; }

signals:
    void phaseComplete();

private slots:
    void onButtonClicked();

private:
    RunawayButton *runawayButton;
    QLabel *instructionLabel;
    QLabel *hintLabel;
    bool m_active;

    void setupUI();
};

#endif // PHASE1WIDGET_H