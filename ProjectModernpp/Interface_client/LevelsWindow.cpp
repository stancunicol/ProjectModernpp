#include "LevelsWindow.h"

LevelsWindow::LevelsWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Levels");
    setFixedSize(300, 300);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
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
    easyLevelButton = new QPushButton("EASY", this);
    mediumLevelButton = new QPushButton("MEDIUM", this);
    hardLevelButton = new QPushButton("HARD", this);

    easyLevelButton->setFixedSize(250, 60);
    mediumLevelButton->setFixedSize(250, 60);
    hardLevelButton->setFixedSize(250, 60);

    QFont buttonFont("Segoe Script", 14, QFont::Bold);
    easyLevelButton->setFont(buttonFont);
    mediumLevelButton->setFont(buttonFont);
    hardLevelButton->setFont(buttonFont);

    easyLevelButton->setStyleSheet("QPushButton {"
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

    mediumLevelButton->setStyleSheet("QPushButton {"
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

    hardLevelButton->setStyleSheet("QPushButton {"
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
    verticalLayout->addWidget(easyLevelButton, 0, Qt::AlignCenter);
    verticalLayout->addWidget(mediumLevelButton, 0, Qt::AlignCenter);
    verticalLayout->addWidget(hardLevelButton, 0, Qt::AlignCenter);
    label->setAlignment(Qt::AlignCenter);

}

LevelsWindow::~LevelsWindow()
{}

void LevelsWindow::OnEasyLevelClicked()
{
}

void LevelsWindow::OnMediumLevelClicked()
{
}

void LevelsWindow::OnHardLevelClicked()
{
}

