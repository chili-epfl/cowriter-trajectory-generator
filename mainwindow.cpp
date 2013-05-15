#include <iostream>
#include <QtGui>

#include "renderarea.h"

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
             cout << "Points:" << endl;
             for (auto curve : svgPathParser.path.curves) {
                cout << "(x,y)->(" << curve.x << ", " << curve.y << ")" << endl;
             }
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
