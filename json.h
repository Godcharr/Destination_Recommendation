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
    template <typename T> void SetArray(QString, T);
    template<> void SetArray<GJson*>(QString, GJson*);
    template<> void SetArray<QStringList*>(QString, QStringList*);
    template<> void SetArray<bool*>(QString, bool*);
    template<> void SetArray<NumberList*>(QString, NumberList*);
    template<> void SetArray<int*>(QString, int*);
    template<> void SetArray<int**>(QString, int**);
    QString GetKey(GJson*);
    enum SpaceSign{
        ONLY_OBJECT,
        OBJECT_ARRAY,
        NORMAL_ARRAY,
        NORMAL_VARIABLE
    };
    std::vector<SpaceSign> space_sign_array;
    std::vector<std::vector<int>> array_size;
    QString json_str_;
public:
    GJson(QFile*);
    GJson(QString);
    ~GJson();
    std::vector<QString> key_;
    std::vector<void*> value_;
    bool is_value_array_ = false;
    //GJson* children_;
    bool is_file_exit_;
    // QString json_str_;
    //qint64 value_size_;
};

#endif // JSON_H
