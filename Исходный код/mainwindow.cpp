#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <QStyle>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QPainter>
#include <QTimer>
#include <QLinearGradient>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

    bgAnimationTimer = new QTimer(this);
    connect(bgAnimationTimer, &QTimer::timeout, this, &MainWindow::animateBackground);
    bgAnimationTimer->start(50);

    ui->centralwidget->setStyleSheet("#centralwidget { background: transparent; }");

    ui->position->setStyleSheet(
        "QComboBox {"
        " background-color: white;"
        " border: 1px solid #ccc;"
        " border-radius: 3px;"
        " padding: 5px;"
        " min-width: 200px;"
        " color: black;"
        "}"
        "QComboBox QAbstractItemView {"
        " color: black;"
        " background-color: white;"
        "}"
    );

    ui->logIn->setStyleSheet(
        "QPushButton {"
        " background-color: #4CAF50;"
        " color: white;"
        " border: none;"
        " padding: 8px 16px;"
        " border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3e8e41; }"
    );

    ui->SignUp->setStyleSheet(
        "QPushButton {"
        " background-color: #4CAF50;"
        " color: white;"
        " border: none;"
        " padding: 8px 16px;"
        " border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3e8e41; }"
    );

    setupButtonAnimations();
    setWindowTitle("University Management System");

    QFont titleFont("Arial", 16, QFont::Bold);
    ui->label->setFont(titleFont);
    ui->label->setStyleSheet("color: #2c3e50;");
    ui->label->setAlignment(Qt::AlignCenter);

    QFont titleFont1("Arial", 16, QFont::Bold);
    ui->label_2->setFont(titleFont1);
    ui->label_2->setStyleSheet("color: #2c3e50;");
    ui->label_2->setAlignment(Qt::AlignCenter);

    QFont titleFont2("Arial", 16, QFont::Bold);
    ui->label_3->setFont(titleFont2);
    ui->label_3->setStyleSheet("color: #2c3e50;");
    ui->label_3->setAlignment(Qt::AlignCenter);

    ui->login->setPlaceholderText("Enter your username");
    ui->password->setPlaceholderText("Enter your password");
    ui->password->setEchoMode(QLineEdit::Password);

    ui->position->setStyleSheet("QComboBox { combobox-popup: 0; }");
    ui->password->setStyleSheet(
        "QLineEdit {"
        " padding-right: 30px;"
        " border: 1px solid #ccc;"
        " border-radius: 3px;"
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
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);

    QPropertyAnimation *closeAnim = new QPropertyAnimation(closeButton, "geometry", this);
    closeAnim->setDuration(300);
    closeAnim->setStartValue(QRect(width() - 34, -30, 24, 24));
    closeAnim->setEndValue(QRect(width() - 34, 10, 24, 24));
    closeAnim->setEasingCurve(QEasingCurve::OutBack);
    closeAnim->start();

    togglePasswordButton = new QPushButton(this);
    togglePasswordButton->setStyleSheet(
        "QPushButton {"
        " border: none;"
        " background-color: transparent;"
        " padding: 0px;"
        "}"
    );
    togglePasswordButton->setCursor(Qt::PointingHandCursor);
    togglePasswordButton->setFixedSize(24, 24);

    QHBoxLayout *passwordLayout = new QHBoxLayout(ui->password);
    passwordLayout->addStretch();
    passwordLayout->addWidget(togglePasswordButton);
    passwordLayout->setContentsMargins(0, 0, 5, 0);
    ui->password->setLayout(passwordLayout);

    connect(togglePasswordButton, &QPushButton::clicked, this, &MainWindow::on_togglePasswordVisibility_clicked);
}

//Обработка видимости пароля
void MainWindow::on_togglePasswordVisibility_clicked()
{
    if (ui->password->echoMode() == QLineEdit::Password) {
        ui->password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->password->setEchoMode(QLineEdit::Password);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, bgColor1);
    gradient.setColorAt(1, bgColor2);
    painter.fillRect(rect(), gradient);
    QMainWindow::paintEvent(event);
}

