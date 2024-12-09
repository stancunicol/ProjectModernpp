#include "ControlsWindow.h"

ControlsWindow::ControlsWindow(QWidget* parent)
    : QDialog(parent)
{
    // Set window title
    setWindowTitle("Game Controls");
    setFixedSize(300, 200);

    QPalette palette; 
    QColor customColor(20, 20, 20, 245);
    palette.setColor(QPalette::Window, customColor);
    setPalette(palette);
    setAutoFillBackground(true);

    // Create label to display instructions
    m_instructionLabel = new QLabel(this);
    m_instructionLabel->setText("<b>Set Your Game Controls</b><br><br>"
        "Click on a field and press a key to assign it.");

    // Create editable fields for controls
    m_upKeyEdit = new QLineEdit(this);
    m_upKeyEdit->setText("W");
    m_upKeyEdit->setAlignment(Qt::AlignCenter);

    m_downKeyEdit = new QLineEdit(this);
    m_downKeyEdit->setText("S");
    m_downKeyEdit->setAlignment(Qt::AlignCenter);

    m_leftKeyEdit = new QLineEdit(this);
    m_leftKeyEdit->setText("A");
    m_leftKeyEdit->setAlignment(Qt::AlignCenter);

    m_rightKeyEdit = new QLineEdit(this);
    m_rightKeyEdit->setText("D");
    m_rightKeyEdit->setAlignment(Qt::AlignCenter);

    // Set the focus policy to StrongFocus, which allows these fields to receive focus
    m_upKeyEdit->setFocusPolicy(Qt::StrongFocus);
    m_downKeyEdit->setFocusPolicy(Qt::StrongFocus);
    m_leftKeyEdit->setFocusPolicy(Qt::StrongFocus);
    m_rightKeyEdit->setFocusPolicy(Qt::StrongFocus);

    m_upKeyEdit->setReadOnly(true); // Disable manual editing to ensure only key events set the value
    m_downKeyEdit->setReadOnly(true);
    m_leftKeyEdit->setReadOnly(true);
    m_rightKeyEdit->setReadOnly(true);

    connect(m_upKeyEdit, &QLineEdit::editingFinished, this, [this]() { m_upKeyEdit->clearFocus(); });
    connect(m_downKeyEdit, &QLineEdit::editingFinished, this, [this]() { m_downKeyEdit->clearFocus(); });
    connect(m_leftKeyEdit, &QLineEdit::editingFinished, this, [this]() { m_leftKeyEdit->clearFocus(); });
    connect(m_rightKeyEdit, &QLineEdit::editingFinished, this, [this]() { m_rightKeyEdit->clearFocus(); });

    m_closeButton = new QPushButton("Close", this); // Create a close button
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* layout = new QVBoxLayout; // Layout for displaying controls
    layout->addWidget(m_instructionLabel);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel("Move Up:"), 0, 0);
    gridLayout->addWidget(m_upKeyEdit, 0, 1);
    gridLayout->addWidget(new QLabel("Move Down:"), 1, 0);
    gridLayout->addWidget(m_downKeyEdit, 1, 1);
    gridLayout->addWidget(new QLabel("Move Left:"), 2, 0);
    gridLayout->addWidget(m_leftKeyEdit, 2, 1);
    gridLayout->addWidget(new QLabel("Move Right:"), 3, 0);
    gridLayout->addWidget(m_rightKeyEdit, 3, 1);

    layout->addLayout(gridLayout);
    layout->addWidget(m_closeButton);

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
    if (pressedKey == m_upKeyEdit->text() ||
        pressedKey == m_downKeyEdit->text() ||
        pressedKey == m_leftKeyEdit->text() ||
        pressedKey == m_rightKeyEdit->text())
    {
        // If the key is already assigned, show a warning and return without setting the key
        QMessageBox::warning(this, "Duplicate Key", "This key is already assigned to another action!");
        return;
    }

    // Set the key in the focused field
    if (focusedWidget == m_upKeyEdit) 
    {
        m_upKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == m_downKeyEdit) 
    {
        m_downKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == m_leftKeyEdit) 
    {
        m_leftKeyEdit->setText(pressedKey);
    }
    else if (focusedWidget == m_rightKeyEdit) 
    {
        m_rightKeyEdit->setText(pressedKey);
    }
}

ControlsWindow::~ControlsWindow()
{

}
