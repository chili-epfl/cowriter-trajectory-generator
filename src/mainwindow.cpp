#include <iostream>
#include <algorithm>    // std::max

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include "dpi.h"
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

    sampler = new HomogeneousSampler();

    density = 5;
    iterations = 5;
}

MainWindow::~MainWindow()
{
    delete sampler;
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
             sampler->setPath(svgPathParser.path);
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

void MainWindow::on_groupBox_toggled(bool arg1)
{
   if (!renderArea->showTrajPoints) {
        int len = renderArea->setTrajPoints(sampler->sample(density, iterations));
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
    int len = renderArea->setTrajPoints(sampler->sample(density, iterations));
    findChild<QLabel *>("nbPoints")->setText(QString::number(len) + " points");
    renderArea->update();
}



void MainWindow::on_pushButton_clicked()
{
    auto traj = sampler->sample(density, iterations);
    cout << endl << "----------------------------------" << endl;
    cout << "Waypoints" << endl;
    cout << "(in mm, assuming the svg is at 90dpi)" << endl;
    cout << "x\ty\tz" << endl;
    for (auto tp : traj) {
        cout << PX2MM(tp.p.x) << "\t" << PX2MM(tp.p.y) << "\t0.0" << endl;
    }
    cout << endl << "----------------------------------" << endl;
}

void MainWindow::on_samplingMethod_currentIndexChanged(const QString &method)
{
    delete sampler;
    findChild<QLabel *>("iterationsLabel")->setEnabled(false);
    findChild<QSpinBox *>("iterations")->setEnabled(false);

    if  (method == "Homogeneous")
        sampler = new HomogeneousSampler();
    else if  (method == "Curvature") {
        findChild<QLabel *>("iterationsLabel")->setEnabled(true);
        findChild<QSpinBox *>("iterations")->setEnabled(true);
        sampler = new CurvatureSampler();
    }
    else
        sampler = new BaseSampler();

    sampler->setPath(svgPathParser.path);

    int len = renderArea->setTrajPoints(sampler->sample(density, iterations));
    findChild<QLabel *>("nbPoints")->setText(QString::number(len) + " points");
    renderArea->update();
}


void MainWindow::on_displayVelocities_stateChanged(int arg1)
{

   if (!renderArea->showVelocities) {
        renderArea->showVelocities = true;
   }
    else {
       renderArea->showVelocities = false;
   }
   renderArea->update();
}

void MainWindow::on_iterations_valueChanged(int value)
{

    iterations = value;
    int len = renderArea->setTrajPoints(sampler->sample(density, iterations));
    findChild<QLabel *>("nbPoints")->setText(QString::number(len) + " points");
    renderArea->update();

}
