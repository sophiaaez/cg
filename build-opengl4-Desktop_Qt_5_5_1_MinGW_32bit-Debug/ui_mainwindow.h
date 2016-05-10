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
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
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
    QLabel *lbZRot;
    QDial *yRot;
    QLabel *lbYRot;
    QDial *xRot;
    QDial *zRot;
    QLabel *lbXRot;
    QDial *unfold;
    QLabel *lbUnfold;
    QDial *zoom;
    QLabel *lbZoom;
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
        lbZRot = new QLabel(centralWidget);
        lbZRot->setObjectName(QStringLiteral("lbZRot"));

        gridLayout->addWidget(lbZRot, 0, 3, 1, 1, Qt::AlignHCenter);

        yRot = new QDial(centralWidget);
        yRot->setObjectName(QStringLiteral("yRot"));
        yRot->setMaximum(360);
        yRot->setValue(0);
        yRot->setWrapping(true);
        yRot->setNotchesVisible(false);

        gridLayout->addWidget(yRot, 1, 2, 1, 1);

        lbYRot = new QLabel(centralWidget);
        lbYRot->setObjectName(QStringLiteral("lbYRot"));

        gridLayout->addWidget(lbYRot, 0, 2, 1, 1, Qt::AlignHCenter);

        xRot = new QDial(centralWidget);
        xRot->setObjectName(QStringLiteral("xRot"));
        xRot->setMaximum(360);
        xRot->setWrapping(true);

        gridLayout->addWidget(xRot, 1, 1, 1, 1);

        zRot = new QDial(centralWidget);
        zRot->setObjectName(QStringLiteral("zRot"));
        zRot->setMaximum(360);
        zRot->setWrapping(true);

        gridLayout->addWidget(zRot, 1, 3, 1, 1);

        lbXRot = new QLabel(centralWidget);
        lbXRot->setObjectName(QStringLiteral("lbXRot"));

        gridLayout->addWidget(lbXRot, 0, 1, 1, 1, Qt::AlignHCenter);

        unfold = new QDial(centralWidget);
        unfold->setObjectName(QStringLiteral("unfold"));
        unfold->setMaximum(100);
        unfold->setWrapping(false);
        unfold->setNotchesVisible(false);

        gridLayout->addWidget(unfold, 1, 0, 1, 1);

        lbUnfold = new QLabel(centralWidget);
        lbUnfold->setObjectName(QStringLiteral("lbUnfold"));

        gridLayout->addWidget(lbUnfold, 0, 0, 1, 1, Qt::AlignHCenter);

        zoom = new QDial(centralWidget);
        zoom->setObjectName(QStringLiteral("zoom"));
        zoom->setMaximum(200);
        zoom->setValue(100);
        zoom->setWrapping(false);

        gridLayout->addWidget(zoom, 1, 4, 1, 1);

        lbZoom = new QLabel(centralWidget);
        lbZoom->setObjectName(QStringLiteral("lbZoom"));

        gridLayout->addWidget(lbZoom, 0, 4, 1, 1, Qt::AlignHCenter);


        verticalLayout->addLayout(gridLayout);

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
        lbZRot->setText(QApplication::translate("MainWindow", "Z", 0));
        lbYRot->setText(QApplication::translate("MainWindow", "Y", 0));
        lbXRot->setText(QApplication::translate("MainWindow", "X", 0));
        lbUnfold->setText(QApplication::translate("MainWindow", "Unfold", 0));
        lbZoom->setText(QApplication::translate("MainWindow", "Zoom", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
