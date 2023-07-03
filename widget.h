#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QtWebEngineWidgets>
#include <QWebEngineHistory>
#include <QWebEngineHistoryItem>
#include <QWebEnginePage>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    QTabWidget m_tabWidget;
    //void Widget::loadStyleSheet(const QString &styleSheetFile);

    // 定义槽函数
protected slots:
    void onTabCurrentChanged(int index);
    void onTabCloseRequested(int index);
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
