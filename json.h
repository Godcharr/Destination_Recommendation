#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include "numberlist.h"
#include <QRegularExpression>
#ifndef JSON_H
#define JSON_H
class GJson{
private:
    bool ReadFile(QFile*, QString*&);
    QString GetChildren();
    QString GetObject(QString);
    QString GetObjectArray(QString);
    bool JudgeObject(QString);
    bool JudgeString(QString);
    bool JudgeBool(QString);
    bool JudgeNum(QString);
    bool JudgeNull(QString);
    template <typename T> bool JudgeArray(QString);
    template<typename... Args> bool JudgeArrayArray(Args&&... args);
    template <typename T> void* SetArray(QString, int&);
    template<> void* SetArray<GJson*>(QString, int&);
    template<> void* SetArray<QStringList*>(QString, int&);
    template<> void* SetArray<bool*>(QString, int&);
    template<> void* SetArray<NumberList*>(QString, int&);
    template<> void* SetArray<int*>(QString, int&);
    template<> void* SetArray<int**>(QString, int&);
    bool JudgeGetArray(QString);
    QString GetKey(GJson*);
    enum DataType{
        GJSON,
        QSTRING,
        BOOL,
        INT,
        DOUBLE,
        GNULL,
        ARRAY
    };
    std::vector<void*> GetAllPointer(std::vector<void*>*);
    /*
     * pair数组存储申请空间的数量
     * 例：{{1，0}，{1，1}，{1，4}}
       ***********************************************************
     * bool: true表示是数组 需要delete[]; false表示不是数组，需要delet。
     * int: 表示需要delete的深度。
     * ***********************************************************
     * 对象：{false, 1}
     * null： {false, 0}
     * 数组：{true, depth}
     * {null, null, ...}：{true, 0}.
      */
    std::vector<std::pair<bool, int>> array_size_;
    QString json_str_;
public:
    GJson(QFile*);
    GJson(QString);
    ~GJson();
    std::vector<QString> key_;
    std::vector<std::pair<void*, DataType>> value_;
    std::vector<std::pair<void*, DataType>> value_for_delete_;
    std::vector<std::vector<int>> array_width_;
    //GJson* children_;
    bool is_file_exit_;
    // QString json_str_;
    //qint64 value_size_;
};

#endif // JSON_H
