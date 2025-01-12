#include "PLayWindow.h"

PlayWindow::PlayWindow(QWidget* parent)
    : QDialog(parent)
{
    // Set title and size for window
    setWindowTitle("Generate Code");
    setFixedSize(300, 200);

    QPalette palette;
    QColor customColor(20, 20, 20, 245);
    palette.setColor(QPalette::Window, customColor);
    palette.setColor(QPalette::WindowText, Qt::white); // Text
    setPalette(palette);
    setAutoFillBackground(true);

    // Creating  layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_generateCode = new QLineEdit(this);
    m_generateCode->setPlaceholderText("CODE");
    m_generateCode->setAlignment(Qt::AlignCenter);
    m_generateCode->setEnabled(false);
    m_generateCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");
    m_generateCode->setFixedSize(240, 50);

    layout->addWidget(m_generateCode);
    layout->setAlignment(Qt::AlignCenter);

    // Create generate code button
    m_generateButton = new QPushButton("Generate code", this);
    m_generateButton->setStyleSheet("QPushButton {"
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
    layout->addWidget(m_generateButton);

    m_forwardButton = new QPushButton("Start game", this);
    m_forwardButton->setStyleSheet("QPushButton {"
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
        "color: rgba(255, 255, 255, 0.7);"
        "}");
    layout->addWidget(m_forwardButton);
    connect(m_generateButton, &QPushButton::clicked, this, &PlayWindow::OnGenerateClicked);
    connect(m_forwardButton, &QPushButton::clicked, this, &PlayWindow::OnForwardClicked);
}

PlayWindow::~PlayWindow()
{

}

void PlayWindow::OnForwardClicked()
{
    if (m_generate == false)
        QMessageBox::information(this, "Game room", "Please generate a room number");
    else
        accept();
}

void PlayWindow::OnGenerateClicked()
{
    m_serverObject.GetGeneratedCodeFromServer();
    QString randomNumber = QString::number(m_serverObject.GetRoomCode());
    m_generate = true;
    m_generateCode->setText(randomNumber);

    m_serverObject.PostServerData("http://localhost:8080/registerRoom", "{\"code\": \"" + randomNumber.toStdString() + "\"}");
}

