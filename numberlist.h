#ifndef NumberListTYPE_H
#define NumberListTYPE_H
#include <QString>
#include <unordered_map>
#include <QDebug>
class NumberList
{
public:
    NumberList(QString);
    NumberList(std::unordered_map<int, int>, std::unordered_map<int, double>);
    NumberList(QStringList);
    ~NumberList();
    int Find(int);
    double Find(double);
private:
    enum Type{
        INT,
        DOUBLE
    };
    std::unordered_map<int, int> int_array;
    std::unordered_map<int, double> double_array;
    void SetData();
    bool Insert(int, int);
    bool Insert(int, double);
    bool Erase(int);
    //bool Erase(T);
};

#endif // NumberListTYPE_H
