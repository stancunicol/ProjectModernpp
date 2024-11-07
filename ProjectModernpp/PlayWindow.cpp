#include "PlayWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
PlayWindow::PlayWindow(QWidget* parent)
    : QWidget(parent) {
    // Setare titlu și dimensiune fereastră
    setWindowTitle("Generate Code");
    setFixedSize(300, 200);


    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); // Fundal negru
    palette.setColor(QPalette::WindowText, Qt::white); // Text alb
    setPalette(palette);
    setAutoFillBackground(true); // Asigură-te că fundalul se umple cu culoarea
}
