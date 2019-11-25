#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "parametricdiamond.h"

namespace Ui {
class ParametricDiamond;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ParametricDiamond diamond;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_crownPointsSpinBox_valueChanged(int arg1);

    void on_pushButton_pressed();

    void on_crownRed_valueChanged(int value);

    void on_crownGreen_valueChanged(int value);

    void on_crownBlue_valueChanged(int value);

    void on_starRed_valueChanged(int value);

    void on_starGreen_valueChanged(int value);

    void on_starBlue_valueChanged(int value);

    void on_girdleRed_valueChanged(int value);

    void on_girdleGreen_valueChanged(int value);

    void on_girdleBlue_valueChanged(int value);

    void on_girdleTopRed_valueChanged(int value);

    void on_girdleTopGreen_valueChanged(int value);

    void on_girdleTopBlue_valueChanged(int value);

    void on_girdleBotRed_valueChanged(int value);

    void on_girdleBotGreen_valueChanged(int value);

    void on_girdleBotBlue_valueChanged(int value);

    void on_culetRed_valueChanged(int value);

    void on_culetGreen_valueChanged(int value);

    void on_culetBlue_valueChanged(int value);

    void on_girdlePointsSpinBox_valueChanged(int arg1);

    void on_crownRadius_valueChanged(double arg1);

    void on_crownRotation_valueChanged(double arg1);

    void on_girdleRadius_valueChanged(double arg1);

    void on_girdleRotation_valueChanged(double arg1);

    void on_starZ_valueChanged(int value);

    void on_starOffset_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_culetZ_valueChanged(int value);

    void on_pavillonOffset_valueChanged(int value);

    void on_pushButton_2_clicked();

private:
    Ui::ParametricDiamond *ui;
};

#endif // MAINWINDOW_H
