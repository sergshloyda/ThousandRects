/********************************************************************************
** Form generated from reading UI file 'thousandrectsinconcurentthreads.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THOUSANDRECTSINCONCURENTTHREADS_H
#define UI_THOUSANDRECTSINCONCURENTTHREADS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "twidget.h"

QT_BEGIN_NAMESPACE

class Ui_ThousandRectsInConcurentThreadsClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    RenderWidget *widget;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *chan_1;
    QCheckBox *chan_2;
    QCheckBox *chan_3;
    QCheckBox *chan_4;
    QCheckBox *chan_5;
    QCheckBox *chan_6;
    QCheckBox *chan_7;
    QCheckBox *chan_8;
    QCheckBox *chan_9;
    QCheckBox *chan_10;
    QCheckBox *chan_11;
    QCheckBox *chan_12;
    QCheckBox *chan_13;
    QCheckBox *chan_14;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ThousandRectsInConcurentThreadsClass)
    {
        if (ThousandRectsInConcurentThreadsClass->objectName().isEmpty())
            ThousandRectsInConcurentThreadsClass->setObjectName(QStringLiteral("ThousandRectsInConcurentThreadsClass"));
        ThousandRectsInConcurentThreadsClass->resize(1072, 598);
        verticalLayout_2 = new QVBoxLayout(ThousandRectsInConcurentThreadsClass);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(ThousandRectsInConcurentThreadsClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        checkBox = new QCheckBox(ThousandRectsInConcurentThreadsClass);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout->addWidget(checkBox);

        checkBox_2 = new QCheckBox(ThousandRectsInConcurentThreadsClass);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout->addWidget(checkBox_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget = new RenderWidget(ThousandRectsInConcurentThreadsClass);
        widget->setObjectName(QStringLiteral("widget"));

        horizontalLayout_2->addWidget(widget);

        groupBox = new QGroupBox(ThousandRectsInConcurentThreadsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(150, 400));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        chan_1 = new QCheckBox(groupBox);
        chan_1->setObjectName(QStringLiteral("chan_1"));

        verticalLayout->addWidget(chan_1);

        chan_2 = new QCheckBox(groupBox);
        chan_2->setObjectName(QStringLiteral("chan_2"));

        verticalLayout->addWidget(chan_2);

        chan_3 = new QCheckBox(groupBox);
        chan_3->setObjectName(QStringLiteral("chan_3"));

        verticalLayout->addWidget(chan_3);

        chan_4 = new QCheckBox(groupBox);
        chan_4->setObjectName(QStringLiteral("chan_4"));

        verticalLayout->addWidget(chan_4);

        chan_5 = new QCheckBox(groupBox);
        chan_5->setObjectName(QStringLiteral("chan_5"));

        verticalLayout->addWidget(chan_5);

        chan_6 = new QCheckBox(groupBox);
        chan_6->setObjectName(QStringLiteral("chan_6"));

        verticalLayout->addWidget(chan_6);

        chan_7 = new QCheckBox(groupBox);
        chan_7->setObjectName(QStringLiteral("chan_7"));

        verticalLayout->addWidget(chan_7);

        chan_8 = new QCheckBox(groupBox);
        chan_8->setObjectName(QStringLiteral("chan_8"));

        verticalLayout->addWidget(chan_8);

        chan_9 = new QCheckBox(groupBox);
        chan_9->setObjectName(QStringLiteral("chan_9"));

        verticalLayout->addWidget(chan_9);

        chan_10 = new QCheckBox(groupBox);
        chan_10->setObjectName(QStringLiteral("chan_10"));

        verticalLayout->addWidget(chan_10);

        chan_11 = new QCheckBox(groupBox);
        chan_11->setObjectName(QStringLiteral("chan_11"));

        verticalLayout->addWidget(chan_11);

        chan_12 = new QCheckBox(groupBox);
        chan_12->setObjectName(QStringLiteral("chan_12"));

        verticalLayout->addWidget(chan_12);

        chan_13 = new QCheckBox(groupBox);
        chan_13->setObjectName(QStringLiteral("chan_13"));

        verticalLayout->addWidget(chan_13);

        chan_14 = new QCheckBox(groupBox);
        chan_14->setObjectName(QStringLiteral("chan_14"));

        verticalLayout->addWidget(chan_14);

        verticalSpacer = new QSpacerItem(20, 189, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);

        horizontalLayout_2->setStretch(0, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(ThousandRectsInConcurentThreadsClass);

        QMetaObject::connectSlotsByName(ThousandRectsInConcurentThreadsClass);
    } // setupUi

    void retranslateUi(QWidget *ThousandRectsInConcurentThreadsClass)
    {
        ThousandRectsInConcurentThreadsClass->setWindowTitle(QApplication::translate("ThousandRectsInConcurentThreadsClass", "ThousandRectsInConcurentThreads", 0));
        pushButton->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "PushButton", 0));
        checkBox->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        checkBox_2->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        groupBox->setTitle(QApplication::translate("ThousandRectsInConcurentThreadsClass", "GroupBox", 0));
        chan_1->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_2->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_3->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_4->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_5->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_6->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_7->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_8->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_9->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_10->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_11->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_12->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_13->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        chan_14->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
    } // retranslateUi

};

namespace Ui {
    class ThousandRectsInConcurentThreadsClass: public Ui_ThousandRectsInConcurentThreadsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THOUSANDRECTSINCONCURENTTHREADS_H
