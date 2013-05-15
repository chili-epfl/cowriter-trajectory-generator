#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RenderArea;

#include "svgpathparser.h"

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

    void on_showTraj_stateChanged(int arg1);

    void on_showSvg_stateChanged(int arg1);

    void on_trajDensity_valueChanged(int value);

private:
    RenderArea *renderArea;
    SvgPathParser svgPathParser;
    int density; // density of points when generating trajectory -> nb of points generated per cubic spline

    Ui::MainWindow *ui;
    void wheelEvent(QWheelEvent *evt);

    QPoint initialDragPos;

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // MAINWINDOW_H
