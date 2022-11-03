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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <MyOpenGLWidget03.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionShow;
    QAction *act_DrawCube;
    QAction *act_DrawBall;
    QAction *act_DrawCone;
    QAction *actionMove;
    QAction *act_MoveCube;
    QAction *act_MoveBall;
    QAction *act_MoveCone;
    QWidget *centralwidget;
    MyOpenGLWidget03 *openGLWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(768, 556);
        MainWindow->setStyleSheet(QLatin1String("QWidget {\n"
"	background-color:rgb(68, 68, 68);\n"
"	color:rgb(255, 255, 255);\n"
"	font: 12pt;\n"
"}\n"
"QMenuBar {\n"
"	background-color:rgb(200, 200, 200);\n"
"	color:rgb(60, 60, 60);\n"
"}\n"
"QWidget:checked {\n"
"	background-color:rgb(150, 150, 150);\n"
"}"));
        actionShow = new QAction(MainWindow);
        actionShow->setObjectName(QStringLiteral("actionShow"));
        act_DrawCube = new QAction(MainWindow);
        act_DrawCube->setObjectName(QStringLiteral("act_DrawCube"));
        act_DrawCube->setCheckable(true);
        act_DrawBall = new QAction(MainWindow);
        act_DrawBall->setObjectName(QStringLiteral("act_DrawBall"));
        act_DrawBall->setCheckable(true);
        act_DrawCone = new QAction(MainWindow);
        act_DrawCone->setObjectName(QStringLiteral("act_DrawCone"));
        act_DrawCone->setCheckable(true);
        actionMove = new QAction(MainWindow);
        actionMove->setObjectName(QStringLiteral("actionMove"));
        act_MoveCube = new QAction(MainWindow);
        act_MoveCube->setObjectName(QStringLiteral("act_MoveCube"));
        act_MoveCube->setCheckable(true);
        act_MoveBall = new QAction(MainWindow);
        act_MoveBall->setObjectName(QStringLiteral("act_MoveBall"));
        act_MoveBall->setCheckable(true);
        act_MoveCone = new QAction(MainWindow);
        act_MoveCone->setObjectName(QStringLiteral("act_MoveCone"));
        act_MoveCone->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        openGLWidget = new MyOpenGLWidget03(centralwidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(110, 60, 551, 351));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 768, 27));
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

        toolBar->addAction(actionShow);
        toolBar->addAction(act_DrawCube);
        toolBar->addAction(act_DrawBall);
        toolBar->addAction(act_DrawCone);
        toolBar_2->addAction(actionMove);
        toolBar_2->addAction(act_MoveCube);
        toolBar_2->addAction(act_MoveBall);
        toolBar_2->addAction(act_MoveCone);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionShow->setText(QApplication::translate("MainWindow", "Draw:", Q_NULLPTR));
        act_DrawCube->setText(QApplication::translate("MainWindow", "Cube", Q_NULLPTR));
        act_DrawBall->setText(QApplication::translate("MainWindow", "Ball", Q_NULLPTR));
        act_DrawCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        actionMove->setText(QApplication::translate("MainWindow", "Move:", Q_NULLPTR));
        act_MoveCube->setText(QApplication::translate("MainWindow", "Cube", Q_NULLPTR));
        act_MoveBall->setText(QApplication::translate("MainWindow", "Ball", Q_NULLPTR));
        act_MoveCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
