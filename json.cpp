#include "json.h"
GJson::GJson(QFile* file){
    QString* str;
    this->is_file_exit_ = this->ReadFile(file, str);
    if(!is_file_exit_){
        qWarning() << "File is not exist!";
    }
    typedef bool (GJson::*JudgeType) (char);
    JudgeType judge_type[] = {this->JudgeObject,
                              &this->JudgeString,
                              &this->JudgeBool,
                              &this->JudgeNum,
                              &this->JudgeNull,
                              &this->JudgeArray};
    T val;
    for(int i = 0; i < 6; i++){
        this->*judge_type[i](str, &val, &array_size);
    }
    this->value_ = val;
}
GJson::GJson(QString str){
    typedef bool (GJson::*JudgeType) (char);
    JudgeType judge_type[] = {&this->JudgeObject,
                              &this->JudgeString,
                              &this->JudgeBool,
                              &this->JudgeNum,
                              &this->JudgeNull,
                              &this->JudgeArray};
    T val;
    for(int i = 0; i < 6; i++){
        this->*judge_type[i](str, &val, array_size);
    }
    this->value_ = val;
}
GJson::~GJson(){
    for(int i = 0; i < space_sign_array.size(); i++){
        if(space_sign_array[i] == ONLY_OBJECT){
            delete this->value_;
        }
        if(space_sign_array[i] == OBJECT_ARRAY){
            for(int j = 0; j < array_size; j++){
                delete this->value_[j];
            }
        }
        if(space_sign_array[i] == NORMAL_ARRAY){
            delete[] this->value_;
        }
        if(space_sign_array[i] == NORMAL_VARIABLE){
            delete this->value_;
        }

    }
}
bool GJson::ReadFile(QFile* file, QString*& str){
    QTextStream in(file);
    // 设置字符编码为UTF-8
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    QString* content = new QString;
    this->space_sign_array.push_back(ONLY_OBJECT);
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        *content = in.readAll();
        file->close();
    } else {
        qWarning() << "Can not open the file!";
    }
    // 去除换行、空格、制表符
    content->remove(QRegularExpression("[\\n\\r\\t\\s]"));
    if(this->content[0] == '{'){
        this->content.remove(0,1);
        this->content.chop(1);
    }
    qDebug() << "File content: " << content;
    str = content;
    return true;
}

