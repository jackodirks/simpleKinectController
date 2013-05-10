#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer.reset(new QTimer());
    timer->setInterval(5000);
    connect(timer.get(),SIGNAL(timeout()),this,SLOT(emptyErrorLabel()));
    connect(ui->kinectHeightSlider,SIGNAL(valueChanged(int)),this,SLOT(setNewValue(int)));
    connect(ui->comboBoxKinect,SIGNAL(activated(QString)),this,SIGNAL(dropDownBoxUpdated(QString)));
    connect(ui->pushButtonApplyHeight,SIGNAL(clicked()),this,SLOT(buttonPressToUpdateKinectAngle()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewValue(int val){
    this->ui->labelNewHeight->setText(QString::number(val));
}

void MainWindow::setDropDownList(QMap<int, QString> map){
    QMapIterator<int,QString>i(map);
    int currentSelection = ui->comboBoxKinect->findText(ui->comboBoxKinect->currentText());
    ui->comboBoxKinect->clear();
    while(i.hasNext()){
        i.next();        
        ui->comboBoxKinect->addItem(i.value());
    }
    ui->comboBoxKinect->setCurrentIndex(currentSelection);
}

void MainWindow::setComboBox(QString str){
    int index = ui->comboBoxKinect->findText(str);
    ui->comboBoxKinect->setCurrentIndex(index);
}

void MainWindow::displayError(QString error){    
    ui->labelError->setText("<font color='red'>" + error + "</font>");
    timer->start();
}

void MainWindow::emptyErrorLabel(){
    ui->labelError->setText("");
}

void MainWindow::kinectAngle(long angle){
    ui->kinectHeightSlider->setValue(angle);
    ui->labelCurHeight->setText(QString::number(angle));
}

void MainWindow::buttonPressToUpdateKinectAngle(){
    emit updateKinectAngle(ui->kinectHeightSlider->value());
}
