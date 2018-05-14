
#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QIcon>

#include <src/q_kulanday_main_window.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QApplication::setApplicationName("kulanday");
    
    QKulandayMainWindow mainWin;
    
#ifdef __linux__
#else
        QIcon::setThemeSearchPaths(QStringList("./oxygen"));
        QIcon::setThemeName("oxygen");
#endif
    
    qDebug() << QIcon::themeSearchPaths();
    
    mainWin.setWindowIcon(QIcon(":logo"));
    mainWin.show();
    return app.exec();
}
