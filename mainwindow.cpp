#include <iostream>
#include <algorithm>    // std::max

#include <QtGui>

#include "renderarea.h"
#include  "trajsampler.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    renderArea = new RenderArea;
    setCentralWidget(renderArea);

    density = 5;
}

MainWindow::~MainWindow()
{
    delete renderArea;
    delete ui;

}


void MainWindow::open()
{
    QString fileName =
                 QFileDialog::getOpenFileName(this, tr("Open SVG File"),
                                              QDir::currentPath(),
                                              tr("SVG Files (*.svg)"));
         if (fileName.isEmpty())
             return;

         QFile file(fileName);
         if (!file.open(QFile::ReadOnly | QFile::Text)) {
             QMessageBox::warning(this, tr("SVG Trajectory tuning"),
                                  tr("Cannot read file %1:\n%2.")
                                  .arg(fileName)
                                  .arg(file.errorString()));
             return;
         }

         if (svgPathParser.read(&file)) {
             statusBar()->showMessage(tr("File loaded"), 2000);
         }
}


void MainWindow::updatePath()
{
    renderArea->preparePath(svgPathParser.path);
}

void MainWindow::on_actionOpen_triggered()
{

    open();
    updatePath();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::wheelEvent(QWheelEvent *evt)
{
    auto step = (evt->delta() / 8) / 15; // nb of steps. Cf http://harmattan-dev.nokia.com/docs/library/html/qt4/qwheelevent.html#delta
    renderArea->zoom += step / 10.0;
    renderArea->zoom = max(renderArea->zoom, 0.1);
    renderArea->update();
}

void MainWindow::mousePressEvent(QMouseEvent *evt)
{
    initialDragPos = evt->pos() - renderArea->center;
}

void MainWindow::mouseMoveEvent(QMouseEvent *evt)
{
    renderArea->center = evt->pos() - initialDragPos;
    renderArea->update();
}

void MainWindow::on_showCtlPoints_stateChanged(int arg1)
{
   renderArea->showCtlPoints ? renderArea->showCtlPoints = false : renderArea->showCtlPoints = true;
    renderArea->update();
}

void MainWindow::on_showTraj_stateChanged(int arg1)
{
   if (!renderArea->showTrajPoints) {
        auto sampler = TrajSampler(svgPathParser.path);
        auto traj = sampler.sample(density);
        findChild<QLabel *>("nbPoints")->setText(QString::number(traj.size()) + " points");
        renderArea->setTrajPoints(traj);
       renderArea->showTrajPoints = true;
    renderArea->update();
   }
    else {
       renderArea->showTrajPoints = false;
        findChild<QLabel *>("nbPoints")->setText("");
            renderArea->update();
   }
}

void MainWindow::on_showSvg_stateChanged(int arg1)
{
   renderArea->showSvg ? renderArea->showSvg = false : renderArea->showSvg = true;
    renderArea->update();
}

void MainWindow::on_trajDensity_valueChanged(int value)
{
    density = value;
    auto sampler = TrajSampler(svgPathParser.path);
    auto traj = sampler.sample(density);
    findChild<QLabel *>("nbPoints")->setText(QString::number(traj.size()) + " points");
    renderArea->setTrajPoints(traj);
    renderArea->update();
 }
