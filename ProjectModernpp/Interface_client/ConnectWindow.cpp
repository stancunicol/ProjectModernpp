#include "ConnectWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
ConnectWindow::ConnectWindow(QWidget* parent)
    : QDialog(parent) 
{
    // set size and title for window
    setWindowTitle("Join Room");
    setFixedSize(300, 200);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); 
    palette.setColor(QPalette::WindowText, Qt::white); 
    setPalette(palette);
    setAutoFillBackground(true); 

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("Insert code:", this);
    QFont font;
    font.setPointSize(14); 
    font.setFamily("Segoe Script");  
    font.setBold(true);  

    label->setFont(font);
    layout->addWidget(label);

    //label for insert code
    insertCode = new QLineEdit(this);
    layout->addWidget(insertCode);
    insertCode->setAlignment(Qt::AlignCenter);
    insertCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");
    insertCode->setFixedSize(240, 50);
    layout->setAlignment(Qt::AlignCenter); 

    //button for start game
    startButton = new QPushButton("START GAME", this);
    startButton->setStyleSheet("background-color:green;"
        "color:lightGreen;"
        "font-weight: bold;"
        "border-radius: 5px;"
        "font-size: 15px;"
        "padding: 10px;"
        "border: 2px solid darkGreen; ");
    layout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &ConnectWindow::OnGenerateClicked);
}

void ConnectWindow::OnGenerateClicked()
{
    QString roomCode = insertCode->text();

    if (roomCode.isEmpty()) 
    {
        QMessageBox::warning(this, "Error", "The code field is empty!");
    }
    else 
    {
        QMessageBox::information(this, "Success", "You have joined the room: " + roomCode);
    }

}

ConnectWindow::~ConnectWindow() 
{ 
}
