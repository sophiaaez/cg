#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Handle slider usage
    //connect(ui->xRot, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setRotX(int)));
    connect(ui->cube_x, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setCubeX(int)));
    connect(ui->cube_z, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setCubeZ(int)));
    connect(ui->cylinder_x, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setCylinderX(int)));
    connect(ui->cylinder_z, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setCylinderZ(int)));
    connect(ui->start_button, SIGNAL(clicked(bool)), ui->glwidget, SLOT(setStart(bool)));
    connect(ui->flip, SIGNAL(clicked(bool)), ui->glwidget, SLOT(setFlip(bool)));
    connect(ui->perspective, SIGNAL(clicked(bool)), ui->glwidget, SLOT(setPerspective(bool)));

    // Handle any rotation change requests from mouse dragging and key presses
    //connect( ui->glwidget, SIGNAL(changeRotation(int,int,int)), this, SLOT(onChangeRotation(int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

