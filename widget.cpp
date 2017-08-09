#include "widget.h"
#include "ui_widget.h"
#include <QTableWidget>
#include "QWizard"
#include <QDebug>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>
#include <QVector>
#include <QPair>

//Widget::Widget(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::Widget)
//{
//    ui->setupUi(this);
//}

//Widget::~Widget()
//{
//    delete ui;
//}

ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new RegistrationPage);
    addPage(new ConclusionPage);

//    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap("auction.jpg"));

    setWindowTitle(tr("Vickrey-Clarke-Groves Mechanism"));
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Introduction");

    QPixmap p("VCG_Mechanism.PNG");
    label2 = new QLabel();
//    label2->setGeometry(25,60,60,60);
    label2->setPixmap(p.scaled(500,500,Qt::KeepAspectRatio));

    label = new QLabel("This will show you how VCG Mechanism works !!!");
    label->setFont(QFont("Courier New", 12, QFont::Bold));
    label->setWordWrap(true);

    //Input players number
    inputNum = new QLabel("Please input number of players ?");
    inputNum->setFont(QFont("Courier New", 12, QFont::Bold));

    lineEdit1 = new QLineEdit();
    lineEdit1->setFocus();
    lineEdit1->setText("5");//get mouse focus
    lineEdit1->setValidator(new QIntValidator(2,100,lineEdit1)); //range from 2 to 100

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(inputNum);
    layout->addWidget(lineEdit1);
    layout->addWidget(label2);
    setLayout(layout);

    //connect(lineEdit1,textChanged(),this,)

    registerField("number",lineEdit1);
}
QVector<QString> usedset;
QVector<QPair<QString,int> > finallist;

RegistrationPage::RegistrationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("initialization");

    //Show input bid table
    bidTable = new QTableWidget;
    bidTable->setRowCount(2);     //設定QTableWidget列數
    bidTable->setColumnCount(5);  //設定QTableWidget行數

    explain = new QLabel("bid => money(number) \nbundle => (just like)A,B");
    explain->setFont(QFont("Courier New", 15, QFont::Bold));

//    QObject::connect(enterBtn,SIGNAL(clicked()),bidTable,SLOT(insertColumn(int));


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(bidTable);
    layout->addWidget(explain);

//    layout->addWidget(emailLabel, 1, 0);
//    layout->addWidget(emailLineEdit, 1, 1);
    setLayout(layout);

    registerField("bidTable",bidTable);

}

void RegistrationPage::initializePage(){
    int number = field("number").toInt();
    bidTable->setColumnCount(number);  //設定QTableWidget行數
    for(int i=1;i<=number;i++){
         HorizontalHeader<<"P"+QString::number(i);
    }
    VerticalHeader<<"bid"<<"bundle";
    bidTable->setHorizontalHeaderLabels(HorizontalHeader);
    bidTable->setVerticalHeaderLabels(VerticalHeader);
}
bool RegistrationPage::validatePage(){
    int number = field("number").toInt();
    QJsonObject bidlist;
    QJsonArray biditem;
    for(int i=0;i<number;i++){
        QString bid = bidTable->item(0,i)->text();
        QString bundle = bidTable->item(1,i)->text();
        biditem.append(bid);
        biditem.append(bundle);
        bidlist.insert("P"+QString::number(i),biditem);
        biditem.pop_back();
        biditem.pop_back();
    }
    QJsonDocument doc(bidlist);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    qDebug()<<strJson.toStdString().data();

    for(int i=0;i<bidlist.size();i++){
        QJsonArray data=bidlist["P"+QString::number(i)].toArray();
        for(int q=0;q<=data[1].toString().count()/2;q++){
            QString one = data[1].toString().split(",").at(q);
            usedset.push_back(one);
            //qDebug()<<one;
        }
        for(int j=0;j<<bidlist.size();j++)
            if(i!=j){

            }
    }
    for(int i=0;i<usedset.size();i++){
        qDebug()<<usedset[i];
    }


    return true;
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{

    setTitle("Conclusion");

    label = new QLabel("You are now successfully registered. Have a "
                               "nice day!");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);

}

void ConclusionPage::initializePage(){

//    for(int i=1;i<=number;i++){
//         HorizontalHeader<<"P"+QString::number(i);
//    }

}
