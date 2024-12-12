#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QColor>
#include <QPixmap>

class ControlsWindow : public QDialog
{
    Q_OBJECT

public:
    ControlsWindow(QWidget* parent = nullptr);
    void setupKeyEdit(QLineEdit* keyEdit);
    ~ControlsWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QLabel* m_instructionLabel=nullptr;
    QLineEdit* m_upKeyEdit = nullptr;
    QLineEdit* m_downKeyEdit = nullptr;
    QLineEdit* m_leftKeyEdit = nullptr;
    QLineEdit* m_rightKeyEdit = nullptr;
    QLineEdit* m_fireKeyEdit=nullptr;
};
