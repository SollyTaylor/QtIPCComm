/********************************************************************************
** Form generated from reading UI file 'ipc_test.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IPC_TEST_H
#define UI_IPC_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IPC_testClass
{
public:
    QWidget *centralWidget;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QLabel *picLabel;
    QPushButton *chooseButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *IPC_testClass)
    {
        if (IPC_testClass->objectName().isEmpty())
            IPC_testClass->setObjectName(QStringLiteral("IPC_testClass"));
        IPC_testClass->resize(715, 478);
        centralWidget = new QWidget(IPC_testClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        loadButton = new QPushButton(centralWidget);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(10, 110, 75, 23));
        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(10, 80, 75, 23));
        picLabel = new QLabel(centralWidget);
        picLabel->setObjectName(QStringLiteral("picLabel"));
        picLabel->setGeometry(QRect(110, 20, 551, 401));
        chooseButton = new QPushButton(centralWidget);
        chooseButton->setObjectName(QStringLiteral("chooseButton"));
        chooseButton->setGeometry(QRect(10, 40, 75, 23));
        IPC_testClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(IPC_testClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 715, 23));
        IPC_testClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(IPC_testClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        IPC_testClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(IPC_testClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        IPC_testClass->setStatusBar(statusBar);

        retranslateUi(IPC_testClass);

        QMetaObject::connectSlotsByName(IPC_testClass);
    } // setupUi

    void retranslateUi(QMainWindow *IPC_testClass)
    {
        IPC_testClass->setWindowTitle(QApplication::translate("IPC_testClass", "IPC_test", 0));
        loadButton->setText(QApplication::translate("IPC_testClass", "\344\273\216\345\206\205\345\255\230\350\257\273\345\217\226", 0));
        saveButton->setText(QApplication::translate("IPC_testClass", "\344\277\235\345\255\230\345\210\260\345\206\205\345\255\230", 0));
        picLabel->setText(QApplication::translate("IPC_testClass", "TextLabel", 0));
        chooseButton->setText(QApplication::translate("IPC_testClass", "\351\200\211\346\213\251\350\267\257\345\276\204", 0));
    } // retranslateUi

};

namespace Ui {
    class IPC_testClass: public Ui_IPC_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IPC_TEST_H
