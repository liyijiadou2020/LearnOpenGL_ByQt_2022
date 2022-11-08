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
#include <foxopenglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionWireframe;
    QAction *actionDrawSphere;
    QAction *actionDrawCone;
    QAction *actionSphere;
    QAction *actionCone;
    QAction *actionisDraw;
    QAction *actionMove;
    QAction *actionMoveSphare;
    QAction *actionMoveCone;
    QAction *actionLightColor;
    QWidget *centralWidget;
    FoxOpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBarMove;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1110, 693);
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
        actionWireframe = new QAction(MainWindow);
        actionWireframe->setObjectName(QStringLiteral("actionWireframe"));
        actionWireframe->setCheckable(true);
        actionWireframe->setChecked(false);
        actionDrawSphere = new QAction(MainWindow);
        actionDrawSphere->setObjectName(QStringLiteral("actionDrawSphere"));
        actionDrawSphere->setCheckable(true);
        actionDrawSphere->setChecked(false);
        actionDrawCone = new QAction(MainWindow);
        actionDrawCone->setObjectName(QStringLiteral("actionDrawCone"));
        actionDrawCone->setCheckable(true);
        actionDrawCone->setChecked(false);
        actionSphere = new QAction(MainWindow);
        actionSphere->setObjectName(QStringLiteral("actionSphere"));
        actionCone = new QAction(MainWindow);
        actionCone->setObjectName(QStringLiteral("actionCone"));
        actionisDraw = new QAction(MainWindow);
        actionisDraw->setObjectName(QStringLiteral("actionisDraw"));
        actionisDraw->setAutoRepeat(true);
        actionisDraw->setVisible(true);
        actionisDraw->setMenuRole(QAction::NoRole);
        actionisDraw->setIconVisibleInMenu(false);
        actionMove = new QAction(MainWindow);
        actionMove->setObjectName(QStringLiteral("actionMove"));
        actionMoveSphare = new QAction(MainWindow);
        actionMoveSphare->setObjectName(QStringLiteral("actionMoveSphare"));
        actionMoveSphare->setCheckable(true);
        actionMoveCone = new QAction(MainWindow);
        actionMoveCone->setObjectName(QStringLiteral("actionMoveCone"));
        actionMoveCone->setCheckable(true);
        actionLightColor = new QAction(MainWindow);
        actionLightColor->setObjectName(QStringLiteral("actionLightColor"));
        actionLightColor->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/pic/Picture/light.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLightColor->setIcon(icon);
        QFont font;
        font.setPointSize(10);
        actionLightColor->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new FoxOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(400, 220, 300, 200));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1110, 24));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBarMove = new QToolBar(MainWindow);
        toolBarMove->setObjectName(QStringLiteral("toolBarMove"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBarMove);

        menuBar->addAction(menu_2->menuAction());
        menu_2->addAction(actionSphere);
        menu_2->addAction(actionCone);
        mainToolBar->addAction(actionisDraw);
        mainToolBar->addAction(actionDrawSphere);
        mainToolBar->addAction(actionDrawCone);
        mainToolBar->addAction(actionWireframe);
        toolBarMove->addAction(actionMove);
        toolBarMove->addAction(actionMoveSphare);
        toolBarMove->addAction(actionMoveCone);
        toolBarMove->addSeparator();
        toolBarMove->addAction(actionLightColor);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionWireframe->setText(QApplication::translate("MainWindow", "Wireframe", Q_NULLPTR));
        actionDrawSphere->setText(QApplication::translate("MainWindow", "Sphere", Q_NULLPTR));
        actionDrawCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        actionSphere->setText(QApplication::translate("MainWindow", "Sphere", Q_NULLPTR));
        actionCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        actionisDraw->setText(QApplication::translate("MainWindow", "DRAW:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionisDraw->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#ffff00;\">Draw:</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionMove->setText(QApplication::translate("MainWindow", "MOVE:", Q_NULLPTR));
        actionMoveSphare->setText(QApplication::translate("MainWindow", "Sphare", Q_NULLPTR));
        actionMoveCone->setText(QApplication::translate("MainWindow", "Cone", Q_NULLPTR));
        actionLightColor->setText(QApplication::translate("MainWindow", "Color", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "Selete", Q_NULLPTR));
        toolBarMove->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
