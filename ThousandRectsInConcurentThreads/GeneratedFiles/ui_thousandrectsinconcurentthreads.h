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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "devicegroupbox.h"
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
    DeviceGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QWidget *menu;
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
        ThousandRectsInConcurentThreadsClass->resize(1106, 622);
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

        groupBox = new DeviceGroupBox(ThousandRectsInConcurentThreadsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(200, 400));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(12);
        groupBox->setFont(font);
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        menu = new QWidget(groupBox);
        menu->setObjectName(QStringLiteral("menu"));
        menu->setMinimumSize(QSize(120, 25));
        menu->setMaximumSize(QSize(16777215, 25));

        verticalLayout->addWidget(menu);

        chan_1 = new QCheckBox(groupBox);
        chan_1->setObjectName(QStringLiteral("chan_1"));
        QFont font1;
        font1.setPointSize(10);
        chan_1->setFont(font1);

        verticalLayout->addWidget(chan_1);

        chan_2 = new QCheckBox(groupBox);
        chan_2->setObjectName(QStringLiteral("chan_2"));
        chan_2->setFont(font1);

        verticalLayout->addWidget(chan_2);

        chan_3 = new QCheckBox(groupBox);
        chan_3->setObjectName(QStringLiteral("chan_3"));
        chan_3->setFont(font1);

        verticalLayout->addWidget(chan_3);

        chan_4 = new QCheckBox(groupBox);
        chan_4->setObjectName(QStringLiteral("chan_4"));
        chan_4->setFont(font1);

        verticalLayout->addWidget(chan_4);

        chan_5 = new QCheckBox(groupBox);
        chan_5->setObjectName(QStringLiteral("chan_5"));
        chan_5->setFont(font1);

        verticalLayout->addWidget(chan_5);

        chan_6 = new QCheckBox(groupBox);
        chan_6->setObjectName(QStringLiteral("chan_6"));
        chan_6->setFont(font1);

        verticalLayout->addWidget(chan_6);

        chan_7 = new QCheckBox(groupBox);
        chan_7->setObjectName(QStringLiteral("chan_7"));
        chan_7->setFont(font1);

        verticalLayout->addWidget(chan_7);

        chan_8 = new QCheckBox(groupBox);
        chan_8->setObjectName(QStringLiteral("chan_8"));
        chan_8->setFont(font1);

        verticalLayout->addWidget(chan_8);

        chan_9 = new QCheckBox(groupBox);
        chan_9->setObjectName(QStringLiteral("chan_9"));
        chan_9->setFont(font1);

        verticalLayout->addWidget(chan_9);

        chan_10 = new QCheckBox(groupBox);
        chan_10->setObjectName(QStringLiteral("chan_10"));
        chan_10->setFont(font1);

        verticalLayout->addWidget(chan_10);

        chan_11 = new QCheckBox(groupBox);
        chan_11->setObjectName(QStringLiteral("chan_11"));
        chan_11->setFont(font1);

        verticalLayout->addWidget(chan_11);

        chan_12 = new QCheckBox(groupBox);
        chan_12->setObjectName(QStringLiteral("chan_12"));
        chan_12->setFont(font1);

        verticalLayout->addWidget(chan_12);

        chan_13 = new QCheckBox(groupBox);
        chan_13->setObjectName(QStringLiteral("chan_13"));
        chan_13->setFont(font1);

        verticalLayout->addWidget(chan_13);

        chan_14 = new QCheckBox(groupBox);
        chan_14->setObjectName(QStringLiteral("chan_14"));
        chan_14->setFont(font1);

        verticalLayout->addWidget(chan_14);


        verticalLayout_3->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 133, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);

        horizontalLayout_2->setStretch(0, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_2->setStretch(1, 1);

        retranslateUi(ThousandRectsInConcurentThreadsClass);

        QMetaObject::connectSlotsByName(ThousandRectsInConcurentThreadsClass);
    } // setupUi

    void retranslateUi(QWidget *ThousandRectsInConcurentThreadsClass)
    {
        ThousandRectsInConcurentThreadsClass->setWindowTitle(QApplication::translate("ThousandRectsInConcurentThreadsClass", "ThousandRectsInConcurentThreads", 0));
        pushButton->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "PushButton", 0));
        checkBox->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        checkBox_2->setText(QApplication::translate("ThousandRectsInConcurentThreadsClass", "CheckBox", 0));
        groupBox->setTitle(QApplication::translate("ThousandRectsInConcurentThreadsClass", "\320\224\320\265\321\204\320\265\320\272\321\202\320\276\321\201\320\272\320\276\320\277", 0));
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
