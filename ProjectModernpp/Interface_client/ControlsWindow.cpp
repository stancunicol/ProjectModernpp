#include "ControlsWindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

ControlsWindow::ControlsWindow(QWidget* parent)
    : QDialog(parent),
    m_upKeyEdit(new QLineEdit("W", this)),
    m_downKeyEdit(new QLineEdit("S", this)),
    m_leftKeyEdit(new QLineEdit("A", this)),
    m_rightKeyEdit(new QLineEdit("D", this)),
    m_fireKeyEdit(new QLineEdit("Space", this))
{
    setWindowTitle("Game Controls");
    setFixedSize(600, 500);

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(20, 20, 20, 245));
    setPalette(palette);
    setAutoFillBackground(true);

    QLabel* instructionLabel = new QLabel(
        "<b>Set Your Game Controls</b><br>"
        "Click on a field and press a key to assign it.", this);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("color: white;");

    QLabel* gameLegendLabel = new QLabel(
        "<b>Game Legend:</b>"
        "<ul>"
        "<li><b>Goal:</b> Destroy the enemy base to win the game!</li>"
        "<li><b>Movement:</b> Use arrow keys or WASD to move your tank.</li>"
        "<li><b>Actions:</b> Use 'Space' to shoot your weapon.</li>"
        "<li><b>Walls:</b> Destroyable walls can be broken to open paths, while indestructible walls block the way.</li>"
        "<li><b>Bases:</b> Shout the base in order to win faster.</li>"
        "<li><b>Bombs:</b> Explosions triggered by destroyed walls can eliminate anyone in a 10-meter radius.</li>"
        "</ul>"
        "<br><b>Player Actions:</b>"
        "<ul>"
        "<li>Each enemy killed gives you 100 points.</li>"
        "<li>The winner of the game gets an extra 200 points.</li>"
        "<li>After collecting enough points, improve your weapon's power.</li>"
        "</ul>"
        "<br>Good luck and be strategic!", this);
    gameLegendLabel->setAlignment(Qt::AlignCenter);
    gameLegendLabel->setStyleSheet("color: white;");

    setupKeyEdit(m_upKeyEdit);
    setupKeyEdit(m_downKeyEdit);
    setupKeyEdit(m_leftKeyEdit);
    setupKeyEdit(m_rightKeyEdit);
    setupKeyEdit(m_fireKeyEdit);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel("Move Up:", this), 0, 0);
    gridLayout->addWidget(m_upKeyEdit, 0, 1);
    gridLayout->addWidget(new QLabel("Move Down:", this), 1, 0);
    gridLayout->addWidget(m_downKeyEdit, 1, 1);
    gridLayout->addWidget(new QLabel("Move Left:", this), 2, 0);
    gridLayout->addWidget(m_leftKeyEdit, 2, 1);
    gridLayout->addWidget(new QLabel("Move Right:", this), 3, 0);
    gridLayout->addWidget(m_rightKeyEdit, 3, 1);
    gridLayout->addWidget(new QLabel("Fire:", this), 4, 0);
    gridLayout->addWidget(m_fireKeyEdit, 4, 1);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(gameLegendLabel);
    mainLayout->addLayout(gridLayout);
}

void ControlsWindow::setupKeyEdit(QLineEdit * keyEdit)
{
    keyEdit->setAlignment(Qt::AlignCenter);
    keyEdit->setFocusPolicy(Qt::StrongFocus);
    keyEdit->setReadOnly(true); 
}

void ControlsWindow::keyPressEvent(QKeyEvent * event)
{
    QWidget* focusedWidget = focusWidget();
    if (!focusedWidget)
        return;

    QString pressedKey;
    switch (event->key()) {
    case Qt::Key_Up: pressedKey = "↑"; break;
    case Qt::Key_Down: pressedKey = "↓"; break;
    case Qt::Key_Left: pressedKey = "←"; break;
    case Qt::Key_Right: pressedKey = "→"; break;
    default: pressedKey = event->text().toUpper(); break;
    }

    if (pressedKey == m_upKeyEdit->text() ||
        pressedKey == m_downKeyEdit->text() ||
        pressedKey == m_leftKeyEdit->text() ||
        pressedKey == m_rightKeyEdit->text() ||
        pressedKey == m_fireKeyEdit->text()) {
        QMessageBox::warning(this, "Duplicate Key", "This key is already assigned to another action!");
        return;
    }

    if (focusedWidget == m_upKeyEdit)
        m_upKeyEdit->setText(pressedKey);
    else if (focusedWidget == m_downKeyEdit)
        m_downKeyEdit->setText(pressedKey);
    else if (focusedWidget == m_leftKeyEdit)
        m_leftKeyEdit->setText(pressedKey);
    else if (focusedWidget == m_rightKeyEdit)
        m_rightKeyEdit->setText(pressedKey);
    else if (focusedWidget == m_fireKeyEdit)
        m_fireKeyEdit->setText(pressedKey);
}
ControlsWindow::~ControlsWindow()
{

}
