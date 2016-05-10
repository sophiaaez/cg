#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Handle slider usage
    connect(ui->xRot, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setRotX(int)));
    connect(ui->yRot, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setRotY(int)));
    connect(ui->zRot, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setRotZ(int)));
    connect(ui->zoom, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setZoom(int)));
    connect(ui->unfold, SIGNAL(valueChanged(int)), ui->glwidget, SLOT(setUnfold(int)));

    // Handle any rotation change requests from mouse dragging and key presses
    connect( ui->glwidget, SIGNAL(changeRotation(int,int,int)), this, SLOT(onChangeRotation(int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChangeRotation(int dx, int dy, int dz)
{
    // To apply the requested rotation deltas, we increment...
    int newx = ui->xRot->value() + dx;
    int newy = ui->yRot->value() + dy;
    int newz = ui->zRot->value() + dz;

    // ... and set the sliders to the new angles mod 360 (to ensure 0..359)
    ui->xRot->setValue( newx % 360 );
    ui->yRot->setValue( newy % 360 );
    ui->zRot->setValue( newz % 360 );

    // NB: SetValue will emit valueChanged signal, so the scene gets updated
}
