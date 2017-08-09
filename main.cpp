#include "widget.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(classwizard);

    QApplication app(argc, argv);

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    ClassWizard wizard;
    wizard.show();
    return app.exec();
}
