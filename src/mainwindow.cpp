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
             findChild<QLabel *>("pathLength")->setText("Length: " + QString::number(svgPathParser.path.length()));
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
    displayMousePosition(evt->pos());
}

void MainWindow::displayMousePosition(const QPoint &pos) {
    //TODO: compute correctly the coordinates of the mouse in the path coordinate space
    //float x = (pos.x() + renderArea->center.x()) * renderArea->zoom;
    //float y = (pos.y() + renderArea->center.y()) * renderArea->zoom;
    //statusBar()->showMessage(QString::number(x) + ", " + QString::number(y) , 2000);
}

void MainWindow::on_showCtlPoints_stateChanged(int arg1)
{
   renderArea->showCtlPoints ? renderArea->showCtlPoints = false : renderArea->showCtlPoints = true;
    renderArea->update();
}

int MainWindow::setTrajPoints(const BezierPath &bpath)
{

    auto sampler = TrajSampler(bpath);
    auto path = sampler.sample(density);

    vector<pair<point, float> > traj;
    for (int i = 0; i < path.size(); i++) {
        float normalizedCurvature = fmin(1.0, fmax(0.0, sampler.curvatures[i] * 10));
        pair<point, float> elem(path[i], normalizedCurvature);
        traj.push_back(elem);
    }

    renderArea->setTrajPoints(traj);

    return path.size();

}


void MainWindow::on_groupBox_toggled(bool arg1)
{
   if (!renderArea->showTrajPoints) {
        int len = setTrajPoints(svgPathParser.path);
        findChild<QLabel *>("nbPoints")->setText(QString::number(len) + " points");
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
   if (!renderArea->showSvg) {
        findChild<QLabel *>("pathLength")->setText("Length: " + QString::number(svgPathParser.path.length()));
        renderArea->showSvg = true;
   }
    else {
       renderArea->showSvg = false;
        findChild<QLabel *>("pathLength")->setText("");
   }
   renderArea->update();
}

void MainWindow::on_trajDensity_valueChanged(int value)
{
    density = value;
    int len = setTrajPoints(svgPathParser.path);
    findChild<QLabel *>("nbPoints")->setText(QString::number(len) + " points");
    renderArea->update();
 }


void MainWindow::on_pushButton_clicked()
{
    auto sampler = TrajSampler(svgPathParser.path);
    auto traj = sampler.sample(density);
    for (auto p : traj) {
        cout << p.x << "," << p.y << endl;
    }
}
