#include "json.h"
GJson::GJson(QFile* file){
    QString* str;
    this->is_file_exit_ = this->ReadFile(file, str);
    if(!is_file_exit_){
        qWarning() << "File is not exist!";
    }
    bool (GJson::*funcPtr[])(QString) = {&GJson::JudgeObject,
                                          &GJson::JudgeString,
                                          &GJson::JudgeBool,
                                          &GJson::JudgeNum,
                                          &GJson::JudgeNull,
                                          &GJson::JudgeGetArray};
    for(int i = 0; i < 6; i++){
        (this->*funcPtr[i])(*str);
    }
}
GJson::GJson(QString str){
    bool (GJson::*funcPtr[])(QString) = {&GJson::JudgeObject,
                                         &GJson::JudgeString,
                                         &GJson::JudgeBool,
                                         &GJson::JudgeNum,
                                         &GJson::JudgeNull,
                                         &GJson::JudgeGetArray};
    for(int i = 0; i < 6; i++){
        (this->*funcPtr[i])(str);
    }
}
GJson::~GJson(){
    for(int i = 0; i < array_size_.size(); i++){
        if(!array_size_[i].first){
            if(array_size_[i].second)
                delete value_[i].first;
        }
        else{
            std::vector<void*> p_list;
            std::vector<void*> p_tmp_list;
            void* p_tmp = value_[i].first;
            for(int index_of_depth = array_size_[i].second; index_of_depth >= 0; index_of_depth--){
                for(int index_index_of_now_width = 0; index_index_of_now_width < index_of_depth; index_index_of_now_width++){
                    p_tmp_list.push_back(p_tmp);
                    //p_tmp_list.push_back();
                }
                //for(int index_of_widtn = )
                p_list.push_back(NULL);
            }
        }
    }
}
//std::vector<void*> GJson::GetAllPointer(std::vector<void*>* pointer)
//{
//    for(int i = 0; i < pointer->size(); i++)
//    {
//        //GetAllPointer(*pointer[i].first);
//    }
//    vector<>
//    return NULL;
//}
bool GJson::ReadFile(QFile* file, QString*& str){
    QTextStream in(file);
    // 设置字符编码为UTF-8
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    QString* content = new QString;
    //this->space_sign_array.push_back(ONLY_OBJECT);
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        *content = in.readAll();
        file->close();
    } else {
        qWarning() << "Can not open the file!";
    }
    // 去除换行、空格、制表符
    content->remove(QRegularExpression("[\\n\\r\\t\\s]"));
    if(content[0] == '{'){
        content->remove(0,1);
        content->chop(1);
    }
    else{
        qWarning() << "Json string is wrong! Please repeat it!";
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
        this->value_.push_back({p_obj, GJSON});       // 使用完要释放
        // this->value_for_delete_.push_back({p_obj, GJSON});
        this->array_size_.push_back({false, 1});
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
        this->value_.push_back({p_str, QSTRING});
        this->array_size_.push_back({false, 1});
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
    if(json_str.left(4) == "true"){
        *p_bool = true;
        this->value_.push_back({p_bool, BOOL});
        this->array_size_.push_back({false, 1});
        return true;
    }
    else if(json_str.left(5) == "false"){
        *p_bool = false;
        this->value_.push_back({p_bool, BOOL});
        this->array_size_.push_back({false, 1});
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
            this->value_.push_back({p_double, DOUBLE});
            this->array_size_.push_back({false, 1});
            return true;
        }
        // 后续需要加上科学计数法的判断
        int tmp_number = tmp_str.toInt();
        if(!symbol)
            tmp_number = 0 - tmp_number;
        int* p_int = new int(tmp_number);
        this->value_.push_back({p_int, INT});
        this->array_size_.push_back({false, 1});
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
        this->value_.push_back({NULL, GNULL});
        this->array_size_.push_back({false, 0});
        return true;
    }
    else{
        return false;
    }
}
template <typename T>
bool GJson::JudgeArray(QString json_str)
{
    QString tmp_str = json_str;
    int depth = 0;
    if(tmp_str[0] == '['){
        tmp_str.remove(0,1);
        tmp_str.chop(1);
        void* p_tmp;
        // GJson对象数组
        if(tmp_str[0] == '{'){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            p_tmp = SetArray<GJson*>(tmp_str, depth);
        }
        // QString数组
        if(tmp_str[0] == '\"'){
            tmp_str.remove(0, 1);
            tmp_str.chop(1);
            p_tmp = SetArray<QStringList*>(tmp_str, depth);
        }
        // bool类型数组
        if(tmp_str.left(4) == "true" || tmp_str.left(5) == "false"){
            p_tmp = SetArray<bool*>(tmp_str, depth);
        }
        // 数字（整型或浮点型）数组
        if((tmp_str[0] >= '0' && tmp_str[0] <= '9') || tmp_str[0] == "-"){
            p_tmp = SetArray<NumberList*>(tmp_str, depth);
        }
        // 保存空值的数组
        if(tmp_str.left(4) == "null"){
            p_tmp = SetArray<int**>(tmp_str, depth);
        }
        // 空数组
        if(tmp_str == NULL){
            p_tmp = SetArray<int*>(tmp_str, depth);
        }
        //  保存数组的数组（高维数组）
        if(tmp_str[0] == '['){
            p_tmp = SetArray<T>(tmp_str, depth);
        }
        this->array_size_.push_back({true, depth});
        this->value_.push_back({p_tmp, ARRAY});
        return true;
    }
    else{
        return false;
    }
}
bool GJson::JudgeGetArray(QString str)
{
    int i = 0;
    bool is_ok = false;
    while(str[i] == '['){
        i++;
    }
    if(str[i] == '{')
        is_ok = JudgeArray<GJson*>(str);
    if(str[i] == '\"')
        is_ok = JudgeArray<QStringList*>(str);
    if(str[i] == 't' || str[i] == 'f')
        is_ok = JudgeArray<bool*>(str);
    if((str[i] >= '0' && str[i] <= '9') || str[i] == "-")
        is_ok = JudgeArray<NumberList*>(str);
    if(str[i] == 'n')
        is_ok = JudgeArray<int**>(str);
    if(str[i] == ']')
        is_ok = JudgeArray<int*>(str);

    return is_ok;
}
template <typename T>
void* GJson::SetArray(QString tmp_str, int& depth)
{
    depth++;
    QStringList tmp_list = tmp_str.split(",");
    std::vector<T>* p_array_array = new std::vector<T>(tmp_list.length()); // 需要斟酌
    for(int i = 0; i < tmp_list.length(); i++){
        SetArray<T>(tmp_list[i]);
    }
    return p_array_array;
}
template <>
void* GJson::SetArray<GJson*>(QString tmp_str, int& depth)
{
    depth++;
    tmp_str.remove(0, 1);
    tmp_str.chop(1);
    QStringList tmp_list = tmp_str.split("},{");
    std::vector<GJson*>* p_obj_array = new std::vector<GJson*>(tmp_list.length());
    for(int i = 0; i < tmp_list.length(); i++){
        p_obj_array->push_back(new GJson(tmp_list[i]));
    }
    return p_obj_array;
}
template<>
void* GJson::SetArray<QStringList*>(QString tmp_str, int& depth)
{
    depth++;
    tmp_str.remove(0, 1);
    tmp_str.chop(1);
    QStringList* p_stringlist = new QStringList();
    *p_stringlist = tmp_str.split("\",\"");
    return p_stringlist;
}
template<>
void* GJson::SetArray<bool*>(QString tmp_str, int& depth)
{
    depth++;
    QStringList tmp_list = tmp_str.split(",");
    std::vector<bool>* p_bool_array = new std::vector<bool>(tmp_list.length());
    for(int i = 0; i < tmp_list.length(); i++){
        if(tmp_list[i] == "true")
            p_bool_array->push_back(true);
        else
            p_bool_array->push_back(false);
    }
    return p_bool_array;
}
template<>
void* GJson::SetArray<NumberList*>(QString tmp_str, int& depth)
{
    depth++;
    QStringList tmp_list = tmp_str.split(",");
    NumberList* p_numberlist = new NumberList(tmp_list);
    return p_numberlist;
}
template<>
void* GJson::SetArray<int*>(QString tmp_str, int& depth)
{
    depth++;
    return NULL;
}
template<>
void* GJson::SetArray<int**>(QString tmp_str, int& depth)
{
    depth++;
    QStringList tmp_list = tmp_str.split(",");
    std::vector<void*>* p_null_array = new std::vector<void*>(tmp_list.length());
    for(int i = 0; i < tmp_list.length(); i++){
        p_null_array->push_back(NULL);
    }
    return p_null_array;
}

QString GJson::GetKey(GJson* json_obj)
{
    return "ok";
}
