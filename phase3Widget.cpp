#include "phase3Widget.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QRandomGenerator>
#include <QDebug>

Phase3Widget::Phase3Widget(QWidget *parent)
    : QWidget(parent)
    , phaseCompleted(false)
    , m_active(false)
{
    setupUI();
}

Phase3Widget::~Phase3Widget()
{
    closeAllClones();
}

void Phase3Widget::setActive(bool active)
{
    m_active = active;
    if (active && !phaseCompleted) {
        QTimer::singleShot(500, this, &Phase3Widget::createInitialClones);
    } else if (!active && !phaseCompleted) {
        closeAllClones();
    }
}

void Phase3Widget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    instructionLabel = new QLabel("Ты думал, это конец?", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 28px; font-weight: bold; margin: 50px;");

    mainLayout->addWidget(instructionLabel);
    setLayout(mainLayout);
}

void Phase3Widget::createInitialClones()
{
    if (!m_active || phaseCompleted) return;

    for (int i = 0; i < 20; ++i) {
        bool isSpecial = (i == 0);
        createClone(isSpecial);
    }
}

void Phase3Widget::createClone(bool isSpecial)
{
    if (!m_active || phaseCompleted) return;

    if (getCloneCount() >= 50) {
        return;
    }

    CloneWindow *clone = new CloneWindow(isSpecial);
    clones.append(clone);

    connect(clone, &CloneWindow::closed, this, &Phase3Widget::onCloneClosed);
    connect(clone, &CloneWindow::specialSurrendered, this, &Phase3Widget::onSpecialSurrendered);

    clone->show();
}

void Phase3Widget::onCloneClosed(CloneWindow *window)
{
    if (!m_active || phaseCompleted) return;

    bool wasSpecial = window->isSpecial();

    clones.removeAll(window);
    window->deleteLater();

    if (!phaseCompleted && m_active) {
        if (wasSpecial) {
            createClone(true);
            createClone(false);
        } else {
            createClone(false);
            createClone(false);
        }
    }
}

void Phase3Widget::onSpecialSurrendered()
{
    if (!m_active || phaseCompleted) return;

    phaseCompleted = true;
    closeAllClones();
    emit phaseComplete();
}

void Phase3Widget::createTwoMoreClones(CloneWindow *original)
{
    Q_UNUSED(original);
    if (!m_active || phaseCompleted) return;

    createClone(false);
    createClone(false);
}

void Phase3Widget::closeAllClones()
{
    for (CloneWindow *clone : clones) {
        clone->close();
        clone->deleteLater();
    }
    clones.clear();
}

int Phase3Widget::getCloneCount() const
{
    return clones.size();
}

bool Phase3Widget::hasSpecialClone() const
{
    for (CloneWindow *clone : clones) {
        if (clone->isSpecial()) return true;
    }
    return false;
}

CloneWindow* Phase3Widget::getRandomNonSpecialClone() const
{
    QList<CloneWindow*> nonSpecial;
    for (CloneWindow *clone : clones) {
        if (!clone->isSpecial()) nonSpecial.append(clone);
    }
    if (nonSpecial.isEmpty()) return nullptr;
    int index = QRandomGenerator::global()->bounded(nonSpecial.size());
    return nonSpecial[index];
}