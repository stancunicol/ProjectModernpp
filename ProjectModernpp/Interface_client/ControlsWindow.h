#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QKeyEvent>
#include <QMessageBox>

class ControlsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ControlsWindow(QWidget* parent = nullptr);
    ~ControlsWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QLabel* instructionLabel=nullptr;
    QLineEdit* upKeyEdit = nullptr;
    QLineEdit* downKeyEdit = nullptr;
    QLineEdit* leftKeyEdit = nullptr;
    QLineEdit* rightKeyEdit = nullptr;
    QLineEdit* activeKeyEdit = nullptr;
    QPushButton* closeButton = nullptr;
};
