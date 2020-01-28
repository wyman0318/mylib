/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBoxUART;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout_5;
    QLabel *Label_13;
    QComboBox *ComboBoxCOM_5;
    QLabel *Label_14;
    QComboBox *ComboBoxBaud_5;
    QLabel *Label_15;
    QComboBox *ComboBoxDataBits_5;
    QPushButton *pushButtonUART_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *labelCardNumber;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(160, 480);
        verticalLayout_6 = new QVBoxLayout(Widget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBoxUART = new QGroupBox(Widget);
        groupBoxUART->setObjectName(QStringLiteral("groupBoxUART"));
        verticalLayout_5 = new QVBoxLayout(groupBoxUART);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        formLayout_5 = new QFormLayout();
        formLayout_5->setSpacing(6);
        formLayout_5->setObjectName(QStringLiteral("formLayout_5"));
        Label_13 = new QLabel(groupBoxUART);
        Label_13->setObjectName(QStringLiteral("Label_13"));
        Label_13->setMinimumSize(QSize(45, 0));
        Label_13->setAlignment(Qt::AlignCenter);

        formLayout_5->setWidget(0, QFormLayout::LabelRole, Label_13);

        ComboBoxCOM_5 = new QComboBox(groupBoxUART);
        ComboBoxCOM_5->setObjectName(QStringLiteral("ComboBoxCOM_5"));

        formLayout_5->setWidget(0, QFormLayout::FieldRole, ComboBoxCOM_5);

        Label_14 = new QLabel(groupBoxUART);
        Label_14->setObjectName(QStringLiteral("Label_14"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, Label_14);

        ComboBoxBaud_5 = new QComboBox(groupBoxUART);
        ComboBoxBaud_5->setObjectName(QStringLiteral("ComboBoxBaud_5"));

        formLayout_5->setWidget(1, QFormLayout::FieldRole, ComboBoxBaud_5);

        Label_15 = new QLabel(groupBoxUART);
        Label_15->setObjectName(QStringLiteral("Label_15"));

        formLayout_5->setWidget(2, QFormLayout::LabelRole, Label_15);

        ComboBoxDataBits_5 = new QComboBox(groupBoxUART);
        ComboBoxDataBits_5->setObjectName(QStringLiteral("ComboBoxDataBits_5"));

        formLayout_5->setWidget(2, QFormLayout::FieldRole, ComboBoxDataBits_5);


        verticalLayout_5->addLayout(formLayout_5);

        pushButtonUART_5 = new QPushButton(groupBoxUART);
        pushButtonUART_5->setObjectName(QStringLiteral("pushButtonUART_5"));

        verticalLayout_5->addWidget(pushButtonUART_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetFixedSize);
        label_5 = new QLabel(groupBoxUART);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        horizontalSpacer_5 = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_5->addLayout(horizontalLayout_6);


        verticalLayout_6->addWidget(groupBoxUART);

        labelCardNumber = new QLabel(Widget);
        labelCardNumber->setObjectName(QStringLiteral("labelCardNumber"));
        labelCardNumber->setMinimumSize(QSize(140, 35));
        labelCardNumber->setMaximumSize(QSize(16777215, 30));
        labelCardNumber->setFrameShape(QFrame::Panel);
        labelCardNumber->setFrameShadow(QFrame::Sunken);
        labelCardNumber->setLineWidth(2);

        verticalLayout_6->addWidget(labelCardNumber);

        verticalSpacer = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_6->addItem(verticalSpacer);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        groupBoxUART->setTitle(QApplication::translate("Widget", "\344\270\262\345\217\243", 0));
        Label_13->setText(QApplication::translate("Widget", "\347\253\257\345\217\243", 0));
        ComboBoxCOM_5->clear();
        ComboBoxCOM_5->insertItems(0, QStringList()
         << QApplication::translate("Widget", "ttySAC1", 0)
         << QApplication::translate("Widget", "ttySAC2", 0)
         << QApplication::translate("Widget", "ttySAC3", 0)
        );
        Label_14->setText(QApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207", 0));
        ComboBoxBaud_5->clear();
        ComboBoxBaud_5->insertItems(0, QStringList()
         << QApplication::translate("Widget", "9600", 0)
         << QApplication::translate("Widget", "115200", 0)
        );
        Label_15->setText(QApplication::translate("Widget", "\346\225\260\346\215\256\344\275\215", 0));
        ComboBoxDataBits_5->clear();
        ComboBoxDataBits_5->insertItems(0, QStringList()
         << QApplication::translate("Widget", "8", 0)
         << QApplication::translate("Widget", "7", 0)
         << QApplication::translate("Widget", "6", 0)
         << QApplication::translate("Widget", "5", 0)
        );
        pushButtonUART_5->setText(QApplication::translate("Widget", "\345\274\200\345\247\213\345\210\267\345\215\241", 0));
        label_5->setText(QApplication::translate("Widget", "\345\215\241\345\217\267", 0));
        labelCardNumber->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
