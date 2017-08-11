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


QJsonObject finallist;
QJsonObject subset;

bool Intersection(QJsonArray A,QJsonArray B){
    for(int i=0;i<A.size();i++)
        for(int j=1;j<B.size();j++)
            if(A[i] == B[j]) return false;
    return true;
}

//combination C n 取 m
//now  => current number
//start => where begin
QString set[1001];
void combination(int now,int start,int n,int m,QJsonObject A){
    int finalbid = 0;
    QString temp ="";
    if(now == m+1){
        bool flag = true;
        QJsonArray finalitem;
        for(int i = 1; i <= m; i++){
            QJsonArray data=subset[QString::number(i)].toArray();
            if(Intersection(finalitem,data)){
                for(int q=1;q<data.size();q++){
                    finalitem.append(data[q]);
                }
                //qDebug()<<data[0].toInt();
                finalbid = finalbid + data[0].toInt();
            }
            else flag = false;
            temp = temp +"P" +set[i]+" ";
        }
        finalitem.append(finalbid);
        if(flag)
            finallist.insert(temp,finalitem);
        return ;
   }
    else
        for(int j = start; j<= n; j++){
            QJsonArray data_A=A["P"+QString::number(j)].toArray();
            subset.insert(QString::number(now),data_A);
            set[now]=QString::number(j);
            combination(now+1,j+1,n,m,A);
   }
}
 QVector<QPair<int,QString> > payment;
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
    for(int i=0;i<number;i++){
        QJsonArray biditem;
        QString bid = bidTable->item(0,i)->text();
        QString bundle = bidTable->item(1,i)->text();
        biditem.append(bid.toInt());
        for(int q=0;q<=bundle.count()/2;q++){
            QString temp = bundle.split(",").at(q);
            biditem.append(temp);
        }
        bidlist.insert("P"+QString::number(i+1),biditem);
    }
//    QJsonDocument test(bidlist);
//    QString strJson(test.toJson(QJsonDocument::Compact));
//    qDebug()<<strJson.toStdString().data();

    for(int i=1;i<bidlist.size();i++){
        combination(1,1,bidlist.size(),i,bidlist);
    }

    QJsonDocument doc(finallist);
    QString final(doc.toJson(QJsonDocument::Compact));
    qDebug()<<final.toStdString().data();

    QJsonObject::iterator it;
    QJsonArray final_data;
    QString winner;
    final_data.append(0);
    for (it = finallist.begin(); it != finallist.end(); it++) {
        QJsonArray current_data = it.value().toArray();
        if(current_data.last().toInt()>final_data.last().toInt()){
            final_data = current_data;
            winner = it.key();
        }
    }
    qDebug()<<final_data;
    qDebug()<<winner;

    QJsonArray second_data;
    QString second;
    second_data.append(0);
    for(int i=0;i<winner.count()/3;i++){
        QString temp = winner.split(" ").at(i);
        for (it = finallist.begin(); it != finallist.end(); it++) {
            bool flag = true;
            for(int j=0;j<it.key().count()/3;j++){
                QString foo = it.key().split(" ").at(j);
                if(temp == foo){
                    flag = false;
                    break;
                }
            }
            if(flag){
                QJsonArray current_data = it.value().toArray();
                if(current_data.last().toInt()>second_data.last().toInt()){
                    second_data = current_data;
                    second = it.key();
                }
            }
        }
        QJsonArray winner_bid=bidlist[temp].toArray();
        int welfare = final_data.last().toInt() - winner_bid.first().toInt();
        welfare = second_data.last().toInt() - welfare;
        payment.push_back(qMakePair(welfare,temp));
    }

    for(int i=0;i<payment.size();i++){
        qDebug()<<payment[i].first;
        qDebug()<<payment[i].second;
    }
    return true;
}


ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{

    setTitle("Conclusion");

    label = new QLabel("VCG Payment analysis is done ! ");
    label->setFont(QFont("Courier New", 15, QFont::Bold));
    label->setWordWrap(true);
    label2 = new QLabel();
    label2->setFont(QFont("Courier New", 15));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(label2);
    setLayout(layout);

}

void ConclusionPage::initializePage(){

    QString print = "";
    for(int i=0;i<payment.size();i++){
        print = print + payment[i].second + " should pay " +QString::number(payment[i].first)+" dollars\n";
    }
    label2->setText(print);

}
