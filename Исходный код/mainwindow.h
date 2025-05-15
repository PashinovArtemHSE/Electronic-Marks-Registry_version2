#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controlpanel.h"
#include "userpanel.h"
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QEasingCurve>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_logIn_clicked();
    void animateBackground();
    void on_SignUp_clicked();
    void on_togglePasswordVisibility_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    ControlPanel *cp;
    UserPanel *up;
    QStackedWidget *stackedWidget;
    QTimer *bgAnimationTimer;
    QPushButton *togglePasswordButton;
    float bgAnimationStep = 0.0f;
    QColor bgColor1 = QColor(100, 150, 255);
    QColor bgColor2 = QColor(255, 100, 150);

    QWidget* createLoginPage();
    QWidget* createRegisterPage();
    void setupButtonAnimations();
    void initializeData();
};

#endif // MAINWINDOW_H
