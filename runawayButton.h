#ifndef RUNAWAYBUTTON_H
#define RUNAWAYBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QEnterEvent>

class RunawayButton : public QPushButton
{
    Q_OBJECT

public:
    explicit RunawayButton(QWidget *parent = nullptr);
    ~RunawayButton();

protected:
    void enterEvent(QEnterEvent *event) override;

private:
    void moveToRandomPosition();
};

#endif // RUNAWAYBUTTON_H