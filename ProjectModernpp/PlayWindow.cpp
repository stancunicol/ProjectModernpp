#include "PlayWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
PlayWindow::PlayWindow(QWidget* parent)
    : QWidget(parent) {
    // Setting title and size for window
    setWindowTitle("Generate Code");
    setFixedSize(300, 200);


    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); // Background balck
    palette.setColor(QPalette::WindowText, Qt::white); // Text white
    setPalette(palette);
    setAutoFillBackground(true); 

    // Creating layout
    QVBoxLayout* layout = new QVBoxLayout(this); //vertical arrange
    //Create space for the Code
    GenerateCode = new QLineEdit(this);
    GenerateCode->setPlaceholderText("CODE");
    GenerateCode->setAlignment(Qt::AlignCenter);
    GenerateCode->setEnabled(false);
    GenerateCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");

    GenerateCode->setFixedSize(240, 50);

    layout->addWidget(GenerateCode);
    layout->setAlignment(Qt::AlignCenter); 
}
