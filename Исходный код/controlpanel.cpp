#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QSequentialAnimationGroup>
#include <QPainter>
#include <QTimer>
#include <QtMath>

ControlPanel::ControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    bgTimer = new QTimer(this);
    connect(bgTimer, &QTimer::timeout, this, &ControlPanel::updateBackground);
    bgTimer->start(50);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setWindowOpacity(0.0);
    setupButtonAnimations();

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
    connect(closeButton, &QPushButton::clicked, this, &ControlPanel::close);

    QPropertyAnimation *closeAnim = new QPropertyAnimation(closeButton, "geometry", this);
    closeAnim->setDuration(300);
    closeAnim->setStartValue(QRect(width() - 34, -30, 24, 24));
    closeAnim->setEndValue(QRect(width() - 34, 10, 24, 24));
    closeAnim->setEasingCurve(QEasingCurve::OutBack);
    closeAnim->start();

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start();
}

void ControlPanel::setupButtonAnimations()
{
    QList<QPushButton*> buttons = {
        ui->addstudent,
        ui->removestudent,
        ui->selectsubject,
        ui->ratings,
        ui->add_homework
    };

    QString buttonStyle =
        "QPushButton {"
        " background-color: #4CAF50;"
        " color: white;"
        " border: none;"
        " padding: 12px 24px;"
        " text-align: center;"
        " text-decoration: none;"
        " font-size: 14px;"
        " margin: 8px 4px;"
        " border-radius: 6px;"
        " transition: all 0.3s ease;"
        "}"
        "QPushButton:hover {"
        " background-color: #45a049;"
        " transform: translateY(-2px);"
        " box-shadow: 0 4px 8px rgba(0,0,0,0.1);"
        "}"
        "QPushButton:pressed {"
        " transform: translateY(1px);"
        " box-shadow: none;"
        "}";

    foreach (QPushButton *btn, buttons) {
        btn->setStyleSheet(buttonStyle);
        btn->setGraphicsEffect(nullptr);
    }

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    for (int i = 0; i < buttons.size(); ++i) {
        QPushButton *btn = buttons[i];
        btn->setProperty("opacity", 0.0);
        QPropertyAnimation *anim = new QPropertyAnimation(btn, "opacity");
        anim->setDuration(400);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutBack);
        group->addPause(i * 100);
        group->addAnimation(anim);
    }
    group->start();
}

void ControlPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, bgColor1);
    gradient.setColorAt(1, bgColor2);

    QPainterPath path;
    path.moveTo(0, height());
    for (int x = 0; x <= width(); x += 20) {
        float y = height() - 50 - 30 * sin(x * 0.02 + bgOffset);
        path.lineTo(x, y);
    }
    path.lineTo(width(), height());
    path.closeSubpath();
    painter.fillPath(path, gradient);

    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 15; ++i) {
        float x = qSin(i * 0.7 + bgOffset * 2) * width()/2 + width()/2;
        float y = qCos(i * 0.5 + bgOffset) * height()/2 + height()/2;
        float size = 5 + 3 * qSin(i + bgOffset * 3);
        painter.setBrush(QColor(255, 255, 255, 50));
        painter.drawEllipse(QPointF(x, y), size, size);
    }

    QWidget::paintEvent(event);
}

void ControlPanel::updateBackground()
{
    bgOffset += 0.03;
    update();
}

ControlPanel::~ControlPanel()
{
    bgTimer->stop();
    delete bgTimer;
    delete ui;
}

void ControlPanel::on_addstudent_clicked()
{
    add_student = new AddStudent();
    add_student->show();
}

void ControlPanel::on_removestudent_clicked()
{
    rs = new RemoveStudent();
    rs->show();
}

void ControlPanel::on_selectsubject_clicked()
{
    marks = new Marks();
    marks->show();
}

void ControlPanel::on_ratings_clicked()
{
    rt = new Ratings();
    rt->show();
}

void ControlPanel::on_add_homework_clicked()
{
    homework = new Homework();
    homework->show();
}