//Анимация фона окна
void MainWindow::animateBackground()
{
    bgAnimationStep += 0.005f;
    if (bgAnimationStep > 1.0f) {
        bgAnimationStep = 0.0f;
        QColor temp = bgColor1;
        bgColor1 = bgColor2;
        bgColor2 = temp;
    }

    QColor currentColor1 = QColor(
        bgColor1.red() * (1 - bgAnimationStep) + bgColor2.red() * bgAnimationStep,
        bgColor1.green() * (1 - bgAnimationStep) + bgColor2.green() * bgAnimationStep,
        bgColor1.blue() * (1 - bgAnimationStep) + bgColor2.blue() * bgAnimationStep
    );

    QColor currentColor2 = QColor(
        bgColor2.red() * (1 - bgAnimationStep) + bgColor1.red() * bgAnimationStep,
        bgColor2.green() * (1 - bgAnimationStep) + bgColor1.green() * bgAnimationStep,
        bgColor2.blue() * (1 - bgAnimationStep) + bgColor1.blue() * bgAnimationStep
    );

    bgColor1 = currentColor1;
    bgColor2 = currentColor2;
    update();
}

//Обработка анимации кнопок
void MainWindow::setupButtonAnimations()
{
    QPropertyAnimation *logInAnimation = new QPropertyAnimation(ui->logIn, "geometry", this);
    logInAnimation->setDuration(100);
    logInAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(ui->logIn, &QPushButton::pressed, [=]() {
        logInAnimation->stop();
        QRect startGeometry = ui->logIn->geometry();
        logInAnimation->setStartValue(startGeometry);
        logInAnimation->setEndValue(startGeometry.adjusted(2, 2, -2, -2));
        logInAnimation->start();
    });
    connect(ui->logIn, &QPushButton::released, [=]() {
        logInAnimation->stop();
        QRect startGeometry = ui->logIn->geometry();
        logInAnimation->setStartValue(startGeometry);
        logInAnimation->setEndValue(startGeometry.adjusted(-2, -2, 2, 2));
        logInAnimation->start();
    });

    QPropertyAnimation *signUpAnimation = new QPropertyAnimation(ui->SignUp, "geometry", this);
    signUpAnimation->setDuration(100);
    signUpAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(ui->SignUp, &QPushButton::pressed, [=]() {
        signUpAnimation->stop();
        QRect startGeometry = ui->SignUp->geometry();
        signUpAnimation->setStartValue(startGeometry);
        signUpAnimation->setEndValue(startGeometry.adjusted(2, 2, -2, -2));
        signUpAnimation->start();
    });
    connect(ui->SignUp, &QPushButton::released, [=]() {
        signUpAnimation->stop();
        QRect startGeometry = ui->SignUp->geometry();
        signUpAnimation->setStartValue(startGeometry);
        signUpAnimation->setEndValue(startGeometry.adjusted(-2, -2, 2, 2));
        signUpAnimation->start();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    bgAnimationTimer->stop();
    delete bgAnimationTimer;
}

//Обработка нажатия на кнопку для входа в систему
void MainWindow::on_logIn_clicked()
{
    QString username = ui->login->text();
    QString password = ui->password->text();
    QString position = ui->position->currentText();

    if (position == "Tutor") {
        if (Database::validateUser(username, password)) {
            QMessageBox::information(this, "Success!", "You are logged in!");
            cp = new ControlPanel();
            cp->show();
            this->hide();
        } else {
            QMessageBox::information(this, "Error!", "Invalid username or password!");
        }
    } else {
        if (Database::studentExists(username)) {
            QSqlQuery query;
            query.prepare("SELECT password FROM students WHERE name = :name");
            query.bindValue(":name", username);
            if (query.exec() && query.next()) {
                QString storedPassword = query.value(0).toString();
                if (storedPassword == password) {
                    QMessageBox::information(this, "Success!", "You are logged in!");
                    up = new UserPanel(username, this);
                    up->show();
                    this->hide();
                } else {
                    QMessageBox::information(this, "Error!", "Incorrect password!");
                }
            }
        } else {
            QMessageBox::information(this, "Error!", "No such student!");
        }
    }
}

//Обработка нажатия на кнопку регистрации
void MainWindow::on_SignUp_clicked()
{
    if (ui->position->currentText() == "Tutor") {
        QString username = ui->login->text();
        QString password = ui->password->text();

        if (Database::userExists(username)) {
            QMessageBox::information(this, "Error!", "User already exists!");
        } else {
            if (Database::addUser(username, password)) {
                QMessageBox::information(this, "Success!", "User registered successfully!");
            } else {
                QMessageBox::information(this, "Error!", "Failed to register user!");
            }
        }
    } else {
        QMessageBox::warning(this, "Error!", "Students cannot register themselves!");
    }
}
