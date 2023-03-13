/********************************************************************************
** Form generated from reading UI file 'dial_control_par.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAL_CONTROL_PAR_H
#define UI_DIAL_CONTROL_PAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "paramlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_dial_control_par
{
public:
    QLabel *label_thick_units;
    QLabel *label_diam;
    QLabel *label_diam_units;
    ParamLineEdit *ed_thick;
    QLabel *label_thick;
    ParamLineEdit *ed_diam;
    QLabel *label_thick_lim_neg;
    ParamLineEdit *ed_thick_lim_neg;
    QLabel *label_thick_lim_neg_units;
    QLabel *label_thick_lim_pos;
    QLabel *label_thick_lim_pos_units;
    ParamLineEdit *ed_thick_lim_pos;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QCheckBox *checkBox_relative_thick;

    void setupUi(QDialog *dial_control_par)
    {
        if (dial_control_par->objectName().isEmpty())
            dial_control_par->setObjectName(QStringLiteral("dial_control_par"));
        dial_control_par->resize(263, 192);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dial_control_par->sizePolicy().hasHeightForWidth());
        dial_control_par->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        dial_control_par->setFont(font);
        label_thick_units = new QLabel(dial_control_par);
        label_thick_units->setObjectName(QStringLiteral("label_thick_units"));
        label_thick_units->setGeometry(QRect(214, 40, 21, 16));
        label_diam = new QLabel(dial_control_par);
        label_diam->setObjectName(QStringLiteral("label_diam"));
        label_diam->setGeometry(QRect(10, 14, 145, 21));
        label_diam->setFrameShape(QFrame::Panel);
        label_diam->setFrameShadow(QFrame::Sunken);
        label_diam_units = new QLabel(dial_control_par);
        label_diam_units->setObjectName(QStringLiteral("label_diam_units"));
        label_diam_units->setGeometry(QRect(214, 16, 21, 16));
        ed_thick = new ParamLineEdit(dial_control_par);
        ed_thick->setObjectName(QStringLiteral("ed_thick"));
        ed_thick->setGeometry(QRect(157, 37, 51, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ed_thick->sizePolicy().hasHeightForWidth());
        ed_thick->setSizePolicy(sizePolicy1);
        ed_thick->setAlignment(Qt::AlignCenter);
        label_thick = new QLabel(dial_control_par);
        label_thick->setObjectName(QStringLiteral("label_thick"));
        label_thick->setGeometry(QRect(10, 37, 145, 21));
        label_thick->setFrameShape(QFrame::Panel);
        label_thick->setFrameShadow(QFrame::Sunken);
        ed_diam = new ParamLineEdit(dial_control_par);
        ed_diam->setObjectName(QStringLiteral("ed_diam"));
        ed_diam->setGeometry(QRect(157, 14, 51, 20));
        sizePolicy1.setHeightForWidth(ed_diam->sizePolicy().hasHeightForWidth());
        ed_diam->setSizePolicy(sizePolicy1);
        ed_diam->setAlignment(Qt::AlignCenter);
        label_thick_lim_neg = new QLabel(dial_control_par);
        label_thick_lim_neg->setObjectName(QStringLiteral("label_thick_lim_neg"));
        label_thick_lim_neg->setGeometry(QRect(11, 82, 131, 21));
        label_thick_lim_neg->setFrameShape(QFrame::Panel);
        label_thick_lim_neg->setFrameShadow(QFrame::Sunken);
        ed_thick_lim_neg = new ParamLineEdit(dial_control_par);
        ed_thick_lim_neg->setObjectName(QStringLiteral("ed_thick_lim_neg"));
        ed_thick_lim_neg->setGeometry(QRect(144, 81, 51, 20));
        sizePolicy1.setHeightForWidth(ed_thick_lim_neg->sizePolicy().hasHeightForWidth());
        ed_thick_lim_neg->setSizePolicy(sizePolicy1);
        ed_thick_lim_neg->setAlignment(Qt::AlignCenter);
        label_thick_lim_neg_units = new QLabel(dial_control_par);
        label_thick_lim_neg_units->setObjectName(QStringLiteral("label_thick_lim_neg_units"));
        label_thick_lim_neg_units->setGeometry(QRect(201, 84, 21, 16));
        label_thick_lim_pos = new QLabel(dial_control_par);
        label_thick_lim_pos->setObjectName(QStringLiteral("label_thick_lim_pos"));
        label_thick_lim_pos->setGeometry(QRect(11, 105, 131, 21));
        label_thick_lim_pos->setFrameShape(QFrame::Panel);
        label_thick_lim_pos->setFrameShadow(QFrame::Sunken);
        label_thick_lim_pos_units = new QLabel(dial_control_par);
        label_thick_lim_pos_units->setObjectName(QStringLiteral("label_thick_lim_pos_units"));
        label_thick_lim_pos_units->setGeometry(QRect(201, 107, 21, 16));
        ed_thick_lim_pos = new ParamLineEdit(dial_control_par);
        ed_thick_lim_pos->setObjectName(QStringLiteral("ed_thick_lim_pos"));
        ed_thick_lim_pos->setGeometry(QRect(144, 104, 51, 20));
        sizePolicy1.setHeightForWidth(ed_thick_lim_pos->sizePolicy().hasHeightForWidth());
        ed_thick_lim_pos->setSizePolicy(sizePolicy1);
        ed_thick_lim_pos->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(dial_control_par);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 150, 201, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setMinimumSize(QSize(85, 0));
        okButton->setFont(font);

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setFont(font);

        hboxLayout->addWidget(cancelButton);

        checkBox_relative_thick = new QCheckBox(dial_control_par);
        checkBox_relative_thick->setObjectName(QStringLiteral("checkBox_relative_thick"));
        checkBox_relative_thick->setGeometry(QRect(10, 60, 191, 21));
        QFont font1;
        font1.setPointSize(9);
        checkBox_relative_thick->setFont(font1);
        QWidget::setTabOrder(ed_diam, ed_thick);
        QWidget::setTabOrder(ed_thick, ed_thick_lim_neg);
        QWidget::setTabOrder(ed_thick_lim_neg, ed_thick_lim_pos);
        QWidget::setTabOrder(ed_thick_lim_pos, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(dial_control_par);
        QObject::connect(okButton, SIGNAL(clicked()), dial_control_par, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), dial_control_par, SLOT(reject()));

        QMetaObject::connectSlotsByName(dial_control_par);
    } // setupUi

    void retranslateUi(QDialog *dial_control_par)
    {
        dial_control_par->setWindowTitle(QApplication::translate("dial_control_par", "\320\245\320\260\321\200\320\260\320\272\321\202\320\265\321\200\320\270\321\201\321\202\320\270\320\272\320\270 \320\276\320\261\321\212\320\265\320\272\321\202\320\260 \320\270 \320\272\320\276\320\275\321\202\321\200\320\276\320\273\321\217", 0));
        label_thick_units->setText(QApplication::translate("dial_control_par", "\320\274\320\274", 0));
        label_diam->setText(QApplication::translate("dial_control_par", "\320\224\320\270\320\260\320\274\320\265\321\202\321\200 \321\202\321\200\321\203\320\261\321\213", 0));
        label_diam_units->setText(QApplication::translate("dial_control_par", "\320\274\320\274", 0));
        label_thick->setText(QApplication::translate("dial_control_par", "\320\242\320\276\320\273\321\211\320\270\320\275\320\260 \321\201\321\202\320\265\320\275\320\272\320\270 \321\202\321\200\321\203\320\261\321\213", 0));
        label_thick_lim_neg->setText(QApplication::translate("dial_control_par", "\320\264\320\276\320\277\321\203\321\201\320\272 \321\202\320\276\320\273\321\211\320\270\320\275\321\213 \"\342\200\223\"", 0));
        label_thick_lim_neg_units->setText(QApplication::translate("dial_control_par", "\320\274\320\274", 0));
        label_thick_lim_pos->setText(QApplication::translate("dial_control_par", "\320\264\320\276\320\277\321\203\321\201\320\272 \321\202\320\276\320\273\321\211\320\270\320\275\321\213 \"+\"", 0));
        label_thick_lim_pos_units->setText(QApplication::translate("dial_control_par", "\320\274\320\274", 0));
        okButton->setText(QApplication::translate("dial_control_par", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", 0));
        cancelButton->setText(QApplication::translate("dial_control_par", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
        checkBox_relative_thick->setText(QApplication::translate("dial_control_par", "\320\236\321\202\320\275\320\276\321\201\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \320\264\320\276\320\277\321\203\321\201\320\272", 0));
    } // retranslateUi

};

namespace Ui {
    class dial_control_par: public Ui_dial_control_par {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAL_CONTROL_PAR_H
