#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->kinectHeightSlider,SIGNAL(valueChanged(int)),this,SLOT(setNewValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewValue(int val){
    this->ui->labelNewHeight->setText(QString::number(val));
}

void MainWindow::displayError(QString error){
    ui->labelError->setText("<font color='red'>" + error + "</font>");
}
