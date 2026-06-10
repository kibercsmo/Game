#ifndef PHASE3WIDGET_H
#define PHASE3WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include "cloneWindow.h"

class Phase3Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Phase3Widget(QWidget *parent = nullptr);
    ~Phase3Widget();

    void setActive(bool active);
    bool isActive() const { return m_active; }

signals:
    void phaseComplete();

private slots:
    void onCloneClosed(CloneWindow *window);
    void onSpecialSurrendered();
    void createTwoMoreClones(CloneWindow *original);

private:
    QList<CloneWindow*> clones;
    QLabel *instructionLabel;
    bool phaseCompleted;
    bool m_active;

    void setupUI();
    void createInitialClones();
    void createClone(bool isSpecial);
    void closeAllClones();
    int getCloneCount() const;
    bool hasSpecialClone() const;
    CloneWindow* getRandomNonSpecialClone() const;
};

#endif // PHASE3WIDGET_H