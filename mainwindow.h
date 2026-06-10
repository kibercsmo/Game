#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "phase1Widget.h"
#include "phase2Widget.h"
#include "phase3Widget.h"
#include "phase4Widget.h"
#include <QWidget>
#include <QStackedWidget>
#include <QCloseEvent>
#include <QKeyEvent>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void goToPhase(int phase);
    int getCurrentPhase() const { return currentPhase; }

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onPhase1Complete();
    void onPhase2Complete();
    void onPhase3Complete();
    void onPhase4Complete();

private:
    QStackedWidget *stackedWidget;
    int currentPhase;

    Phase1Widget *phase1;
    Phase2Widget *phase2;
    Phase3Widget *phase3;
    Phase4Widget *phase4;

    void setActivePhase(int phase);
};

#endif // MAINWINDOW_H