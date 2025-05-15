#include "removestudent.h"
#include "ui_removestudent.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMessageBox>

RemoveStudent::RemoveStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveStudent)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start();

    //Добавление стиля окна
    setStyleSheet(
        "QDialog {"
        " background: transparent;"
        "}"
        "QLabel {"
        " color: white;"
        " font-weight: bold;"
        " background: transparent;"
        "}"
        "QLineEdit {"
        " background-color: rgba(255, 255, 255, 200);"
        " border: 1px solid #ccc;"
        " border-radius: 5px;"
        " padding: 8px;"
        " min-width: 200px;"
        "}"
    );

    ui->removestudent->setStyleSheet(
        "QPushButton {"
        " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
        " stop:0 #4CAF50, stop:1 #2E7D32);"
        " color: white;"
        " border: none;"
        " padding: 10px 20px;"
        " border-radius: 5px;"
        " font-weight: bold;"
        "}"
        "QPushButton:hover {"
        " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
        " stop:0 #66BB6A, stop:1 #388E3C);"
        "}"
        "QPushButton:pressed {"
        " background-color: #1B5E20;"
        "}"
    );

    QPushButton *closeButton = new QPushButton(this);
    closeButton->setText("×");
    closeButton->setStyleSheet(
        "QPushButton {"
        " background-color: rgba(255, 255, 255, 0.3);"
        " color: white;"
        " border: none;"
        " border-radius: 12px;"
        " min-width: 24px;"
        " max-width: 24px;"
        " min-height: 24px;"
        " max-height: 24px;"
        " font-size: 18px;"
        " font-weight: bold;"
        "}"
        "QPushButton:hover {"
        " background-color: rgba(255, 50, 50, 0.7);"
        "}"
        "QPushButton:pressed {"
        " background-color: rgba(255, 0, 0, 0.9);"
        "}"
    );

    closeButton->move(width() - 34, 10);
    closeButton->raise();
    connect(closeButton, &QPushButton::clicked, this, &RemoveStudent::close);

    QPropertyAnimation *closeAnim = new QPropertyAnimation(closeButton, "geometry", this);
    closeAnim->setDuration(300);
    closeAnim->setStartValue(QRect(width() - 34, -30, 24, 24));
    closeAnim->setEndValue(QRect(width() - 34, 10, 24, 24));
    closeAnim->setEasingCurve(QEasingCurve::OutBack);
    closeAnim->start();

    QTimer *bgTimer = new QTimer(this);
    connect(bgTimer, &QTimer::timeout, this, [this](){ update(); });
    bgTimer->start(50);
}

//Добавление анимации
void RemoveStudent::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(100, 150, 255));
    gradient.setColorAt(1, QColor(220, 120, 220));
    painter.fillRect(rect(), gradient);

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.fillPath(path, gradient);
}

RemoveStudent::~RemoveStudent()
{
    delete ui;
}

//Обработка нажатия кнопки для удаления студента
void RemoveStudent::on_removestudent_clicked()
{
    QString studentName = ui->lusername->text();
    QString password = ui->lpassword->text();

    if (password != "qwerty") {
        QMessageBox::information(this, "Error!", "Wrong password!");
        return;
    }

    if (!Database::studentExists(studentName)) {
        QMessageBox::information(this, "Error", QString("Student '%1' not found").arg(studentName));
        return;
    }

    if (Database::removeStudent(studentName)) {
        QMessageBox::information(this, "Success!", "Student removed successfully!");
    } else {
        QMessageBox::information(this, "Error!", "Failed to remove student!");
    }
}
