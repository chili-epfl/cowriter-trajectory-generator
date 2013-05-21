#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RenderArea;

#include "svgpathparser.h"
#include "trajsampler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void open();
   void updatePath();

private slots:
    void on_actionOpen_triggered();


    void on_actionQuit_triggered();

    void on_showCtlPoints_stateChanged(int arg1);


    void on_showSvg_stateChanged(int arg1);

    void on_trajDensity_valueChanged(int value);

    void on_groupBox_toggled(bool arg1);

    void on_pushButton_clicked();

    void on_samplingMethod_currentIndexChanged(const QString &arg1);

    void on_displayVelocities_stateChanged(int arg1);

    void on_iterations_valueChanged(int value);

private:
    RenderArea *renderArea;
    SvgPathParser svgPathParser;
    TrajSampler* sampler;
    int density; // density of points when generating trajectory -> nb of points generated per cubic spline
    int iterations; // nb of iterations for iterative samplers

    Ui::MainWindow *ui;

    QPoint initialDragPos;

    int initSampler();
    void displayMousePosition(const QPoint& pos);

    void wheelEvent(QWheelEvent *evt);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // MAINWINDOW_H
