﻿#include "CustomToolBar.h"

CustomTitleBar::CustomTitleBar(QWidget* parent)
    : QWidget(parent), m_dragging(false)
{

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(50, 50, 50));
    this->setPalette(palette);

    setFixedHeight(35);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QHBoxLayout* leftLayout = new QHBoxLayout();
    m_iconLabel = new QLabel(this);
    leftLayout->addWidget(m_iconLabel); 
    leftLayout->setContentsMargins(0, 0, 0, 0);  

    QHBoxLayout* rightLayout = new QHBoxLayout();
    m_minimizeButton = new QPushButton("⎯", this);
    m_minimizeButton->setFixedSize(40, 35);
    m_minimizeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px"
        "} QPushButton:hover {"
        "background-color: rgba(0,0,0,0.10);"
        "} QPushButton:pressed {"
        "background-color: rgba(0,0,0,0.4); color: rgba(0, 0, 0,0.5);"
        "}");
    rightLayout->addWidget(m_minimizeButton);

    m_maximizeButton = new QPushButton("❒", this);
    m_maximizeButton->setFixedSize(40, 35);
    m_maximizeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px;"
        "} QPushButton:hover {"
        "background-color: rgba(0,0,0,0.10);"
        "} QPushButton:pressed {"
        "background-color: rgba(0,0,0,0.4); color: rgba(0, 0, 0,0.5);"
        "}");
    rightLayout->addWidget(m_maximizeButton);

    m_closeButton = new QPushButton("X", this);
    m_closeButton->setFixedSize(40, 35);
    m_closeButton->setStyleSheet("QPushButton {"
        "background-color: transparent;"
        "color: white;"
        "border: none;"
        "font-size: 15px"
        "} QPushButton:hover {"
        "background-color: red; "
        "} QPushButton:pressed {"
        "background-color: rgba(255, 77, 77, 0.69); color: rgba(0, 0, 0,0.5);"
        "}");
    rightLayout->addWidget(m_closeButton);
    rightLayout->setContentsMargins(0, 0, 0, 0);  

    connect(m_minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::onMinimizeButtonClicked);
    connect(m_maximizeButton, &QPushButton::clicked, this, &CustomTitleBar::onMaximizeButtonClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &CustomTitleBar::onCloseButtonClicked);

    QHBoxLayout* centerLayout = new QHBoxLayout();

    centerLayout->addStretch();  

    m_imageLabel = new QLabel(this);
    centerLayout->setContentsMargins(0, 0, 0, 0);  
    m_imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); 
    centerLayout->addWidget(m_imageLabel); 

    centerLayout->addStretch(); 
  
    mainLayout->addLayout(leftLayout);  
    mainLayout->addSpacing(75);
    mainLayout->addLayout(centerLayout); 
    mainLayout->addLayout(rightLayout);  

    mainLayout->setContentsMargins(0, 0, 0, 0); 
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

CustomTitleBar::~CustomTitleBar() 
{
   
}


void CustomTitleBar::SetIcon(const QPixmap& pixmap1) 
{

    if (pixmap1.isNull()) 
    {
        qDebug() << "Error in opening the file!";
    }
    QPixmap scaledPixmap = pixmap1.scaled(40, 50, Qt::KeepAspectRatio);

    m_iconLabel->setPixmap(scaledPixmap);
}

void CustomTitleBar::SetImage(const QString& imagePath) 
{
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) 
    {
        pixmap = pixmap.scaledToHeight(35, Qt::SmoothTransformation);

        m_imageLabel->setPixmap(pixmap);

        m_imageLabel->setFixedHeight(35);

        m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        m_imageLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    }
    else
        qDebug() << "Error in opening the file!";
}

void CustomTitleBar::mousePressEvent(QMouseEvent* event) 
{
    m_dragging = true;
    m_dragPosition = event->globalPosition().toPoint() - parentWidget()->geometry().topLeft();
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent* event) 
{
    if (m_dragging) 
    {
        parentWidget()->move(event->globalPosition().toPoint() - m_dragPosition);
    }
}

void CustomTitleBar::mouseReleaseEvent(QMouseEvent* event) 
{
    m_dragging = false;
}

void CustomTitleBar::onCloseButtonClicked() 
{
    qApp->quit();
}

void CustomTitleBar::onMaximizeButtonClicked() 
{
    if (parentWidget()->isMaximized()) 
    {
        parentWidget()->showNormal();
    }
    else 
    {
        parentWidget()->showMaximized();
    }
}

void CustomTitleBar::onMinimizeButtonClicked() 
{
    parentWidget()->showMinimized();
}

