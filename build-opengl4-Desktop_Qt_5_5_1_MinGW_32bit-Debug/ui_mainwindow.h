/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "oglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    OGLWidget *glwidget;
    QGridLayout *gridLayout;
    QSlider *cube_z;
    QSlider *cylinder_z;
    QLabel *label;
    QSlider *cube_x;
    QLabel *label_2;
    QSlider *cylinder_x;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *start_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(476, 577);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        glwidget = new OGLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glwidget->sizePolicy().hasHeightForWidth());
        glwidget->setSizePolicy(sizePolicy);
        glwidget->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(glwidget);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 10, 10, -1);
        cube_z = new QSlider(centralWidget);
        cube_z->setObjectName(QStringLiteral("cube_z"));
        cube_z->setMinimum(-60);
        cube_z->setMaximum(70);
        cube_z->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cube_z, 1, 1, 1, 1);

        cylinder_z = new QSlider(centralWidget);
        cylinder_z->setObjectName(QStringLiteral("cylinder_z"));
        cylinder_z->setMinimum(-64);
        cylinder_z->setMaximum(64);
        cylinder_z->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cylinder_z, 1, 3, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cube_x = new QSlider(centralWidget);
        cube_x->setObjectName(QStringLiteral("cube_x"));
        cube_x->setMinimum(-50);
        cube_x->setMaximum(40);
        cube_x->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cube_x, 0, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        cylinder_x = new QSlider(centralWidget);
        cylinder_x->setObjectName(QStringLiteral("cylinder_x"));
        cylinder_x->setMinimum(-44);
        cylinder_x->setMaximum(44);
        cylinder_x->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cylinder_x, 0, 3, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        start_button = new QPushButton(centralWidget);
        start_button->setObjectName(QStringLiteral("start_button"));

        verticalLayout->addWidget(start_button);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 476, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "Cube X", 0));
        label_2->setText(QApplication::translate("MainWindow", "Cube Z", 0));
        label_3->setText(QApplication::translate("MainWindow", "Cylinder X", 0));
        label_4->setText(QApplication::translate("MainWindow", "Cylinder Z", 0));
        start_button->setText(QApplication::translate("MainWindow", "Start", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
