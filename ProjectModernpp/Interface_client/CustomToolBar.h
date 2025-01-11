#pragma once

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>

class CustomTitleBar : public QWidget 
{
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget* parent = nullptr);
    ~CustomTitleBar();

    void setIcon(const QPixmap& pixmap1);
    void setImage(const QString& imagePath);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void onCloseButtonClicked();
    void onMaximizeButtonClicked();
    void onMinimizeButtonClicked();


private:
    QLabel* m_iconLabel = nullptr;
    QLabel* m_imageLabel = nullptr;
    QPushButton* m_closeButton = nullptr;
    QPushButton* m_maximizeButton = nullptr;
    QPushButton* m_minimizeButton = nullptr;

    bool m_dragging;
    QPoint m_dragPosition;
};

