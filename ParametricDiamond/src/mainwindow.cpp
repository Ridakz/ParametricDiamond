#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometryengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParametricDiamond)
{
    ui->setupUi(this);
    ui->openGLWidget->diamond = &diamond;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_crownPointsSpinBox_valueChanged(int arg1)
{
    diamond.crownSize = arg1;
}

void MainWindow::on_pushButton_pressed()
{
    diamond.modeCrown = !diamond.modeCrown;
}

void MainWindow::on_pushButton_2_clicked()
{
    diamond.modePavillon = !diamond.modePavillon;
}


void MainWindow::on_crownRed_valueChanged(int value)
{
    diamond.crownColor.setX(value/255.0);
}


void MainWindow::on_crownGreen_valueChanged(int value)
{
    diamond.crownColor.setY(value/255.0);
}

void MainWindow::on_crownBlue_valueChanged(int value)
{
    diamond.crownColor.setZ(value/255.0);
}


void MainWindow::on_starRed_valueChanged(int value)
{
    diamond.starColor.setX(value/255.0);
}


void MainWindow::on_starGreen_valueChanged(int value)
{
    diamond.starColor.setY(value/255.0);
}

void MainWindow::on_starBlue_valueChanged(int value)
{
    diamond.starColor.setZ(value/255.0);
}

void MainWindow::on_girdleRed_valueChanged(int value)
{
    diamond.girdleColor.setX(value/255.0);
}

void MainWindow::on_girdleGreen_valueChanged(int value)
{
    diamond.girdleColor.setY(value/255.0);
}

void MainWindow::on_girdleBlue_valueChanged(int value)
{
    diamond.girdleColor.setZ(value/255.0);
}

void MainWindow::on_girdleTopRed_valueChanged(int value)
{
    diamond.girdleFacetColor.setX(value/255.0);
}

void MainWindow::on_girdleTopGreen_valueChanged(int value)
{
    diamond.girdleFacetColor.setY(value/255.0);
}

void MainWindow::on_girdleTopBlue_valueChanged(int value)
{
    diamond.girdleFacetColor.setZ(value/255.0);
}

void MainWindow::on_girdleBotRed_valueChanged(int value)
{
    diamond.pavillonColor.setX(value/255.0);
}

void MainWindow::on_girdleBotGreen_valueChanged(int value)
{
    diamond.pavillonColor.setY(value/255.0);
}

void MainWindow::on_girdleBotBlue_valueChanged(int value)
{
    diamond.pavillonColor.setZ(value/255.0);
}

void MainWindow::on_culetRed_valueChanged(int value)
{
    diamond.culetColor.setX(value/255.0);
}

void MainWindow::on_culetGreen_valueChanged(int value)
{
    diamond.culetColor.setY(value/255.0);
}


void MainWindow::on_culetBlue_valueChanged(int value)
{
    diamond.culetColor.setZ(value/255.0);
}

void MainWindow::on_girdlePointsSpinBox_valueChanged(int arg1)
{
    diamond.girdleSize = arg1;
}

void MainWindow::on_crownRadius_valueChanged(double arg1)
{
    diamond.crownRadius = arg1;
}

void MainWindow::on_crownRotation_valueChanged(double arg1)
{
    diamond.crownAngle = arg1;
}

void MainWindow::on_girdleRadius_valueChanged(double arg1)
{
    diamond.girdleRadius = arg1;
}

void MainWindow::on_girdleRotation_valueChanged(double arg1)
{
    diamond.girdleAngle = arg1;
}

void MainWindow::on_starZ_valueChanged(int value)
{
    diamond.crownToStarFacet = diamond.crownToGirdle*value/100.0;
}

void MainWindow::on_starOffset_valueChanged(int value)
{
    diamond.starCoef = 1 +value * (diamond.girdleRadius-diamond.crownRadius)/100;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    diamond.crownToGirdle = -value/10.0;
}

void MainWindow::on_culetZ_valueChanged(int value)
{
    diamond.crownToCulet = diamond.crownToPavillon -value/10.0;
}

void MainWindow::on_pavillonOffset_valueChanged(int value)
{
    diamond.pavillonCoef = value/10.0;
}

