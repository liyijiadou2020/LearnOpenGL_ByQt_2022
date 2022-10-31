/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <lyj_openglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *drawRect;
    QAction *actClear;
    QAction *actWireframe;
    QAction *actionBall;
    QAction *actionCone;
    QAction *actionCube;
    QAction *actionTorus;
    QAction *actionmove;
    QAction *act_moveBall;
    QAction *act_moveCone;
    QAction *act_moveCube;
    QAction *act_moveTorus;
    QWidget *centralwidget;
    LYJ_OpenGLWidget *openGLWidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(736, 466);
        QIcon icon;
        icon.addFile(QStringLiteral(":/pic/pepsi.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color:rgb(68, 68, 68);\n"
"	color:rgb(255, 255, 255);\n"
"	font: 10pt \"\345\271\274\345\234\206\";\n"
"}\n"
"QMenuBar{\n"
"	background-color:rgb(200, 200, 200);\n"
"	color:rgb(60, 60, 60);\n"
"}\n"
""));
        drawRect = new QAction(MainWindow);
        drawRect->setObjectName(QStringLiteral("drawRect"));
        actClear = new QAction(MainWindow);
        actClear->setObjectName(QStringLiteral("actClear"));
        actWireframe = new QAction(MainWindow);
        actWireframe->setObjectName(QStringLiteral("actWireframe"));
        actWireframe->setCheckable(true);
        actionBall = new QAction(MainWindow);
        actionBall->setObjectName(QStringLiteral("actionBall"));
        actionBall->setCheckable(true);
        actionCone = new QAction(MainWindow);
        actionCone->setObjectName(QStringLiteral("actionCone"));
        actionCone->setCheckable(true);
        actionCube = new QAction(MainWindow);
        actionCube->setObjectName(QStringLiteral("actionCube"));
        actionCube->setCheckable(true);
        actionTorus = new QAction(MainWindow);
        actionTorus->setObjectName(QStringLiteral("actionTorus"));
        actionTorus->setCheckable(true);
        actionmove = new QAction(MainWindow);
        actionmove->setObjectName(QStringLiteral("actionmove"));
        act_moveBall = new QAction(MainWindow);
        act_moveBall->setObjectName(QStringLiteral("act_moveBall"));
        act_moveBall->setCheckable(true);
        act_moveCone = new QAction(MainWindow);
        act_moveCone->setObjectName(QStringLiteral("act_moveCone"));
        act_moveCone->setCheckable(true);
        act_moveCube = new QAction(MainWindow);
        act_moveCube->setObjectName(QStringLiteral("act_moveCube"));
        act_moveCube->setCheckable(true);
        act_moveTorus = new QAction(MainWindow);
        act_moveTorus->setObjectName(QStringLiteral("act_moveTorus"));
        act_moveTorus->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        openGLWidget = new LYJ_OpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(230, 110, 300, 200));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 736, 19));
        menu = new QMenu(menubar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
        MainWindow->insertToolBarBreak(toolBar_2);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addSeparator();
        toolBar->addAction(drawRect);
        toolBar->addAction(actClear);
        toolBar->addAction(actWireframe);
        toolBar->addAction(actionBall);
        toolBar->addAction(actionCone);
        toolBar->addAction(actionCube);
        toolBar->addAction(actionTorus);
        toolBar->addSeparator();
        toolBar_2->addAction(actionmove);
        toolBar_2->addAction(act_moveBall);
        toolBar_2->addAction(act_moveCone);
        toolBar_2->addAction(act_moveCube);
        toolBar_2->addAction(act_moveTorus);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "LIYIJIA\342\200\230s Window!", Q_NULLPTR));
        drawRect->setText(QApplication::translate("MainWindow", "\347\224\273\344\270\200\344\270\252\347\237\251\345\275\242", Q_NULLPTR));
        actClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        actWireframe->setText(QApplication::translate("MainWindow", "\347\272\277\346\241\206\346\250\241\345\274\217", Q_NULLPTR));
        actionBall->setText(QApplication::translate("MainWindow", "Ball", Q_NULLPTR));
        actionCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        actionCube->setText(QApplication::translate("MainWindow", "Cube", Q_NULLPTR));
        actionTorus->setText(QApplication::translate("MainWindow", "Torus", Q_NULLPTR));
        actionmove->setText(QApplication::translate("MainWindow", "move", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionmove->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>MOVE</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        act_moveBall->setText(QApplication::translate("MainWindow", "Ball", Q_NULLPTR));
        act_moveCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        act_moveCube->setText(QApplication::translate("MainWindow", "Cube", Q_NULLPTR));
        act_moveTorus->setText(QApplication::translate("MainWindow", "Torus", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\223\215\344\275\234", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\346\237\245\347\234\213", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
