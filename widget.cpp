#include "widget.h"
#include "ui_Widget.h"
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QtNetwork>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //在你的Widget类的构造函数中
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);//新建布局
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));//为布局设置对象名
    this->setLayout(horizontalLayout);//在此Widget中添加该布局
    horizontalLayout->addWidget(&m_tabWidget);//往布局里添加任何你需要的子控件，这里是添加了一个新的Widget作为子控件，_you_widget是你自己的窗体变量，请根据实际需要自己添加任何控件
    // this->loadStyleSheet("./tabstyle.qss");

    //QTabWidget的基本设置
    m_tabWidget.setParent(this);
    m_tabWidget.move(10, 10);
    m_tabWidget.resize(200, 200);
    m_tabWidget.setTabPosition(QTabWidget::North);
    m_tabWidget.setTabShape(QTabWidget::Rounded);
    m_tabWidget.setTabsClosable(false);
    m_tabWidget.setMovable(true);


    QString htmlPath = QCoreApplication::applicationDirPath() + "./static/";
    QString htmlFile = htmlPath + "index.html";
    qDebug() << htmlFile;
    QFile file(htmlFile);
    if(!file.exists())
        qDebug() << "html file is not exist";
    QWebEngineView *view = new QWebEngineView;

    QWebChannel *webChannel = new QWebChannel(view);
    m_tabWidget.addTab(view, "Show On Map");
    view->page()->setWebChannel(webChannel);
    webChannel->registerObject(QString("JSInterface"), view);
    view->page()->load(QUrl("file:///" + htmlFile));


    QWidget* widget = new QWidget(&m_tabWidget);
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* lbl = new QLabel(widget);
    QPushButton* btn = new QPushButton(widget);

    lbl->setText("页面2");
    lbl->setAlignment(Qt::AlignCenter);

    btn->setText("页面2");

    layout->addWidget(lbl);
    layout->addWidget(btn);

    widget->setLayout(layout);

    m_tabWidget.addTab(widget, "Show On Chart");

    m_tabWidget.setCurrentIndex(1);

    connect(&m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
    connect(&m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
}

void Widget::onTabCurrentChanged(int index)
{
    qDebug() << "Page change to: " << index;
}

void Widget::onTabCloseRequested(int index)
{
    m_tabWidget.removeTab(index);
}
Widget::~Widget()
{
    delete ui;
}




