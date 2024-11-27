#include "ControlsWindow.h"

ControlsWindow::ControlsWindow(QWidget *parent)
	: QDialog(parent)
{
    // Set window title
    setWindowTitle("Game Controls");

    // Create label to display controls information
    controlLabel = new QLabel(this);
    controlLabel->setText("<b>Game Controls</b><br><br>"
        "<b>W</b> - Move Up<br>"
        "<b>S</b> - Move Down<br>"
        "<b>A</b> - Move Left<br>"
        "<b>D</b> - Move Right<br><br>"
        "Press the buttons above to close this window.");

    // Create a close button to close the control window
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // Layout for displaying controls
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(controlLabel);
    layout->addWidget(closeButton);

    setLayout(layout);
}

ControlsWindow::~ControlsWindow()
{}
