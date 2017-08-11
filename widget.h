#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWizard>
#include <QLibraryInfo>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QStringList>


//namespace Ui {
//class Widget;
//}

//class Widget : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit Widget(QWidget *parent = 0);
//    ~Widget();

//private:
//    Ui::Widget *ui;


//};

class ClassWizard : public QWizard
{
    Q_OBJECT

public:
    ClassWizard(QWidget *parent = 0);

};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
    QLabel *label2;
    QLabel *inputNum;
    QLineEdit *lineEdit1;
};

class RegistrationPage : public QWizardPage
{
    Q_OBJECT

public:
    RegistrationPage(QWidget *parent = 0);

protected:
    void initializePage();
    virtual bool validatePage();

private:
    QTableWidget *bidTable;
    QStringList HorizontalHeader;
    QStringList VerticalHeader;
    QLabel *explain;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage();\

private:
    QLabel *label;
    QLabel *label2;
};


#endif // WIDGET_H
