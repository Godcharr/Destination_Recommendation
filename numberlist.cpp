#include "numberlist.h"

NumberList::NumberList(std::unordered_map<int, int> i_map, std::unordered_map<int, double>d_map)
{
    this->int_array = i_map;
    this->double_array = d_map;
}

NumberList::NumberList(QString str)
{
    for(int i = 0; i < str.length(); i++){
        if((str[i] <= '0' || str[i] >= '9') && str[i] != "-" && str[i] != ','){
            qWarning() << "The string is error. Please repeat!";
            return;
        }
    }
    QStringList str_list = str.split(',');
    bool symbol;
    for(int i = 0; i < str_list.length(); i++){
        if(str_list[i].startsWith('-')){
            str_list[i].remove(0, 1);
            symbol = false;
        }
        else{
            symbol = true;
        }
        // 后续需要加上科学计数法的判断
        if(str_list[i].contains(".")){
            double number = str_list[i].toDouble();
            if(!symbol)
                this->double_array.insert({i, 0 - number});
            else
                this->double_array.insert({i, number});
        }
        else{
            int number = str_list[i].toInt();
            if(!symbol)
                this->int_array.insert({i, 0 - number});
            else
                this->int_array.insert({i, number});
        }
    }
}

NumberList::NumberList(QStringList str_list)
{
    bool symbol;
    for(int i = 0; i < str_list.length(); i++){
        if(str_list[i].startsWith('-')){
            str_list[i].remove(0, 1);
            symbol = false;
        }
        else{
            symbol = true;
        }
        // 后续需要加上科学计数法的判断
        if(str_list[i].contains(".")){
            double number = str_list[i].toDouble();
            if(!symbol)
                this->double_array.insert({i, 0 - number});
            else
                this->double_array.insert({i, number});
        }
        else{
            int number = str_list[i].toInt();
            if(!symbol)
                this->int_array.insert({i, 0 - number});
            else
                this->int_array.insert({i, number});
        }
    }
}

int NumberList::Find(int index){
    auto int_it = this->int_array.find(index);
    if(int_it != int_array.end())
        return int_it->second;
    return -1;
}

//double NumberList::Find(int index){
//    auto double_it = this->double_array.find(index);
//    if(double_it != double_array.end())
//        return double_it->second;
//    return -2.22;
//}
bool NumberList::Insert(int i, int val){
    this->int_array.insert({i, val});
    return true;
}
bool NumberList::Insert(int i, double val){
    this->double_array.insert({i, val});
    return true;
}
bool NumberList::Erase(int)
{

}
//bool NumberList::Erase(T){

//}
NumberList::~NumberList(){

}
