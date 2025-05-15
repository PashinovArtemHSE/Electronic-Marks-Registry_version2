#include "addstudent.h"
#include "ui_addstudent.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLinearGradient>
#include <QMessageBox>

AddStudent::AddStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudent)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    //добавляем анимацию
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start();

    setStyleSheet(
        "QDialog {"
        " background: transparent;"
        "}"
        "QLineEdit, QComboBox {"
        " background-color: rgba(255, 255, 255, 200);"
        " border: 1px solid #ccc;"
        " border-radius: 5px;"
        " padding: 8px;"
        " font-size: 14px;"
        " min-width: 200px;"
        "}"
        "QLabel {"
        " color: white;"
        " font-weight: bold;"
        " background: transparent;"
        "}"
    );

    ui->addst->setStyleSheet(
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
    connect(closeButton, &QPushButton::clicked, this, &AddStudent::close);

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

//анимация окна
void AddStudent::paintEvent(QPaintEvent *event)
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

AddStudent::~AddStudent()
{
    delete ui;
}

//обработка нажатия на кнопку для добавления студента
void AddStudent::on_addst_clicked()
{
    QString name = ui->name->text();
    QString password = ui->passwd->text();
    QString group = ui->group->text();

    if (name.isEmpty() || password.isEmpty() || group.isEmpty()) {
        QMessageBox::information(this, "Error!", "All fields must be filled!");
        return;
    }

    if (Database::studentExists(name)) {
        QMessageBox::information(this, "Error!", "Student already exists!");
        return;
    }

    if (Database::addStudent(name, password, group)) {
        QPropertyAnimation *anim = new QPropertyAnimation(ui->addst, "geometry", this);
        anim->setDuration(300);
        anim->setStartValue(ui->addst->geometry());
        anim->setEndValue(ui->addst->geometry().adjusted(-5, -5, 5, 5));
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->start();

        QMessageBox::information(this, "Success!", "Student added successfully!");
        this->hide();
    } else {
        QMessageBox::information(this, "Error!", "Failed to add student!");
    }
}
