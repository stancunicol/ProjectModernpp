#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> // Pentru afișarea mesajelor de eroare

class ConnectWindow : public QWidget {
    Q_OBJECT

public:
    ConnectWindow(QWidget* parent = nullptr); // Constructor
    ~ConnectWindow(); // Destructor

protected:
    void OnGenerateClicked(); // Slot pentru gestionarea click-ului pe butonul de generare cod

private:
    QLineEdit* insertCode; // Câmp pentru numele de utilizator
    QPushButton* startButton; //Buton pentru mers mai departe
    QLabel* messageLabel; // Etichetă pentru mesajele de feedback

};

