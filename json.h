#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <numberlist.h>
#include <numberlist.cpp>
#ifndef JSON_H
#define JSON_H
template<typename T>
class GJson{
private:
    bool ReadFile(QFile*);
    QString GetChildren();
    QString GetObject(QString);
    QString GetObjectArray(QString);
    bool JudgeObject(QString, T&);
    bool JudgeString(QString, T&);
    bool JudgeBool(QString, T&);
    bool JudgeNum(QString, T&);
    bool JudgeNull(QString, T&);
    bool JudgeArray(QString, T&);
    enum SpaceSign{
        ONLY_OBJECT,
        OBJECT_ARRAY,
        NORMAL_ARRAY
    };
    std::vector<SpaceSign> space_sign_array;

public:
    GJson(QFile*);
    GJson(QString);
    ~GJson();
    QString* key_;
    T value_;
    bool is_value_array_ = false;
    //GJson* children_;
    bool is_file_exit_;
    // QString json_str_;
    //qint64 value_size_;
};

#endif // JSON_H
