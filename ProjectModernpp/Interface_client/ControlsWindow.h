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
#include <QSettings>

class ControlsWindow : public QDialog
{
    Q_OBJECT

public:
    ControlsWindow(QWidget* parent = nullptr);
    void SaveSettings();
    void LoadSettings();
    void ResetToDefaults();
    void SetupKeyEdit(QLineEdit* keyEdit);
    bool eventFilter(QObject* watched, QEvent* event);
    ~ControlsWindow();

private:
    QLabel* m_instructionLabel=nullptr;
    QLineEdit* m_upKeyEdit = nullptr;
    QLineEdit* m_downKeyEdit = nullptr;
    QLineEdit* m_leftKeyEdit = nullptr;
    QLineEdit* m_rightKeyEdit = nullptr;
    QLineEdit* m_fireKeyEdit=nullptr;
};
