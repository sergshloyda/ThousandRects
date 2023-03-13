/********************************************************************************
** Form generated from reading UI file 'ParamEditToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMEDITTOOLBAR_H
#define UI_PARAMEDITTOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "checkboxsel.h"
#include "comboboxsel.h"
#include "devicegroupbox.h"
#include "manualspeedlbl.h"
#include "paramendrlineeditsel.h"
#include "paramlineeditsel.h"
#include "pushbuttonsel.h"

QT_BEGIN_NAMESPACE

class Ui_ParamEditToolBar
{
public:
    QVBoxLayout *verticalLayout;
    DeviceGroupBox *groupBox_defectoscop;
    QVBoxLayout *verticalLayout_3;
    QWidget *wid_menu_Layout;
    QHBoxLayout *menu_Layout;
    QFrame *frame_common;
    PushButtonSel *button_object_par;
    QLabel *label_a_des;
    CheckBoxSel *checkBox_chan_us_set;
    ParamLineEditSel *ed_chan;
    QLabel *label_chan;
    QLabel *label_signal_defect;
    QLabel *label_signal_ak;
    QLabel *label_a_des_2;
    CheckBoxSel *checkBox_param_pos_edit;
    QFrame *frame_osc;
    ParamLineEditSel *ed_tv;
    QLabel *label_tv;
    ParamLineEditSel *ed_tt;
    QLabel *label_tt;
    ParamLineEditSel *ed_kus;
    ComboBoxSel *comboBox_rej;
    QLabel *label_kus;
    QLabel *label_begr;
    ParamEndRLineEditSel *ed_endr;
    QLabel *label_endr;
    ParamLineEditSel *ed_begr;
    ParamLineEditSel *ed_ninp_us;
    QLabel *label_ninp_us;
    QLabel *label_tact;
    ParamLineEditSel *ed_tact;
    QLabel *label_us_rej;
    ComboBoxSel *comboBox_detpar;
    PushButtonSel *button_paste;
    PushButtonSel *button_copy;
    QFrame *frame_strob;
    QLabel *label_bstr;
    QLabel *label_lstr;
    QLabel *label_nstrb;
    QLabel *label_por;
    ParamLineEditSel *ed_bstr;
    ParamLineEditSel *ed_lstr;
    ParamLineEditSel *ed_por;
    ParamLineEditSel *ed_nstrob;
    QLabel *label_strb_type;
    CheckBoxSel *checkBox_extra_strob_por;
    ParamLineEditSel *ed_por_2;
    QLabel *label_por_1a;
    QLabel *label_por_2;
    ParamLineEditSel *ed_por_3;
    QLabel *label_por_2a;
    QLabel *label_por_3;
    QLabel *label_por_1a_val;
    QLabel *label_por_2a_val;
    QFrame *frame_b_scan;
    QLabel *label_b_begr;
    QLabel *label_b_count;
    QLabel *label_b_stepr;
    QLabel *label_b_por;
    ParamLineEditSel *ed_b_begr;
    ParamEndRLineEditSel *ed_b_count;
    ParamLineEditSel *ed_b_por;
    ParamEndRLineEditSel *ed_b_stepr;
    QFrame *frame_vrch_on_of;
    CheckBoxSel *checkBox_vrch;
    QFrame *frame_vrch;
    QLabel *label_vtime;
    ParamLineEditSel *ed_kusv;
    ParamLineEditSel *ed_np_vrch;
    QLabel *label_kusv;
    ParamLineEditSel *ed_timev;
    QLabel *label_npointv;
    QLabel *label_numpointv;
    ParamLineEditSel *ed_nump_vrch;
    QFrame *frame_generator;
    ComboBoxSel *comboBox_npow;
    QLabel *label_ngen;
    ParamLineEditSel *ed_len_imp;
    QLabel *label_leni;
    ParamLineEditSel *ed_ngen;
    ComboBoxSel *comboBox_val_pow;
    QFrame *frame_object;
    QLabel *label_n_rez;
    ParamLineEditSel *ed_n_rez;
    QLabel *label_t_pr;
    ParamLineEditSel *ed_t_pr;
    ParamLineEditSel *ed_thick_corr;
    QLabel *label_queue_len;
    ParamLineEditSel *ed_thick_dt;
    QLabel *label_cc;
    QLabel *label_alpha;
    ParamLineEditSel *ed_cc;
    QLabel *label_thick_dt;
    ParamLineEditSel *ed_alpha;
    QLabel *label_thick_corr;
    QFrame *frame_scale;
    QLabel *label_coord_descr;
    QLabel *label_coord_units_descr;
    PushButtonSel *button_reset_coord;
    CheckBoxSel *checkBox_scale_mode;
    QLabel *label_coord;
    QFrame *frame_manual_speed;
    QVBoxLayout *verticalLayout_4;
    ManualSpeedLbl *label_manual_speed;
    QFrame *frame_debug;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_start_stop;
    QPushButton *pushButt_debug;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_save_osc;
    QLineEdit *ed_osc_capt;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ParamEditToolBar)
    {
        if (ParamEditToolBar->objectName().isEmpty())
            ParamEditToolBar->setObjectName(QStringLiteral("ParamEditToolBar"));
        ParamEditToolBar->resize(231, 944);
        verticalLayout = new QVBoxLayout(ParamEditToolBar);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_defectoscop = new DeviceGroupBox(ParamEditToolBar);
        groupBox_defectoscop->setObjectName(QStringLiteral("groupBox_defectoscop"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_defectoscop->sizePolicy().hasHeightForWidth());
        groupBox_defectoscop->setSizePolicy(sizePolicy);
        groupBox_defectoscop->setMinimumSize(QSize(200, 821));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(13);
        groupBox_defectoscop->setFont(font);
        verticalLayout_3 = new QVBoxLayout(groupBox_defectoscop);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 3, -1, 3);
        wid_menu_Layout = new QWidget(groupBox_defectoscop);
        wid_menu_Layout->setObjectName(QStringLiteral("wid_menu_Layout"));
        wid_menu_Layout->setMinimumSize(QSize(171, 28));
        wid_menu_Layout->setMaximumSize(QSize(16777215, 28));
        menu_Layout = new QHBoxLayout(wid_menu_Layout);
        menu_Layout->setObjectName(QStringLiteral("menu_Layout"));
        menu_Layout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_3->addWidget(wid_menu_Layout);

        frame_common = new QFrame(groupBox_defectoscop);
        frame_common->setObjectName(QStringLiteral("frame_common"));
        frame_common->setMinimumSize(QSize(192, 90));
        frame_common->setMaximumSize(QSize(192, 90));
        frame_common->setFrameShape(QFrame::Box);
        frame_common->setFrameShadow(QFrame::Sunken);
        button_object_par = new PushButtonSel(frame_common);
        button_object_par->setObjectName(QStringLiteral("button_object_par"));
        button_object_par->setGeometry(QRect(0, 35, 101, 23));
        QFont font1;
        font1.setPointSize(10);
        button_object_par->setFont(font1);
        button_object_par->setAutoDefault(true);
        button_object_par->setDefault(true);
        label_a_des = new QLabel(frame_common);
        label_a_des->setObjectName(QStringLiteral("label_a_des"));
        label_a_des->setGeometry(QRect(27, 10, 21, 21));
        label_a_des->setFont(font1);
        checkBox_chan_us_set = new CheckBoxSel(frame_common);
        checkBox_chan_us_set->setObjectName(QStringLiteral("checkBox_chan_us_set"));
        checkBox_chan_us_set->setGeometry(QRect(86, 65, 101, 20));
        checkBox_chan_us_set->setFont(font1);
        checkBox_chan_us_set->setLayoutDirection(Qt::RightToLeft);
        ed_chan = new ParamLineEditSel(frame_common);
        ed_chan->setObjectName(QStringLiteral("ed_chan"));
        ed_chan->setGeometry(QRect(52, 63, 22, 22));
        ed_chan->setFont(font1);
        ed_chan->setAlignment(Qt::AlignCenter);
        label_chan = new QLabel(frame_common);
        label_chan->setObjectName(QStringLiteral("label_chan"));
        label_chan->setGeometry(QRect(9, 63, 41, 20));
        label_chan->setFont(font1);
        label_signal_defect = new QLabel(frame_common);
        label_signal_defect->setObjectName(QStringLiteral("label_signal_defect"));
        label_signal_defect->setGeometry(QRect(129, 11, 21, 21));
        QFont font2;
        font2.setPointSize(12);
        label_signal_defect->setFont(font2);
        label_signal_defect->setStyleSheet(QStringLiteral("background-color: white"));
        label_signal_defect->setFrameShape(QFrame::Box);
        label_signal_defect->setAlignment(Qt::AlignCenter);
        label_signal_ak = new QLabel(frame_common);
        label_signal_ak->setObjectName(QStringLiteral("label_signal_ak"));
        label_signal_ak->setGeometry(QRect(50, 10, 21, 21));
        label_signal_ak->setFont(font2);
        label_signal_ak->setStyleSheet(QStringLiteral("background-color: white"));
        label_signal_ak->setFrameShape(QFrame::Box);
        label_signal_ak->setAlignment(Qt::AlignCenter);
        label_a_des_2 = new QLabel(frame_common);
        label_a_des_2->setObjectName(QStringLiteral("label_a_des_2"));
        label_a_des_2->setGeometry(QRect(82, 10, 51, 21));
        label_a_des_2->setFont(font1);
        checkBox_param_pos_edit = new CheckBoxSel(frame_common);
        checkBox_param_pos_edit->setObjectName(QStringLiteral("checkBox_param_pos_edit"));
        checkBox_param_pos_edit->setGeometry(QRect(104, 37, 86, 20));
        checkBox_param_pos_edit->setFont(font1);
        checkBox_param_pos_edit->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_3->addWidget(frame_common);

        frame_osc = new QFrame(groupBox_defectoscop);
        frame_osc->setObjectName(QStringLiteral("frame_osc"));
        frame_osc->setMinimumSize(QSize(192, 151));
        frame_osc->setMaximumSize(QSize(192, 151));
        frame_osc->setFrameShape(QFrame::Box);
        frame_osc->setFrameShadow(QFrame::Sunken);
        ed_tv = new ParamLineEditSel(frame_osc);
        ed_tv->setObjectName(QStringLiteral("ed_tv"));
        ed_tv->setGeometry(QRect(144, 127, 21, 22));
        ed_tv->setFont(font1);
        ed_tv->setAlignment(Qt::AlignCenter);
        label_tv = new QLabel(frame_osc);
        label_tv->setObjectName(QStringLiteral("label_tv"));
        label_tv->setGeometry(QRect(111, 129, 21, 16));
        label_tv->setFont(font1);
        ed_tt = new ParamLineEditSel(frame_osc);
        ed_tt->setObjectName(QStringLiteral("ed_tt"));
        ed_tt->setGeometry(QRect(53, 127, 21, 22));
        ed_tt->setFont(font1);
        ed_tt->setAlignment(Qt::AlignCenter);
        label_tt = new QLabel(frame_osc);
        label_tt->setObjectName(QStringLiteral("label_tt"));
        label_tt->setGeometry(QRect(22, 129, 21, 16));
        label_tt->setFont(font1);
        ed_kus = new ParamLineEditSel(frame_osc);
        ed_kus->setObjectName(QStringLiteral("ed_kus"));
        ed_kus->setGeometry(QRect(43, 102, 41, 22));
        ed_kus->setFont(font1);
        ed_kus->setAlignment(Qt::AlignCenter);
        comboBox_rej = new ComboBoxSel(frame_osc);
        comboBox_rej->setObjectName(QStringLiteral("comboBox_rej"));
        comboBox_rej->setGeometry(QRect(103, 6, 81, 22));
        comboBox_rej->setFont(font1);
        comboBox_rej->setLayoutDirection(Qt::RightToLeft);
        comboBox_rej->setMaxCount(2);
        label_kus = new QLabel(frame_osc);
        label_kus->setObjectName(QStringLiteral("label_kus"));
        label_kus->setGeometry(QRect(9, 104, 31, 16));
        label_kus->setFont(font1);
        label_begr = new QLabel(frame_osc);
        label_begr->setObjectName(QStringLiteral("label_begr"));
        label_begr->setGeometry(QRect(8, 77, 31, 21));
        label_begr->setFont(font1);
        ed_endr = new ParamEndRLineEditSel(frame_osc);
        ed_endr->setObjectName(QStringLiteral("ed_endr"));
        ed_endr->setGeometry(QRect(126, 77, 60, 22));
        ed_endr->setFont(font1);
        ed_endr->setAlignment(Qt::AlignCenter);
        label_endr = new QLabel(frame_osc);
        label_endr->setObjectName(QStringLiteral("label_endr"));
        label_endr->setGeometry(QRect(102, 77, 31, 21));
        label_endr->setFont(font1);
        ed_begr = new ParamLineEditSel(frame_osc);
        ed_begr->setObjectName(QStringLiteral("ed_begr"));
        ed_begr->setGeometry(QRect(34, 77, 60, 22));
        ed_begr->setFont(font1);
        ed_begr->setAlignment(Qt::AlignCenter);
        ed_ninp_us = new ParamLineEditSel(frame_osc);
        ed_ninp_us->setObjectName(QStringLiteral("ed_ninp_us"));
        ed_ninp_us->setGeometry(QRect(51, 30, 22, 22));
        ed_ninp_us->setFont(font1);
        ed_ninp_us->setAlignment(Qt::AlignCenter);
        label_ninp_us = new QLabel(frame_osc);
        label_ninp_us->setObjectName(QStringLiteral("label_ninp_us"));
        label_ninp_us->setGeometry(QRect(9, 30, 41, 21));
        label_ninp_us->setFont(font1);
        label_tact = new QLabel(frame_osc);
        label_tact->setObjectName(QStringLiteral("label_tact"));
        label_tact->setGeometry(QRect(9, 5, 31, 20));
        label_tact->setFont(font1);
        ed_tact = new ParamLineEditSel(frame_osc);
        ed_tact->setObjectName(QStringLiteral("ed_tact"));
        ed_tact->setGeometry(QRect(51, 5, 22, 22));
        ed_tact->setFont(font1);
        ed_tact->setAlignment(Qt::AlignCenter);
        label_us_rej = new QLabel(frame_osc);
        label_us_rej->setObjectName(QStringLiteral("label_us_rej"));
        label_us_rej->setGeometry(QRect(102, 30, 82, 23));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        label_us_rej->setFont(font3);
        label_us_rej->setFrameShape(QFrame::Box);
        label_us_rej->setFrameShadow(QFrame::Raised);
        label_us_rej->setAlignment(Qt::AlignCenter);
        comboBox_detpar = new ComboBoxSel(frame_osc);
        comboBox_detpar->setObjectName(QStringLiteral("comboBox_detpar"));
        comboBox_detpar->setGeometry(QRect(105, 102, 81, 22));
        comboBox_detpar->setFont(font1);
        button_paste = new PushButtonSel(frame_osc);
        button_paste->setObjectName(QStringLiteral("button_paste"));
        button_paste->setGeometry(QRect(102, 54, 82, 21));
        QFont font4;
        font4.setPointSize(9);
        button_paste->setFont(font4);
        button_paste->setFocusPolicy(Qt::NoFocus);
        button_copy = new PushButtonSel(frame_osc);
        button_copy->setObjectName(QStringLiteral("button_copy"));
        button_copy->setGeometry(QRect(20, 54, 82, 21));
        button_copy->setFont(font4);
        button_copy->setFocusPolicy(Qt::NoFocus);

        verticalLayout_3->addWidget(frame_osc);

        frame_strob = new QFrame(groupBox_defectoscop);
        frame_strob->setObjectName(QStringLiteral("frame_strob"));
        frame_strob->setMinimumSize(QSize(192, 129));
        frame_strob->setMaximumSize(QSize(192, 129));
        frame_strob->setFrameShape(QFrame::Box);
        frame_strob->setFrameShadow(QFrame::Sunken);
        label_bstr = new QLabel(frame_strob);
        label_bstr->setObjectName(QStringLiteral("label_bstr"));
        label_bstr->setGeometry(QRect(5, 29, 31, 21));
        label_bstr->setFont(font1);
        label_lstr = new QLabel(frame_strob);
        label_lstr->setObjectName(QStringLiteral("label_lstr"));
        label_lstr->setGeometry(QRect(98, 29, 31, 20));
        label_lstr->setFont(font1);
        label_nstrb = new QLabel(frame_strob);
        label_nstrb->setObjectName(QStringLiteral("label_nstrb"));
        label_nstrb->setGeometry(QRect(3, 4, 51, 21));
        label_nstrb->setFont(font1);
        label_por = new QLabel(frame_strob);
        label_por->setObjectName(QStringLiteral("label_por"));
        label_por->setGeometry(QRect(105, 4, 41, 21));
        label_por->setFont(font1);
        ed_bstr = new ParamLineEditSel(frame_strob);
        ed_bstr->setObjectName(QStringLiteral("ed_bstr"));
        ed_bstr->setGeometry(QRect(32, 29, 60, 22));
        ed_bstr->setFont(font1);
        ed_bstr->setAlignment(Qt::AlignCenter);
        ed_lstr = new ParamLineEditSel(frame_strob);
        ed_lstr->setObjectName(QStringLiteral("ed_lstr"));
        ed_lstr->setGeometry(QRect(127, 29, 60, 22));
        ed_lstr->setFont(font1);
        ed_lstr->setAlignment(Qt::AlignCenter);
        ed_por = new ParamLineEditSel(frame_strob);
        ed_por->setObjectName(QStringLiteral("ed_por"));
        ed_por->setGeometry(QRect(145, 4, 41, 22));
        ed_por->setFont(font1);
        ed_por->setAlignment(Qt::AlignCenter);
        ed_nstrob = new ParamLineEditSel(frame_strob);
        ed_nstrob->setObjectName(QStringLiteral("ed_nstrob"));
        ed_nstrob->setGeometry(QRect(71, 5, 21, 22));
        ed_nstrob->setFont(font1);
        ed_nstrob->setAlignment(Qt::AlignCenter);
        label_strb_type = new QLabel(frame_strob);
        label_strb_type->setObjectName(QStringLiteral("label_strb_type"));
        label_strb_type->setGeometry(QRect(40, 4, 31, 21));
        label_strb_type->setFont(font1);
        label_strb_type->setAlignment(Qt::AlignCenter);
        checkBox_extra_strob_por = new CheckBoxSel(frame_strob);
        checkBox_extra_strob_por->setObjectName(QStringLiteral("checkBox_extra_strob_por"));
        checkBox_extra_strob_por->setGeometry(QRect(5, 52, 157, 20));
        checkBox_extra_strob_por->setFont(font1);
        checkBox_extra_strob_por->setLayoutDirection(Qt::RightToLeft);
        ed_por_2 = new ParamLineEditSel(frame_strob);
        ed_por_2->setObjectName(QStringLiteral("ed_por_2"));
        ed_por_2->setGeometry(QRect(146, 74, 41, 22));
        ed_por_2->setFont(font1);
        ed_por_2->setAlignment(Qt::AlignCenter);
        label_por_1a = new QLabel(frame_strob);
        label_por_1a->setObjectName(QStringLiteral("label_por_1a"));
        label_por_1a->setGeometry(QRect(5, 75, 43, 21));
        label_por_1a->setFont(font1);
        label_por_2 = new QLabel(frame_strob);
        label_por_2->setObjectName(QStringLiteral("label_por_2"));
        label_por_2->setGeometry(QRect(99, 75, 43, 21));
        label_por_2->setFont(font1);
        ed_por_3 = new ParamLineEditSel(frame_strob);
        ed_por_3->setObjectName(QStringLiteral("ed_por_3"));
        ed_por_3->setGeometry(QRect(146, 99, 41, 22));
        ed_por_3->setFont(font1);
        ed_por_3->setAlignment(Qt::AlignCenter);
        label_por_2a = new QLabel(frame_strob);
        label_por_2a->setObjectName(QStringLiteral("label_por_2a"));
        label_por_2a->setGeometry(QRect(4, 100, 43, 21));
        label_por_2a->setFont(font1);
        label_por_3 = new QLabel(frame_strob);
        label_por_3->setObjectName(QStringLiteral("label_por_3"));
        label_por_3->setGeometry(QRect(99, 100, 43, 21));
        label_por_3->setFont(font1);
        label_por_1a_val = new QLabel(frame_strob);
        label_por_1a_val->setObjectName(QStringLiteral("label_por_1a_val"));
        label_por_1a_val->setGeometry(QRect(52, 76, 41, 22));
        label_por_1a_val->setFont(font1);
        label_por_1a_val->setFrameShape(QFrame::Panel);
        label_por_1a_val->setFrameShadow(QFrame::Sunken);
        label_por_1a_val->setAlignment(Qt::AlignCenter);
        label_por_2a_val = new QLabel(frame_strob);
        label_por_2a_val->setObjectName(QStringLiteral("label_por_2a_val"));
        label_por_2a_val->setGeometry(QRect(52, 100, 41, 22));
        label_por_2a_val->setFont(font1);
        label_por_2a_val->setFrameShape(QFrame::Panel);
        label_por_2a_val->setFrameShadow(QFrame::Sunken);
        label_por_2a_val->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(frame_strob);

        frame_b_scan = new QFrame(groupBox_defectoscop);
        frame_b_scan->setObjectName(QStringLiteral("frame_b_scan"));
        frame_b_scan->setMinimumSize(QSize(192, 59));
        frame_b_scan->setMaximumSize(QSize(192, 59));
        frame_b_scan->setFrameShape(QFrame::Box);
        frame_b_scan->setFrameShadow(QFrame::Sunken);
        label_b_begr = new QLabel(frame_b_scan);
        label_b_begr->setObjectName(QStringLiteral("label_b_begr"));
        label_b_begr->setGeometry(QRect(5, 29, 31, 21));
        label_b_begr->setFont(font1);
        label_b_count = new QLabel(frame_b_scan);
        label_b_count->setObjectName(QStringLiteral("label_b_count"));
        label_b_count->setGeometry(QRect(98, 29, 31, 20));
        label_b_count->setFont(font1);
        label_b_stepr = new QLabel(frame_b_scan);
        label_b_stepr->setObjectName(QStringLiteral("label_b_stepr"));
        label_b_stepr->setGeometry(QRect(3, 4, 31, 21));
        label_b_stepr->setFont(font1);
        label_b_por = new QLabel(frame_b_scan);
        label_b_por->setObjectName(QStringLiteral("label_b_por"));
        label_b_por->setGeometry(QRect(97, 4, 41, 21));
        label_b_por->setFont(font1);
        ed_b_begr = new ParamLineEditSel(frame_b_scan);
        ed_b_begr->setObjectName(QStringLiteral("ed_b_begr"));
        ed_b_begr->setGeometry(QRect(32, 29, 60, 22));
        ed_b_begr->setFont(font1);
        ed_b_begr->setAlignment(Qt::AlignCenter);
        ed_b_count = new ParamEndRLineEditSel(frame_b_scan);
        ed_b_count->setObjectName(QStringLiteral("ed_b_count"));
        ed_b_count->setGeometry(QRect(127, 29, 60, 22));
        ed_b_count->setFont(font1);
        ed_b_count->setAlignment(Qt::AlignCenter);
        ed_b_por = new ParamLineEditSel(frame_b_scan);
        ed_b_por->setObjectName(QStringLiteral("ed_b_por"));
        ed_b_por->setGeometry(QRect(137, 4, 41, 22));
        ed_b_por->setFont(font1);
        ed_b_por->setAlignment(Qt::AlignCenter);
        ed_b_stepr = new ParamEndRLineEditSel(frame_b_scan);
        ed_b_stepr->setObjectName(QStringLiteral("ed_b_stepr"));
        ed_b_stepr->setGeometry(QRect(36, 5, 52, 22));
        ed_b_stepr->setFont(font1);
        ed_b_stepr->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(frame_b_scan);

        frame_vrch_on_of = new QFrame(groupBox_defectoscop);
        frame_vrch_on_of->setObjectName(QStringLiteral("frame_vrch_on_of"));
        frame_vrch_on_of->setMinimumSize(QSize(192, 25));
        frame_vrch_on_of->setMaximumSize(QSize(192, 25));
        frame_vrch_on_of->setFrameShape(QFrame::StyledPanel);
        frame_vrch_on_of->setFrameShadow(QFrame::Raised);
        checkBox_vrch = new CheckBoxSel(frame_vrch_on_of);
        checkBox_vrch->setObjectName(QStringLiteral("checkBox_vrch"));
        checkBox_vrch->setGeometry(QRect(10, 0, 71, 20));
        checkBox_vrch->setFont(font1);
        checkBox_vrch->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_3->addWidget(frame_vrch_on_of);

        frame_vrch = new QFrame(groupBox_defectoscop);
        frame_vrch->setObjectName(QStringLiteral("frame_vrch"));
        frame_vrch->setMinimumSize(QSize(192, 59));
        frame_vrch->setMaximumSize(QSize(192, 59));
        frame_vrch->setFrameShape(QFrame::Box);
        frame_vrch->setFrameShadow(QFrame::Sunken);
        label_vtime = new QLabel(frame_vrch);
        label_vtime->setObjectName(QStringLiteral("label_vtime"));
        label_vtime->setGeometry(QRect(5, 30, 51, 21));
        label_vtime->setFont(font1);
        ed_kusv = new ParamLineEditSel(frame_vrch);
        ed_kusv->setObjectName(QStringLiteral("ed_kusv"));
        ed_kusv->setGeometry(QRect(154, 30, 31, 22));
        ed_kusv->setFont(font1);
        ed_kusv->setAlignment(Qt::AlignCenter);
        ed_np_vrch = new ParamLineEditSel(frame_vrch);
        ed_np_vrch->setObjectName(QStringLiteral("ed_np_vrch"));
        ed_np_vrch->setGeometry(QRect(56, 4, 21, 22));
        ed_np_vrch->setFont(font1);
        ed_np_vrch->setAlignment(Qt::AlignCenter);
        label_kusv = new QLabel(frame_vrch);
        label_kusv->setObjectName(QStringLiteral("label_kusv"));
        label_kusv->setGeometry(QRect(122, 30, 31, 21));
        label_kusv->setFont(font1);
        ed_timev = new ParamLineEditSel(frame_vrch);
        ed_timev->setObjectName(QStringLiteral("ed_timev"));
        ed_timev->setGeometry(QRect(53, 30, 60, 22));
        ed_timev->setFont(font1);
        ed_timev->setAlignment(Qt::AlignCenter);
        label_npointv = new QLabel(frame_vrch);
        label_npointv->setObjectName(QStringLiteral("label_npointv"));
        label_npointv->setGeometry(QRect(5, 3, 51, 21));
        label_npointv->setFont(font1);
        label_numpointv = new QLabel(frame_vrch);
        label_numpointv->setObjectName(QStringLiteral("label_numpointv"));
        label_numpointv->setGeometry(QRect(78, 5, 10, 21));
        label_numpointv->setFont(font1);
        label_numpointv->setAlignment(Qt::AlignCenter);
        ed_nump_vrch = new ParamLineEditSel(frame_vrch);
        ed_nump_vrch->setObjectName(QStringLiteral("ed_nump_vrch"));
        ed_nump_vrch->setGeometry(QRect(90, 4, 21, 22));
        ed_nump_vrch->setFont(font1);
        ed_nump_vrch->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(frame_vrch);

        frame_generator = new QFrame(groupBox_defectoscop);
        frame_generator->setObjectName(QStringLiteral("frame_generator"));
        frame_generator->setMinimumSize(QSize(192, 59));
        frame_generator->setMaximumSize(QSize(192, 59));
        frame_generator->setFrameShape(QFrame::Box);
        frame_generator->setFrameShadow(QFrame::Sunken);
        comboBox_npow = new ComboBoxSel(frame_generator);
        comboBox_npow->setObjectName(QStringLiteral("comboBox_npow"));
        comboBox_npow->setGeometry(QRect(4, 32, 96, 22));
        comboBox_npow->setFont(font1);
        label_ngen = new QLabel(frame_generator);
        label_ngen->setObjectName(QStringLiteral("label_ngen"));
        label_ngen->setGeometry(QRect(8, 5, 31, 21));
        label_ngen->setFont(font1);
        ed_len_imp = new ParamLineEditSel(frame_generator);
        ed_len_imp->setObjectName(QStringLiteral("ed_len_imp"));
        ed_len_imp->setGeometry(QRect(144, 5, 31, 22));
        ed_len_imp->setFont(font1);
        ed_len_imp->setAlignment(Qt::AlignCenter);
        label_leni = new QLabel(frame_generator);
        label_leni->setObjectName(QStringLiteral("label_leni"));
        label_leni->setGeometry(QRect(107, 5, 31, 21));
        label_leni->setFont(font1);
        ed_ngen = new ParamLineEditSel(frame_generator);
        ed_ngen->setObjectName(QStringLiteral("ed_ngen"));
        ed_ngen->setGeometry(QRect(52, 5, 22, 22));
        ed_ngen->setFont(font1);
        ed_ngen->setAlignment(Qt::AlignCenter);
        comboBox_val_pow = new ComboBoxSel(frame_generator);
        comboBox_val_pow->setObjectName(QStringLiteral("comboBox_val_pow"));
        comboBox_val_pow->setGeometry(QRect(102, 32, 85, 22));
        comboBox_val_pow->setFont(font1);

        verticalLayout_3->addWidget(frame_generator);

        frame_object = new QFrame(groupBox_defectoscop);
        frame_object->setObjectName(QStringLiteral("frame_object"));
        frame_object->setMinimumSize(QSize(192, 85));
        frame_object->setMaximumSize(QSize(192, 85));
        frame_object->setFrameShape(QFrame::Box);
        frame_object->setFrameShadow(QFrame::Sunken);
        label_n_rez = new QLabel(frame_object);
        label_n_rez->setObjectName(QStringLiteral("label_n_rez"));
        label_n_rez->setGeometry(QRect(93, 63, 41, 20));
        label_n_rez->setFont(font1);
        ed_n_rez = new ParamLineEditSel(frame_object);
        ed_n_rez->setObjectName(QStringLiteral("ed_n_rez"));
        ed_n_rez->setGeometry(QRect(147, 60, 21, 22));
        ed_n_rez->setFont(font1);
        ed_n_rez->setAlignment(Qt::AlignCenter);
        label_t_pr = new QLabel(frame_object);
        label_t_pr->setObjectName(QStringLiteral("label_t_pr"));
        label_t_pr->setGeometry(QRect(2, 13, 31, 16));
        label_t_pr->setFont(font1);
        ed_t_pr = new ParamLineEditSel(frame_object);
        ed_t_pr->setObjectName(QStringLiteral("ed_t_pr"));
        ed_t_pr->setGeometry(QRect(35, 8, 46, 22));
        ed_t_pr->setFont(font1);
        ed_t_pr->setAlignment(Qt::AlignCenter);
        ed_thick_corr = new ParamLineEditSel(frame_object);
        ed_thick_corr->setObjectName(QStringLiteral("ed_thick_corr"));
        ed_thick_corr->setGeometry(QRect(35, 60, 46, 22));
        ed_thick_corr->setFont(font1);
        ed_thick_corr->setAlignment(Qt::AlignCenter);
        label_queue_len = new QLabel(frame_object);
        label_queue_len->setObjectName(QStringLiteral("label_queue_len"));
        label_queue_len->setGeometry(QRect(96, 36, 90, 16));
        label_queue_len->setFont(font1);
        ed_thick_dt = new ParamLineEditSel(frame_object);
        ed_thick_dt->setObjectName(QStringLiteral("ed_thick_dt"));
        ed_thick_dt->setGeometry(QRect(136, 60, 46, 22));
        ed_thick_dt->setFont(font1);
        ed_thick_dt->setAlignment(Qt::AlignCenter);
        label_cc = new QLabel(frame_object);
        label_cc->setObjectName(QStringLiteral("label_cc"));
        label_cc->setGeometry(QRect(104, 11, 31, 16));
        label_cc->setFont(font1);
        label_alpha = new QLabel(frame_object);
        label_alpha->setObjectName(QStringLiteral("label_alpha"));
        label_alpha->setGeometry(QRect(18, 37, 16, 16));
        label_alpha->setFont(font1);
        ed_cc = new ParamLineEditSel(frame_object);
        ed_cc->setObjectName(QStringLiteral("ed_cc"));
        ed_cc->setGeometry(QRect(136, 8, 46, 22));
        ed_cc->setFont(font1);
        ed_cc->setAlignment(Qt::AlignCenter);
        label_thick_dt = new QLabel(frame_object);
        label_thick_dt->setObjectName(QStringLiteral("label_thick_dt"));
        label_thick_dt->setGeometry(QRect(93, 63, 41, 16));
        label_thick_dt->setFont(font1);
        ed_alpha = new ParamLineEditSel(frame_object);
        ed_alpha->setObjectName(QStringLiteral("ed_alpha"));
        ed_alpha->setGeometry(QRect(35, 34, 46, 22));
        ed_alpha->setFont(font1);
        ed_alpha->setAlignment(Qt::AlignCenter);
        label_thick_corr = new QLabel(frame_object);
        label_thick_corr->setObjectName(QStringLiteral("label_thick_corr"));
        label_thick_corr->setGeometry(QRect(2, 62, 31, 16));
        label_thick_corr->setFont(font1);

        verticalLayout_3->addWidget(frame_object);

        frame_scale = new QFrame(groupBox_defectoscop);
        frame_scale->setObjectName(QStringLiteral("frame_scale"));
        frame_scale->setMinimumSize(QSize(192, 49));
        frame_scale->setMaximumSize(QSize(192, 49));
        frame_scale->setFrameShape(QFrame::Box);
        frame_scale->setFrameShadow(QFrame::Sunken);
        label_coord_descr = new QLabel(frame_scale);
        label_coord_descr->setObjectName(QStringLiteral("label_coord_descr"));
        label_coord_descr->setGeometry(QRect(6, 21, 131, 21));
        label_coord_descr->setFont(font1);
        label_coord_descr->setFrameShape(QFrame::Box);
        label_coord_descr->setFrameShadow(QFrame::Sunken);
        label_coord_units_descr = new QLabel(frame_scale);
        label_coord_units_descr->setObjectName(QStringLiteral("label_coord_units_descr"));
        label_coord_units_descr->setGeometry(QRect(113, 20, 21, 21));
        label_coord_units_descr->setFont(font1);
        button_reset_coord = new PushButtonSel(frame_scale);
        button_reset_coord->setObjectName(QStringLiteral("button_reset_coord"));
        button_reset_coord->setGeometry(QRect(139, 19, 51, 26));
        button_reset_coord->setFont(font4);
        button_reset_coord->setAutoDefault(true);
        checkBox_scale_mode = new CheckBoxSel(frame_scale);
        checkBox_scale_mode->setObjectName(QStringLiteral("checkBox_scale_mode"));
        checkBox_scale_mode->setGeometry(QRect(8, -2, 185, 21));
        checkBox_scale_mode->setFont(font1);
        checkBox_scale_mode->setLayoutDirection(Qt::LeftToRight);
        label_coord = new QLabel(frame_scale);
        label_coord->setObjectName(QStringLiteral("label_coord"));
        label_coord->setGeometry(QRect(60, 20, 51, 21));
        label_coord->setFont(font1);
        label_coord->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(frame_scale);

        frame_manual_speed = new QFrame(groupBox_defectoscop);
        frame_manual_speed->setObjectName(QStringLiteral("frame_manual_speed"));
        frame_manual_speed->setFrameShape(QFrame::StyledPanel);
        frame_manual_speed->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_manual_speed);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_manual_speed = new ManualSpeedLbl(frame_manual_speed);
        label_manual_speed->setObjectName(QStringLiteral("label_manual_speed"));
        label_manual_speed->setFrameShape(QFrame::Box);

        verticalLayout_4->addWidget(label_manual_speed);


        verticalLayout_3->addWidget(frame_manual_speed);

        frame_debug = new QFrame(groupBox_defectoscop);
        frame_debug->setObjectName(QStringLiteral("frame_debug"));
        frame_debug->setMinimumSize(QSize(192, 65));
        frame_debug->setMaximumSize(QSize(192, 65));
        frame_debug->setFrameShape(QFrame::StyledPanel);
        frame_debug->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_debug);
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 2, -1, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_start_stop = new QPushButton(frame_debug);
        pushButton_start_stop->setObjectName(QStringLiteral("pushButton_start_stop"));
        pushButton_start_stop->setFont(font1);
        pushButton_start_stop->setCheckable(true);
        pushButton_start_stop->setAutoDefault(true);
        pushButton_start_stop->setDefault(false);

        horizontalLayout->addWidget(pushButton_start_stop);

        pushButt_debug = new QPushButton(frame_debug);
        pushButt_debug->setObjectName(QStringLiteral("pushButt_debug"));
        pushButt_debug->setFont(font4);

        horizontalLayout->addWidget(pushButt_debug);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_save_osc = new QPushButton(frame_debug);
        pushButton_save_osc->setObjectName(QStringLiteral("pushButton_save_osc"));
        pushButton_save_osc->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_save_osc);

        ed_osc_capt = new QLineEdit(frame_debug);
        ed_osc_capt->setObjectName(QStringLiteral("ed_osc_capt"));
        ed_osc_capt->setFont(font1);

        horizontalLayout_2->addWidget(ed_osc_capt);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(frame_debug);

        frame_b_scan->raise();
        wid_menu_Layout->raise();
        frame_generator->raise();
        frame_strob->raise();
        frame_vrch->raise();
        frame_osc->raise();
        frame_common->raise();
        frame_object->raise();
        frame_scale->raise();
        frame_vrch_on_of->raise();
        frame_debug->raise();
        frame_manual_speed->raise();

        verticalLayout->addWidget(groupBox_defectoscop);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ParamEditToolBar);

        QMetaObject::connectSlotsByName(ParamEditToolBar);
    } // setupUi

    void retranslateUi(QWidget *ParamEditToolBar)
    {
        ParamEditToolBar->setWindowTitle(QApplication::translate("ParamEditToolBar", "ParamEditToolBox", 0));
        groupBox_defectoscop->setTitle(QApplication::translate("ParamEditToolBar", "\320\224\320\265\321\204\320\265\320\272\321\202\320\276\321\201\320\272\320\276\320\277", 0));
        button_object_par->setText(QApplication::translate("ParamEditToolBar", "\320\236\320\261\321\212\320\265\320\272\321\202 \320\272\320\276\320\275\321\202\321\200.", 0));
        label_a_des->setText(QApplication::translate("ParamEditToolBar", "\320\220\320\232", 0));
        checkBox_chan_us_set->setText(QApplication::translate("ParamEditToolBar", "\320\227\320\260\320\264\320\260\320\275\320\270\320\265 \321\203\321\201\320\270\320\273", 0));
        label_chan->setText(QApplication::translate("ParamEditToolBar", "\320\232\320\260\320\275\320\260\320\273", 0));
        label_a_des_2->setText(QApplication::translate("ParamEditToolBar", "\320\224\320\265\321\204\320\265\320\272\321\202", 0));
        checkBox_param_pos_edit->setText(QApplication::translate("ParamEditToolBar", "\320\230\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", 0));
        label_tv->setText(QApplication::translate("ParamEditToolBar", "TV", 0));
        label_tt->setText(QApplication::translate("ParamEditToolBar", "TT", 0));
        comboBox_rej->clear();
        comboBox_rej->insertItems(0, QStringList()
         << QApplication::translate("ParamEditToolBar", "\321\200\320\260\320\264\320\270\320\276", 0)
         << QApplication::translate("ParamEditToolBar", "\320\262\320\270\320\264\320\265\320\276", 0)
        );
        label_kus->setText(QApplication::translate("ParamEditToolBar", "K \321\203\321\201", 0));
        label_begr->setText(QApplication::translate("ParamEditToolBar", "\320\235\320\260\321\207", 0));
        label_endr->setText(QApplication::translate("ParamEditToolBar", "\320\232\320\276\320\275", 0));
        label_ninp_us->setText(QApplication::translate("ParamEditToolBar", "\320\243\321\201\320\270\320\273", 0));
        label_tact->setText(QApplication::translate("ParamEditToolBar", "\320\242\320\260\320\272\321\202", 0));
        label_us_rej->setText(QApplication::translate("ParamEditToolBar", "\320\224\320\225\320\244", 0));
        comboBox_detpar->clear();
        comboBox_detpar->insertItems(0, QStringList()
         << QApplication::translate("ParamEditToolBar", "\320\224\320\265\321\202 \302\261", 0)
         << QApplication::translate("ParamEditToolBar", "\320\224\320\265\321\202 +", 0)
         << QApplication::translate("ParamEditToolBar", "\320\224\320\265\321\202 -", 0)
        );
        button_paste->setText(QApplication::translate("ParamEditToolBar", "\320\222\321\201\321\202\320\260\320\262\320\270\321\202\321\214", 0));
        button_copy->setText(QApplication::translate("ParamEditToolBar", "\320\232\320\276\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0));
        label_bstr->setText(QApplication::translate("ParamEditToolBar", "\320\235\320\260\321\207", 0));
        label_lstr->setText(QApplication::translate("ParamEditToolBar", "\320\224\320\273\320\270\321\202", 0));
        label_nstrb->setText(QApplication::translate("ParamEditToolBar", "\320\241\321\202\321\200\320\276\320\261", 0));
        label_por->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\263", 0));
        label_strb_type->setText(QApplication::translate("ParamEditToolBar", "\320\235\320\220\320\240", 0));
        checkBox_extra_strob_por->setText(QApplication::translate("ParamEditToolBar", "\320\234\320\275\320\276\320\263\320\276\321\203\321\200\320\276\320\262\320\275\320\265\320\262\321\213\320\271 \321\201\321\202\321\200\320\276\320\261", 0));
        label_por_1a->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\2632", 0));
        label_por_2->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\2633", 0));
        label_por_2a->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\2634", 0));
        label_por_3->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\2635", 0));
        label_b_begr->setText(QApplication::translate("ParamEditToolBar", "\320\235\320\260\321\207", 0));
        label_b_count->setText(QApplication::translate("ParamEditToolBar", "\320\224\320\273\320\270\321\202", 0));
        label_b_stepr->setText(QApplication::translate("ParamEditToolBar", "\320\250\320\260\320\263", 0));
        label_b_por->setText(QApplication::translate("ParamEditToolBar", "\320\237\320\276\321\200\320\276\320\263", 0));
        checkBox_vrch->setText(QApplication::translate("ParamEditToolBar", "\320\222\321\200\321\207 \320\262\320\272\320\273.", 0));
        label_vtime->setText(QApplication::translate("ParamEditToolBar", "\320\222\321\200\320\265\320\274\321\217", 0));
        label_kusv->setText(QApplication::translate("ParamEditToolBar", "\320\232 \321\203\321\201", 0));
        label_npointv->setText(QApplication::translate("ParamEditToolBar", "\320\235\320\276\320\274\320\265\321\200 \321\202.", 0));
        label_numpointv->setText(QApplication::translate("ParamEditToolBar", "/", 0));
        comboBox_npow->clear();
        comboBox_npow->insertItems(0, QStringList()
         << QApplication::translate("ParamEditToolBar", "\320\223\320\265\320\275    1-4", 0)
         << QApplication::translate("ParamEditToolBar", "\320\223\320\265\320\275    5-8", 0)
        );
        label_ngen->setText(QApplication::translate("ParamEditToolBar", "\320\223\320\265\320\275", 0));
        label_leni->setText(QApplication::translate("ParamEditToolBar", "\320\230\320\274\320\277", 0));
        comboBox_val_pow->clear();
        comboBox_val_pow->insertItems(0, QStringList()
         << QApplication::translate("ParamEditToolBar", "50V", 0)
         << QApplication::translate("ParamEditToolBar", "100 V", 0)
         << QApplication::translate("ParamEditToolBar", "150 V", 0)
         << QApplication::translate("ParamEditToolBar", "200 V", 0)
        );
        comboBox_val_pow->setCurrentText(QApplication::translate("ParamEditToolBar", "50V", 0));
        label_n_rez->setText(QApplication::translate("ParamEditToolBar", "\342\204\226 \321\200\320\265\320\267", 0));
        label_t_pr->setText(QApplication::translate("ParamEditToolBar", "T \320\277\321\200", 0));
        label_queue_len->setText(QApplication::translate("ParamEditToolBar", "\320\236\321\207\320\265\321\200\320\265\320\264\321\214 > 200", 0));
        label_cc->setText(QApplication::translate("ParamEditToolBar", "\320\241 \320\267\320\262", 0));
        label_alpha->setText(QApplication::translate("ParamEditToolBar", "\316\261", 0));
        label_thick_dt->setText(QApplication::translate("ParamEditToolBar", "\320\243\321\210\320\270\321\200", 0));
        label_thick_corr->setText(QApplication::translate("ParamEditToolBar", "\320\232\320\276\321\200\321\200", 0));
        label_coord_descr->setText(QApplication::translate("ParamEditToolBar", "\320\232\320\276\320\276\321\200\320\264.", 0));
        label_coord_units_descr->setText(QApplication::translate("ParamEditToolBar", "\320\274\320\274", 0));
        button_reset_coord->setText(QApplication::translate("ParamEditToolBar", "\320\241\320\261\321\200\320\276\321\201", 0));
        checkBox_scale_mode->setText(QApplication::translate("ParamEditToolBar", "\320\234\320\260\321\201\321\210\321\202\320\260\320\261\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\272 \320\264\320\276\320\277\321\203\321\201\320\272\321\203", 0));
        label_coord->setText(QApplication::translate("ParamEditToolBar", "0", 0));
        label_manual_speed->setText(QApplication::translate("ParamEditToolBar", "V __ \320\274\320\274/\321\201", 0));
        pushButton_start_stop->setText(QApplication::translate("ParamEditToolBar", "Test", 0));
        pushButt_debug->setText(QApplication::translate("ParamEditToolBar", "Debug", 0));
        pushButton_save_osc->setText(QApplication::translate("ParamEditToolBar", "\320\236\321\201\321\206\320\270\320\273.", 0));
    } // retranslateUi

};

namespace Ui {
    class ParamEditToolBar: public Ui_ParamEditToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMEDITTOOLBAR_H
