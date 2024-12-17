#include "CustomToolBar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>

CustomTitleBar::CustomTitleBar(QWidget* parent)
    : QWidget(parent), m_dragging(false) {

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(50, 50, 50));
    this->setPalette(palette);

    setFixedHeight(35);

    // main horizontal layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // Layout for the title and icon
    QHBoxLayout* leftLayout = new QHBoxLayout();
    m_iconLabel = new QLabel(this);
    leftLayout->addWidget(m_iconLabel);


    leftLayout->addSpacerItem(new QSpacerItem(500, 500, QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Layout for the logo (image)
    m_imageLabel = new QLabel(this);
    leftLayout->addWidget(m_imageLabel);

    // layout for the buttons
    QHBoxLayout* rightLayout = new QHBoxLayout();
    rightLayout->addStretch();


    m_minimizeButton = new QPushButton("_", this);
    m_minimizeButton->setFixedSize(30, 20);
    m_minimizeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,0,0,0.10);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,0,0,0.4) "
        "color: rgba(0, 0, 0,0.5);"//black
        "}");
    rightLayout->addWidget(m_minimizeButton);

    m_maximizeButton = new QPushButton("□", this);
    m_maximizeButton->setFixedSize(30, 20);
    m_maximizeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0,0,0,0.10);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0,0,0,0.4) "
        "color: rgba(0, 0, 0,0.5);"
        "}");
    rightLayout->addWidget(m_maximizeButton);

    m_closeButton = new QPushButton("X", this);
    m_closeButton->setFixedSize(30, 20);
    m_closeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px"
        "}"
        "QPushButton:hover {"
        "background-color: red;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(255, 77, 77, 0.69); "
        "color: rgba(0, 0, 0,0.5);"
        "}");
    rightLayout->addWidget(m_closeButton);

    // Conectarea semnalelor pentru butoane
    connect(m_minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::onMinimizeButtonClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &CustomTitleBar::onMaximizeButtonClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &CustomTitleBar::onCloseButtonClicked);

    // addi
    mainLayout->addLayout(leftLayout);   // Stânga
    mainLayout->addLayout(rightLayout);  // Dreapta

    setLayout(mainLayout);
}
CustomTitleBar::~CustomTitleBar() {
    // Destructor
}


void CustomTitleBar::setIcon(const QPixmap& pixmap1) {

    if (pixmap1.isNull()) {
        qDebug() << "Fișierul nu a fost incărcat corect!";
    }

    QPixmap scaledPixmap = pixmap1.scaled(40, 50, Qt::KeepAspectRatio);

    m_iconLabel->setPixmap(scaledPixmap);
}

void CustomTitleBar::setImage(const QString& imagePath) {
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        m_imageLabel->setPixmap(pixmap.scaled(200, 15, Qt::KeepAspectRatio));//resize the image
    }
}

void CustomTitleBar::mousePressEvent(QMouseEvent* event) {
    
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent* event) {
    
}

void CustomTitleBar::mouseReleaseEvent(QMouseEvent* event) {
    
}
void CustomTitleBar::onCloseButtonClicked() {
    
}

void CustomTitleBar::onMaximizeButtonClicked() {
   
}

void CustomTitleBar::onMinimizeButtonClicked() {
    
}

