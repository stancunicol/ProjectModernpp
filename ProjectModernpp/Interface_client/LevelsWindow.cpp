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
    font.setFamily("Segoe Script");
    font.setBold(true);
    label->setStyleSheet("color: white;");
    label->setFont(font);
    verticalLayout->addWidget(label);

    //buttons for levels
    m_easyLevelButton = new QPushButton("EASY", this);
    m_mediumLevelButton = new QPushButton("MEDIUM", this);
    m_hardLevelButton = new QPushButton("HARD", this);

    m_easyLevelButton->setFixedSize(250, 60);
    m_mediumLevelButton->setFixedSize(250, 60);
    m_hardLevelButton->setFixedSize(250, 60);

    QFont buttonFont("Segoe Script", 14, QFont::Bold);
    m_easyLevelButton->setFont(buttonFont);
    m_mediumLevelButton->setFont(buttonFont);
    m_hardLevelButton->setFont(buttonFont);

    m_easyLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(158, 204, 158, 1);"  // green
        "color: black;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(124, 160, 124, 0.7);"  // dark green
        "color: rgba(255, 255, 255, 0.5);"  // transparent white
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(86, 123, 86, 0.9);"  // even darker green
        "color: rgba(255, 255, 255, 1);"  // transparent white
        "}");

    m_mediumLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(248, 253, 170, 1);"//beige
        "color: black;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(237, 242, 164, 0.7);"  // dark beige
        "color: rgba(255, 255, 255, 0.5);"  // transparent white
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(222, 227, 153, 0.9);"  // even darker beige
        "color: rgba(255, 255, 255, 1);"  // transparent white
        "}");

    m_hardLevelButton->setStyleSheet("QPushButton {"
        "background-color: rgba(235, 126, 102, 1);"  // red
        "color: black;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(219, 122, 97, 0.7);"  // dark red
        "color: rgba(255, 255, 255, 0.5);"  // transparent white
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(205, 118, 96, 0.9);"  // even darker red
        "color: rgba(255, 255, 255,1 );"  //transparent white
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
    emit levelSelected(m_level);
    accept();
}

void LevelsWindow::OnMediumLevelClicked()
{
    m_level = 2;
    emit levelSelected(m_level);
    accept();
}

void LevelsWindow::OnHardLevelClicked()
{
    m_level = 3;
    emit levelSelected(m_level);
}

