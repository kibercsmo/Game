#ifndef PHASE2WIDGET_H
#define PHASE2WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCloseEvent>

class Phase2Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Phase2Widget(QWidget *parent = nullptr);
    ~Phase2Widget();

    void setActive(bool active);
    bool isActive() const { return m_active; }

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void phaseComplete();

private slots:
    void checkCodeWord();

private:
    QLabel *instructionLabel;
    QLabel *hintLabel;
    QLineEdit *secretInput;
    bool m_active;

    void setupUI();
    void showSecretInput();
};

#endif // PHASE2WIDGET_H