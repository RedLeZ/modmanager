#include "qss/stylesheets.h"
#include "ui/modmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ui/framelesswrapper.h"
#include "config.hpp"

#include <ui/windowstitlebar.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/modmanager.png"));
    a.setOrganizationName("kaniol");
    a.setApplicationName("Mod Manager");
    a.setApplicationDisplayName(QObject::tr("Mod Manager"));
    a.setStyleSheet(styleSheetPath(Config().getCustomStyle()));

    //setup translator
    QTranslator translator;
    if(auto language = Config().getLanguage();
            !language.isEmpty() && translator.load(":/i18n/" + language)){
        a.installTranslator(&translator);
    } else if(language != "en_US"){
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
    }

    //setup theme
    if(!Config().getUseSystemIconTheme())
        QIcon::setThemeName("breeze-modmanager");

#ifdef Q_OS_WIN
    QFont font("微软雅黑", 9);
    a.setFont(font);
#endif
#ifdef Q_OS_MACOS
    QFont font("PingFang", 12);
    a.setFont(font);
#endif
    ModManager w;

#ifdef Q_OS_WIN
    if(Config().getUseFramelessWindow()){
        QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
        QApplication::setAttribute(Qt::AA_NativeWindows, false);
        auto frameless = new FramelessWrapper(&w);
        QObject::connect(&w, &ModManager::closed, frameless, &FramelessWrapper::close);
        frameless->show();
    } else
#endif
        w.show();
    return a.exec();
}
