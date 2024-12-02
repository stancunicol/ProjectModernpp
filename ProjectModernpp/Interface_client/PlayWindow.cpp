#include "PLayWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>

PlayWindow::PlayWindow(QWidget* parent)
    : QDialog(parent) 
{
    // Set title and size for window
    setWindowTitle("Generate Code");
    setFixedSize(300, 200);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); // Background
    palette.setColor(QPalette::WindowText, Qt::white); // Text 
    setPalette(palette);
    setAutoFillBackground(true);

    // Creating  layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    generateCode = new QLineEdit(this);
    generateCode->setPlaceholderText("CODE");
    generateCode->setAlignment(Qt::AlignCenter);
    generateCode->setEnabled(false);
    generateCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");
    generateCode->setFixedSize(240, 50);

    layout->addWidget(generateCode);
    layout->setAlignment(Qt::AlignCenter);

    // Create generate code button
    generateButton = new QPushButton("Generate code", this);
    generateButton->setStyleSheet("QPushButton {"
        "background-color: rgba(0, 191, 255, 0.7);" // light blue
        "color: white;" 
        "border: none;"
        "padding: 10px;" 
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0, 0, 255, 0.7);" // dark blue
        "color: rgba(255, 255, 255, 0.5);" 
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 139, 0.7);" // even darker blue (dark blue shade)
        "color: rgba(255, 255, 255, 0.7);" 
        "}");
    layout->addWidget(generateButton);

    forwardButton = new QPushButton("Start game", this);
    forwardButton->setStyleSheet("QPushButton {"
        "background-color: rgba(144, 238, 144, 0.7);" // light green
        "color: white;" 
        "border: none;"
        "padding: 10px;" 
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(34, 139, 34, 0.7);" // dark green
        "color: rgba(255, 255, 255, 0.5);" 
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 100, 0, 0.7);" // even darker green (dark green shade)
        "color: rgba(255, 255, 255, 0.7);" t
        "}");
    layout->addWidget(forwardButton);
    connect(generateButton, &QPushButton::clicked, this, &PlayWindow::OnGenerateClicked);
    connect(forwardButton, &QPushButton::clicked, this, &PlayWindow::OnForwardClicked);
}

PlayWindow::~PlayWindow() 
{
    
}

//function for generate random numbers
QString PlayWindow::GenerateRandomNumber()
{
    QString number;

    
    srand(time(NULL));
    for (int i = 0; i < 5; ++i) {

        int digit = rand() % 10; 
        number.append(QString::number(digit)); 
    }
    return number;

}

void PlayWindow::OnForwardClicked()
{

}

void PlayWindow::OnGenerateClicked() 
{
    QString randomNumber = GenerateRandomNumber();
    generateCode->setText(randomNumber);
}

