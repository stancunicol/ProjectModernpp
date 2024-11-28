#include "ControlsWindow.h"
#include <QMessageBox>
#include <QKeyEvent>

ControlsWindow::ControlsWindow(QWidget* parent)
    : QDialog(parent)
{
    // Set window title
    setWindowTitle("Game Controls");

    // Create label to display instructions
    instructionLabel = new QLabel(this);
    instructionLabel->setText("<b>Set Your Game Controls</b><br><br>"
        "Click on a field and press a key to assign it.");

    // Create editable fields for controls
    upKeyEdit = new QLineEdit(this);
    upKeyEdit->setText("W");
    upKeyEdit->setAlignment(Qt::AlignCenter);

    downKeyEdit = new QLineEdit(this);
    downKeyEdit->setText("S");
    downKeyEdit->setAlignment(Qt::AlignCenter);

    leftKeyEdit = new QLineEdit(this);
    leftKeyEdit->setText("A");
    leftKeyEdit->setAlignment(Qt::AlignCenter);

    rightKeyEdit = new QLineEdit(this);
    rightKeyEdit->setText("D");
    rightKeyEdit->setAlignment(Qt::AlignCenter);

    // Set the focus policy to StrongFocus, which allows these fields to receive focus
    upKeyEdit->setFocusPolicy(Qt::StrongFocus);
    downKeyEdit->setFocusPolicy(Qt::StrongFocus);
    leftKeyEdit->setFocusPolicy(Qt::StrongFocus);
    rightKeyEdit->setFocusPolicy(Qt::StrongFocus);

    upKeyEdit->setReadOnly(true); // Disable manual editing to ensure only key events set the value
    downKeyEdit->setReadOnly(true);
    leftKeyEdit->setReadOnly(true);
    rightKeyEdit->setReadOnly(true);

    connect(upKeyEdit, &QLineEdit::editingFinished, this, [this]() { upKeyEdit->clearFocus(); });
    connect(downKeyEdit, &QLineEdit::editingFinished, this, [this]() { downKeyEdit->clearFocus(); });
    connect(leftKeyEdit, &QLineEdit::editingFinished, this, [this]() { leftKeyEdit->clearFocus(); });
    connect(rightKeyEdit, &QLineEdit::editingFinished, this, [this]() { rightKeyEdit->clearFocus(); });

    closeButton = new QPushButton("Close", this); // Create a close button
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* layout = new QVBoxLayout; // Layout for displaying controls
    layout->addWidget(instructionLabel);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel("Move Up:"), 0, 0);
    gridLayout->addWidget(upKeyEdit, 0, 1);
    gridLayout->addWidget(new QLabel("Move Down:"), 1, 0);
    gridLayout->addWidget(downKeyEdit, 1, 1);
    gridLayout->addWidget(new QLabel("Move Left:"), 2, 0);
    gridLayout->addWidget(leftKeyEdit, 2, 1);
    gridLayout->addWidget(new QLabel("Move Right:"), 3, 0);
    gridLayout->addWidget(rightKeyEdit, 3, 1);

    layout->addLayout(gridLayout);
    layout->addWidget(closeButton);

    setLayout(layout);
}

void ControlsWindow::keyPressEvent(QKeyEvent* event)
{
    // Capture the key press and set it to the focused field
    QWidget* focusedWidget = focusWidget();
    QString pressedKey;

    // Handle special keys like Up, Down, Left, Right arrow keys
    if (event->key() == Qt::Key_Up) 
    {
        pressedKey = "↑"; // For up arrow
    }
    else 
        if (event->key() == Qt::Key_Down) 
        {
            pressedKey = "↓"; // For down arrow
        }
        else 
            if (event->key() == Qt::Key_Left) 
            {
                pressedKey = "←"; // For left arrow
            }
            else 
                if (event->key() == Qt::Key_Right) 
                {
                    pressedKey = "→"; // For right arrow
                }
                else 
                {
                    pressedKey = event->text().toUpper(); // For regular alphanumeric keys
                }

    // Check if the pressed key is already assigned to any other control
    if (pressedKey == upKeyEdit->text() ||
        pressedKey == downKeyEdit->text() ||
        pressedKey == leftKeyEdit->text() ||
        pressedKey == rightKeyEdit->text())
    {
        // If the key is already assigned, show a warning and return without setting the key
        QMessageBox::warning(this, "Duplicate Key", "This key is already assigned to another action!");
        return;
    }

    // Set the key in the focused field
    if (focusedWidget == upKeyEdit) 
    {
        upKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == downKeyEdit) 
    {
        downKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == leftKeyEdit) 
    {
        leftKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == rightKeyEdit) 
    {
        rightKeyEdit->setText(pressedKey);
    }
}

ControlsWindow::~ControlsWindow()
{

}
