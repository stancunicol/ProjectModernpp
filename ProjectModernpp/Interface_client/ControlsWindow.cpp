#include "ControlsWindow.h"

ControlsWindow::ControlsWindow(QWidget* parent)
    : QDialog(parent),
    m_upKeyEdit(new QLineEdit(this)),
    m_downKeyEdit(new QLineEdit(this)),
    m_leftKeyEdit(new QLineEdit(this)),
    m_rightKeyEdit(new QLineEdit(this)),
    m_fireKeyEdit(new QLineEdit(this))
{
    this->setFocus();
    setWindowTitle("Game Controls");
    setFixedSize(600, 500);

    setStyleSheet(
        "ControlsWindow {"
        "background-image: url(background.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "}");

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

    QPushButton* resetButton = new QPushButton("Reset to Defaults", this);
    connect(resetButton, &QPushButton::clicked, this, &ControlsWindow::resetToDefaults);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(gameLegendLabel);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(resetButton);

    ControlsWindow::loadSettings();
    //Aply event filter for every QLineEdit to capture the event
    m_upKeyEdit->installEventFilter(this);
    m_downKeyEdit->installEventFilter(this);
    m_leftKeyEdit->installEventFilter(this);
    m_rightKeyEdit->installEventFilter(this);
    m_fireKeyEdit->installEventFilter(this);
}

void ControlsWindow::saveSettings()
{
    QSettings settings("MyCompany", "BattleCity");
    settings.setValue("UpKey", m_upKeyEdit->text());
    settings.setValue("DownKey", m_downKeyEdit->text());
    settings.setValue("LeftKey", m_leftKeyEdit->text());
    settings.setValue("RightKey", m_rightKeyEdit->text());
    settings.setValue("FireKey", m_fireKeyEdit->text());
}

void ControlsWindow::loadSettings()
{
    QSettings settings("MyCompany", "BattleCity");
    m_upKeyEdit->setText(settings.value("UpKey", "W").toString());
    m_downKeyEdit->setText(settings.value("DownKey", "S").toString());
    m_leftKeyEdit->setText(settings.value("LeftKey", "A").toString());
    m_rightKeyEdit->setText(settings.value("RightKey", "D").toString());
    m_fireKeyEdit->setText(settings.value("FireKey", "Space").toString());
}

void ControlsWindow::resetToDefaults()
{
    m_upKeyEdit->setText("W");
    m_downKeyEdit->setText("S");
    m_leftKeyEdit->setText("A");
    m_rightKeyEdit->setText("D");
    m_fireKeyEdit->setText("Space");

    saveSettings(); 
}

void ControlsWindow::setupKeyEdit(QLineEdit* keyEdit)
{
    keyEdit->setAlignment(Qt::AlignCenter);
    keyEdit->setFocusPolicy(Qt::StrongFocus);
    keyEdit->setReadOnly(true);  
}

bool ControlsWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        QLineEdit* keyEdit = qobject_cast<QLineEdit*>(watched);
        if (keyEdit) 
        {
            QString pressedKey;
            switch (keyEvent->key()) 
            {
            case Qt::Key_Up: pressedKey = "↑"; break;
            case Qt::Key_Down: pressedKey = "↓"; break;
            case Qt::Key_Left: pressedKey = "←"; break;
            case Qt::Key_Right: pressedKey = "→"; break;
            case Qt::Key_Space: pressedKey = "Space"; break;
            default: pressedKey = keyEvent->text().toUpper(); break;
            }

            if (pressedKey != m_upKeyEdit->text() && pressedKey != m_downKeyEdit->text() &&
                pressedKey != m_leftKeyEdit->text() && pressedKey != m_rightKeyEdit->text() &&
                pressedKey != m_fireKeyEdit->text()) 
            {
                keyEdit->setText(pressedKey);
                saveSettings(); 
            }
            else 
            {
                QMessageBox::warning(this, "Duplicate Key", "This key is already assigned to another action!");
            }

            return true;
        }
    }
    //let the QWidget to proces the event
    return QDialog::eventFilter(watched, event);
}

ControlsWindow::~ControlsWindow()
{
    saveSettings();
}
