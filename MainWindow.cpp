#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clearErrorTimer.reset(new QTimer());
    fpsTimer.reset(new QTimer);
    fpsMutex.reset(new QMutex);
    fpsLabel.reset(new QLabel);
    statusBar()->addPermanentWidget(fpsLabel.get());
    frames = 0;
    clearErrorTimer->setInterval(5000);
    clearErrorTimer->setSingleShot(true);
    fpsTimer->setInterval(1000);
    setFixedSize(width(),height());
    connect(ui->imageDisplayGLWidget,SIGNAL(gotFrame()),this,SLOT(gotFrame())); //Connects the signal that the OpenGL Widget sends when it has received and processed a frame to the slot here that counts the frames
    connect(fpsTimer.get(),SIGNAL(timeout()),this,SLOT(fpsTimeOut())); //Connects the FPSTimer to its slot that updates the FPS label
    connect(clearErrorTimer.get(),SIGNAL(timeout()),this,SLOT(emptyErrorLabel()));
    connect(ui->kinectHeightSlider,SIGNAL(valueChanged(int)),this,SLOT(setNewValue(int)));
    connect(ui->comboBoxKinect,SIGNAL(activated(QString)),this,SIGNAL(dropDownBoxUpdated(QString)));
    connect(ui->pushButtonApplyHeight,SIGNAL(clicked()),this,SLOT(buttonPressToUpdateKinectAngle()));
    connect(this,SIGNAL(receiveVGAArray(QByteArray)),ui->imageDisplayGLWidget,SLOT(receiveByteArray(QByteArray)));
    connect(this,SIGNAL(setStatus(QString,int)),this->statusBar(),SLOT(showMessage(QString,int)));
    connect(ui->checkBoxVerticalFlip,SIGNAL(toggled(bool)),ui->imageDisplayGLWidget,SLOT(flipImage(bool)));
    fpsTimer->start();
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
    clearErrorTimer->start();
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

void MainWindow::gotFrame(){
    fpsMutex->lock();
    ++frames;
    fpsMutex->unlock();
}

void MainWindow::fpsTimeOut(){
    fpsMutex->lock();
    fpsLabel->setText(QString::number(frames) + " FPS");
    frames = 0;
    fpsMutex->unlock();
}
