#ifndef CLONEWINDOW_H
#define CLONEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class CloneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CloneWindow(bool isSpecial, QWidget *parent = nullptr);
    ~CloneWindow();

    bool isSpecial() const { return m_isSpecial; }

signals:
    void closed(CloneWindow *window);
    void specialSurrendered();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSurrenderClicked();

private:
    bool m_isSpecial;
    QPushButton *surrenderButton;
    QLabel *messageLabel;

    void setupUI();
};

#endif // CLONEWINDOW_H