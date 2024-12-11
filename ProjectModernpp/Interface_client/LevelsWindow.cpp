#include "LevelsWindow.h"

LevelsWindow::LevelsWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Levels");
    setFixedSize(300, 300);

    QPalette palette;
    QColor customColor(20, 20, 20, 245); 
    palette.setColor(QPalette::Window, customColor);
    palette.setColor(QPalette::WindowText, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    QVBoxLayout* verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(10);

    //add text in the window
    QLabel* label = new QLabel("SELECT LEVEL", this);
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    label->setStyleSheet("color: white;");
    label->setFont(font);
    verticalLayout->addWidget(label);
    verticalLayout->setAlignment(Qt::AlignCenter);

    //buttons for levels
    m_easyLevelButton = new QPushButton("EASY", this);
    m_mediumLevelButton = new QPushButton("MEDIUM", this);
    m_hardLevelButton = new QPushButton("HARD", this);

    m_easyLevelButton->setFixedSize(250, 60);
    m_mediumLevelButton->setFixedSize(250, 60);
    m_hardLevelButton->setFixedSize(250, 60);

    m_easyLevelButton->setFont(font);
    m_mediumLevelButton->setFont(font);
    m_hardLevelButton->setFont(font);

    m_easyLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(34, 177, 76, 0.5);"  
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0, 128, 0, 0.4);"  
        "color: rgba(255, 255, 255, 1);"  
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 100, 0, 0.6);"  
        "color: rgba(255, 255, 255, 1);"  
        "}");

    m_mediumLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(255, 204, 0, 0.5);" 
        "color: black;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 165, 0, 0.4);"
        "color: rgba(255, 255, 255, 1);"  
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(255, 140, 0, 0.6);"  
        "color: rgba(255, 255, 255, 1);"  
        "}");

    m_hardLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(204, 0, 0, 0.5);"
        "color: white;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(178, 0, 0, 0.4);" 
        "color: rgba(255, 255, 255, 1);"  
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(153, 0, 0, 0.6);"  
        "color: rgba(255, 255, 255, 1);"  
        "}");

    //add buttons and text in Vertical Layout
    verticalLayout->addWidget(m_easyLevelButton, 0, Qt::AlignCenter);
    verticalLayout->addWidget(m_mediumLevelButton, 0, Qt::AlignCenter);
    verticalLayout->addWidget(m_hardLevelButton, 0, Qt::AlignCenter);
    label->setAlignment(Qt::AlignCenter);

    connect(m_easyLevelButton, &QPushButton::clicked, this, &LevelsWindow::OnEasyLevelClicked);
    connect(m_mediumLevelButton, &QPushButton::clicked, this, &LevelsWindow::OnMediumLevelClicked);
    connect(m_hardLevelButton, &QPushButton::clicked, this, &LevelsWindow::OnHardLevelClicked);
}

LevelsWindow::~LevelsWindow()
{

}

void LevelsWindow::OnEasyLevelClicked()
{
    m_level = 1;
    SendLevelToServer(m_level);
    emit levelSelected(m_level);
    accept();
}

void LevelsWindow::OnMediumLevelClicked()
{
    m_level = 2;
    SendLevelToServer(m_level);
    emit levelSelected(m_level);
    accept();
}

void LevelsWindow::OnHardLevelClicked()
{
    m_level = 3;
    SendLevelToServer(m_level);
    emit levelSelected(m_level);
    accept();
}

