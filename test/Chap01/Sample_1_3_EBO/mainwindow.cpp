#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 在我们添加的openglwidget上还有一层central widget。
    // 我们觉得central widget没有存在的必要，所以把central widget设成了opengl widget。
    // 这样OpenGL widget就会铺满这个central widget了
    setCentralWidget(ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_drawRect_triggered()
{
    ui->openGLWidget->drawShape(LYJ_OpenGLWidget::Rect);
}


void MainWindow::on_actClear_triggered()
{
    ui->openGLWidget->drawShape(LYJ_OpenGLWidget::None);
}

void MainWindow::on_actWireframe_triggered()
{
    ui->openGLWidget->setWirefame(ui->actWireframe->isChecked());
}
