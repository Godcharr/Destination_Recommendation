#include "json.h"
template<typename T>
GJson<T>::GJson(QFile* file){
    this->is_file_exit_ = this->ReadFile(file);
    if(!is_file_exit_){
        qWarning() << "File is not exist!";
    }
}
template<typename T>
GJson<T>::GJson(QString str){
    typedef bool (GJson::*JudgeType) (char);
    JudgeType judge_type[] = {&this->JudgeObject,
                              &this->JudgeString,
                              &this->JudgeBool,
                              &this->JudgeNum,
                              &this->JudgeNull,
                              &this->JudgeArray};
    T val;
    for(int i = 0; i < 6; i++){
        this->*judge_type[i](str, &val);
    }
    this->value_ = str;
}
template<typename T>
GJson<T>::~GJson(){

}
template<typename T>
bool GJson<T>::ReadFile(QFile* file){
    QTextStream in(file);
    // 设置字符编码为UTF-8
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    // 逐行读取
    while (!in.atEnd()) {
        this->json_str_ += in.readLine();
    }
    if(this->json_str_[0] == '{'){
        this->json_str_.remove(0,1);
        this->json_str_.chop(1);
    }
    qDebug() << "File content: " << this->json_str_;
    return true;
}
template<typename T>
// 判断value是否为GJson对象，判断完成后修改传入的value值。
bool GJson<T>::JudgeObject(QString json_str, T& val)
{
    if(json_str[0] == '{'){
        QString tmp_str = json_str;
        tmp_str.remove(0, 1);
        tmp_str.chop(1);
        val = new GJson(tmp_str);       // 使用完要释放
        this->space_sign_array.push_back(ONLY_OBJECT);
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
// 判断value是否为字符串类型，判断完成后修改传入的value值。
bool GJson<T>::JudgeString(QString json_str, T& val)
{
    if(json_str[0] == '\"'){
        QStringList tmp_list = json_str.split("\"");
        QString tmp_str = tmp_list[1];
        val = tmp_str;
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
// 判断value是否为bool类型，判断完成后修改传入的value值。
bool GJson<T>::JudgeBool(QString json_str, T& val)
{
    if(json_str.left(4) == "true"){
        val = true;
        return true;
    }
    else if(json_str.left(5) == "false"){
        val = false;
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
// 判断value是否为数字，判断完成后修改传入的value值。
bool GJson<T>::JudgeNum(QString json_str, T& val)
{
    //需要修改，参考numberlist.cpp中的以QStringList为参数的构造函数
    if((json_str[0] >= '0' && json_str[0] <= '9') || json_str[0] == "-"){
        int index = 0;
        QString tmp_str = "";
        bool symbol = true;
        if(json_str.startsWith('-')){
            index++;
            symbol = false;
        }
        while(json_str[index] >= '0' && json_str[index] <= '9'){
            tmp_str += json_str[index];
            index++;
        }
        if(json_str[index] == '.'){
            tmp_str += json_str[index];
            index++;
            while(json_str[index] >= '0' && json_str[index] <= '9'){
                tmp_str += json_str[index];
                index++;
            }
            double tmp_number = tmp_str.toDouble();
            if(!symbol)
                tmp_number = 0 - tmp_number;
            val = tmp_number;
            return true;
        }
        // 后续需要加上科学计数法的判断
        int tmp_number = tmp_str.toInt();
        if(!symbol)
            tmp_number = 0 - tmp_number;
        val = tmp_number;
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
// 判断value是否为空，判断完成后修改传入的value值。
bool GJson<T>::JudgeNull(QString json_str, T& val)
{
    if(json_str.left(4) == "null"){
        val = NULL;
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
bool GJson<T>::JudgeArray(QString json_str, T& val)
{
    this->is_value_array_ = true;
    QString tmp_str = json_str;
    if(tmp_str[0] == '['){
        tmp_str.remove(0,1);
        tmp_str.chop(1);
        // GJson对象数组
        if(tmp_str[0] == '{'){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            QStringList tmp_list = tmp_str.split("},{");
            val = new GJson*[tmp_list.length()];
            for(int i = 0; i < tmp_list.length(); i++){
                val[i] = new GJson(tmp_list[i]);
            }
            this->space_sign_array.push_back(OBJECT_ARRAY);
        }
        // QString数组
        if(tmp_str[0] == '\"'){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            val = new QStringList();
            val = tmp_str.split("\",\"");
            this->space_sign_array.push_back(NORMAL_ARRAY);
        }
        // bool类型数组
        if(tmp_str.left(4) == "true" || tmp_str.left(5) == "false"){
            QStringList tmp_list = tmp_str.split(",");
            val = new bool[tmp_list.length()];
            for(int i = 0; i < tmp_list.length(); i++){
                if(tmp_list[i] == "true")
                    val[i] = true;
                else
                    val[i] = false;
            }
            this->space_sign_array.push_back(NORMAL_ARRAY);
        }
        // 数字（整型或浮点型）数组
        if((tmp_str[0] >= '0' && tmp_str[0] <= '9') || tmp_str[0] == "-"){
            QStringList tmp_list = tmp_str.split(",");
            val = new NumberList(tmp_list);
            this->space_sign_array.push_back(NORMAL_ARRAY);
        }
        // 保存空值的数组
        if(tmp_str.left(4) == "null"){
            val = NULL;
        }
        //  保存数组的数组（高维数组）
        if(tmp_str[0] == '['){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            QStringList tmp_list = tmp_str.split("],[");
            val = new T[tmp_list.length()]; // 需要斟酌
            for(int i = 0; i < tmp_list.length(); i++){
                this->JudgeArray(tmp_list[i], val[i]);
            }
            this->space_sign_array.push_back(NORMAL_ARRAY);
        }

        return true;
    }
    else{
        return false;
    }
}
