#include "widget.h"
#include "json.h"
#include <QApplication>
#include "CJsonObject.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    QString file_path = QCoreApplication::applicationDirPath() + "./static/";
//    QString file_hhh = file_path + "title.json";
//    QFile file(file_path);
//    if (!file.open(QIODevice::ReadOnly)) {
//        return false;
//    }
//    QString str = "{\"a\":1}";
    //GJson<GJson<int>> json("{\"a\":1}");
    //file.close();
    int iValue;
    double fTimeout;
    std::string strValue;
    neb::CJsonObject oJson("");
    std::cout << oJson.ToString() << std::endl;

    return a.exec();
}