// 判断value是否为GJson对象，判断完成后修改传入的value值。
bool GJson::JudgeObject(QString json_str)
{
    if(json_str[0] == '{'){
        QString tmp_str = json_str;
        tmp_str.remove(0, 1);
        tmp_str.chop(1);
        GJson* p_obj = new GJson(tmp_str);
        this->value_.push_back(p_obj);       // 使用完要释放
        this->space_sign_array.push_back(ONLY_OBJECT);
        this->array_size.push_back(std::vector<int>(1, 1));
        return true;
    }
    else{
        return false;
    }
}
// 判断value是否为字符串类型，判断完成后修改传入的value值。
bool GJson::JudgeString(QString json_str)
{
    if(json_str[0] == '\"'){
        QStringList tmp_list = json_str.split("\"");
        QString* p_str = new QString(tmp_list[1]);
        this->space_sign_array.push_back(ONLY_OBJECT);
        this->value_.push_back(p_str);
        this->array_size.push_back(std::vector<int>(1, 1));
        return true;
    }
    else{
        return false;
    }
}
// 判断value是否为bool类型，判断完成后修改传入的value值。
bool GJson::JudgeBool(QString json_str)
{
    bool* p_bool = new bool;
    this->space_sign_array.push_back(NORMAL_VARIABLE);
    if(json_str.left(4) == "true"){
        *p_bool = true;
        this->value_.push_back(p_bool);
        this->array_size.push_back(std::vector<int>(1, 1));
        return true;
    }
    else if(json_str.left(5) == "false"){
        *p_bool = false;
        this->value_.push_back(p_bool);
        this->array_size.push_back(std::vector<int>(1, 1));
        return true;
    }
    else{
        return false;
    }
}
// 判断value是否为数字，判断完成后修改传入的value值。
bool GJson::JudgeNum(QString json_str)
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
            double* p_double = new double(tmp_number);
            this->space_sign_array.push_back(NORMAL_VARIABLE);
            this->value_.push_back(p_double);
            this->array_size.push_back(std::vector<int>(1, 1));
            return true;
        }
        // 后续需要加上科学计数法的判断
        int tmp_number = tmp_str.toInt();
        if(!symbol)
            tmp_number = 0 - tmp_number;
        int* p_int = new int(tmp_number);
        this->space_sign_array.push_back(NORMAL_VARIABLE);
        this->value_.push_back(p_int);
        this->array_size.push_back(std::vector<int>(1, 1));
        return true;
    }
    else{
        return false;
    }
}
// 判断value是否为空，判断完成后修改传入的value值。
bool GJson::JudgeNull(QString json_str)
{
    if(json_str.left(4) == "null"){
        this->value_.push_back(NULL);
        this->array_size.push_back(std::vector<int>(1, 0));
        return true;
    }
    else{
        return false;
    }
}
template <typename T>
bool GJson::JudgeArray(QString json_str)
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
            GJson** p_obj_array = new GJson*[tmp_list.length()];
            for(int i = 0; i < tmp_list.length(); i++){
                p_obj_array[i] = new GJson(tmp_list[i]);
            }
            this->value_.push_back(p_obj_array);
            this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
            this->space_sign_array.push_back(OBJECT_ARRAY);
        }
        // QString数组
        if(tmp_str[0] == '\"'){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            QStringList* p_stringlist = new QStringList();
            *p_stringlist = tmp_str.split("\",\"");
            this->value_.push_back(p_stringlist);
            this->space_sign_array.push_back(ONLY_OBJECT);
            this->array_size.push_back(std::vector<int>(1, 1));
        }
        // bool类型数组
        if(tmp_str.left(4) == "true" || tmp_str.left(5) == "false"){
            QStringList tmp_list = tmp_str.split(",");
            bool* p_bool_array = new bool[tmp_list.length()];
            for(int i = 0; i < tmp_list.length(); i++){
                if(tmp_list[i] == "true")
                    p_bool_array[i] = true;
                else
                    p_bool_array[i] = false;
            }
            this->value_.push_back(p_bool_array);
            this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
            this->space_sign_array.push_back(NORMAL_ARRAY);
        }
        // 数字（整型或浮点型）数组
        if((tmp_str[0] >= '0' && tmp_str[0] <= '9') || tmp_str[0] == "-"){
            QStringList tmp_list = tmp_str.split(",");
            NumberList* p_numberlist = new NumberList(tmp_list);
            this->value_.push_back(p_numberlist);
            this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
            this->space_sign_array.push_back(ONLY_OBJECT);
        }
        // 保存空值的数组
        if(tmp_str.left(4) == "null"){
            QStringList tmp_list = tmp_str.split(",");
            void** p_null_array = new void*[tmp_list.length()];
            for(int i = 0; i < tmp_list.length(); i++){
                p_null_array[i] = NULL;
            }
            this->value_.push_back(p_null_array);
            this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
        }
        // 空数组
        if(tmp_str == NULL){
            this->value_.push_back(NULL);
            this->array_size.push_back(std::vector<int>(1, 0));
        }
        //  保存数组的数组（高维数组）
        if(tmp_str[0] == '['){
            QStringList tmp_list = tmp_str.split(",");
            T** p_array_array = new T*[tmp_list.length()]; // 需要斟酌
            for(int i = 0; i < tmp_list.length(); i++){
                JudgeArrayArray(tmp_list[i]);
            }

            this->value_.push_back(NULL);
            this->array_size.push_back(std::vector<int>(1, 0));

            this->space_sign_array.push_back(NORMAL_ARRAY);
        }

        return true;
    }
    else{
        return false;
    }
}
template <typename T>
void GJson::SetArray(QString tmp_str, T val)
{
    QStringList tmp_list = tmp_str.split(",");
    T** p_array_array = new T[tmp_list.length()]; // 需要斟酌
    val = p_array_array;
    for(int i = 0; i < tmp_list.length(); i++){
        SetArray(tmp_list[i], val);
    }
    this->value_.push_back(NULL);
    this->array_size.push_back(std::vector<int>(1, 0));
    this->space_sign_array.push_back(NORMAL_ARRAY);
}
template <>
void GJson::SetArray<GJson*>(QString tmp_str, GJson* val)
{
    tmp_str.remove(0, 1);
    tmp_str.chop(1);
    QStringList tmp_list = tmp_str.split("},{");
    GJson** p_obj_array = new GJson*[tmp_list.length()];
    for(int i = 0; i < tmp_list.length(); i++){
        p_obj_array[i] = new GJson(tmp_list[i]);
    }
    this->value_.push_back(p_obj_array);
    this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
    this->space_sign_array.push_back(OBJECT_ARRAY);
}
template<>
void GJson::SetArray<QStringList*>(QString tmp_str, QStringList* val)
{
    tmp_str.remove(0, 1);
    tmp_str.chop(1);
    QStringList* p_stringlist = new QStringList();
    *p_stringlist = tmp_str.split("\",\"");
    this->value_.push_back(p_stringlist);
    this->space_sign_array.push_back(ONLY_OBJECT);
    this->array_size.push_back(std::vector<int>(1, 1));
}
template<>
void GJson::SetArray<bool*>(QString tmp_str, bool* val)
{
    QStringList tmp_list = tmp_str.split(",");
    bool* p_bool_array = new bool[tmp_list.length()];
    for(int i = 0; i < tmp_list.length(); i++){
        if(tmp_list[i] == "true")
            p_bool_array[i] = true;
        else
            p_bool_array[i] = false;
    }
    this->value_.push_back(p_bool_array);
    this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
    this->space_sign_array.push_back(NORMAL_ARRAY);
}
template<>
void GJson::SetArray<NumberList*>(QString tmp_str, NumberList* val)
{
    QStringList tmp_list = tmp_str.split(",");
    NumberList* p_numberlist = new NumberList(tmp_list);
    this->value_.push_back(p_numberlist);
    this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
    this->space_sign_array.push_back(ONLY_OBJECT);
}
template<>
void GJson::SetArray<int*>(QString tmp_str, int* val)
{
    this->value_.push_back(NULL);
    this->array_size.push_back(std::vector<int>(1, 0));
}
template<>
void GJson::SetArray<int**>(QString tmp_str, int** val)
{
    QStringList tmp_list = tmp_str.split(",");
    void** p_null_array = new void*[tmp_list.length()];
    for(int i = 0; i < tmp_list.length(); i++){
        p_null_array[i] = NULL;
    }
    this->value_.push_back(p_null_array);
    this->array_size.push_back(std::vector<int>(1, tmp_list.length()));
}

QString GJson::GetKey(GJson* json_obj)
{

}
