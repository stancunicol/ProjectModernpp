#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QIcon>
#include<QPushButton>

class CustomTitleBar : public QWidget {
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
    QLabel* m_iconLabel;
    QLabel* m_imageLabel;
    QPushButton* m_closeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_minimizeButton;

    bool m_dragging;
    QPoint m_dragPosition;
};

