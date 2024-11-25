#include "PLayWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
PlayWindow::PlayWindow(QWidget* parent)
    : QDialog(parent) {
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
    GenerateCode = new QLineEdit(this);
    GenerateCode->setPlaceholderText("CODE");
    GenerateCode->setAlignment(Qt::AlignCenter);
    GenerateCode->setEnabled(false);
    GenerateCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");

    GenerateCode->setFixedSize(240, 50);

    layout->addWidget(GenerateCode);
    layout->setAlignment(Qt::AlignCenter);


    // Create generate code button
    GenerateButton = new QPushButton("Generate code", this);
    GenerateButton->setStyleSheet("QPushButton {"
        "background-color: rgba(0, 191, 255, 0.7);" // light blue
        "color: white;" // text alb
        "border: none;"
        "padding: 10px;" // spațiul interior dintre conținutul widget-ului și limitele acestuia
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0, 0, 255, 0.7);" // dark blue
        "color: rgba(255, 255, 255, 0.5);" // alb transparent
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 139, 0.7);" // even darker blue (dark blue shade)
        "color: rgba(255, 255, 255, 0.7);" // alb transparent
        "}");
    layout->addWidget(GenerateButton);



    FowardButton = new QPushButton("Start game", this);
    FowardButton->setStyleSheet("QPushButton {"
        "background-color: rgba(144, 238, 144, 0.7);" // light green
        "color: white;" // text alb
        "border: none;"
        "padding: 10px;" // spațiul interior dintre conținutul widget-ului și limitele acestuia
        "border-radius: 5px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(34, 139, 34, 0.7);" // dark green
        "color: rgba(255, 255, 255, 0.5);" // alb transparent
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 100, 0, 0.7);" // even darker green (dark green shade)
        "color: rgba(255, 255, 255, 0.7);" // alb transparent
        "}");
    layout->addWidget(FowardButton);
    connect(GenerateButton, &QPushButton::clicked, this, &PlayWindow::onGenerateClicked);
    connect(FowardButton, &QPushButton::clicked, this, &PlayWindow::onGenerateClicked);

}

PlayWindow::~PlayWindow() {
    // Destructor (nimic special de curățat aici)
}
QString PlayWindow::GenerateRandomNumber()
{
    QString number;

    // Generarea a 5 cifre aleatoare
    srand(time(NULL));
    for (int i = 0; i < 5; ++i) {

        int digit = rand() % 10; // Generare cifră între 0 și 9
        number.append(QString::number(digit)); // Adăugăm cifra la string
    }
    return number;

}

void PlayWindow::OnFowardClicked()
{
}

void PlayWindow::onGenerateClicked() {
    QString randomNumber = GenerateRandomNumber();
    GenerateCode->setText(randomNumber);
}

