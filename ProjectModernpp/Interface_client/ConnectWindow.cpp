#include "ConnectWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
ConnectWindow::ConnectWindow(QWidget* parent)
    : QWidget(parent) {

    // Setare titlu și dimensiune fereastră
    setWindowTitle("Join Room");
    setFixedSize(300, 200);


    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); // Fundal negru
    palette.setColor(QPalette::WindowText, Qt::white); // Text alb
    setPalette(palette);
    setAutoFillBackground(true); // Asigură-te că fundalul se umple cu culoarea


    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("Insert code:", this);
    QFont font;
    font.setPointSize(14);  // Dimensiunea fontului
    font.setFamily("Segoe Script");  // Familia fontului
    font.setBold(true);  // Font îngroșat (opțional)

    label->setFont(font);
    layout->addWidget(label);