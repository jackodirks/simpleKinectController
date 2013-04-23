#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void displayError(QString error);
    
private:
    Ui::MainWindow *ui;
private slots:
    void setNewValue(int val);
};

#endif // MAINWINDOW_H
