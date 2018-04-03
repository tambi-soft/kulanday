
#include <QMainWindow>
#include <QApplication>
#include <QDebug>

#include <src/q_kulanday_main_window.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QApplication::setApplicationName("kulanday");
    
    QKulandayMainWindow mainWin;
    //QIcon::setThemeSearchPaths(QStringList("../assets/icons"));
    //QIcon::setThemeName("oxygen");
    //mainWin.setWindowIcon(QIcon("../assets/icons/logo2.png"));
    mainWin.show();
    return app.exec();
}
