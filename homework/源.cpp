#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include<iomanip>
#include <vector>
#include <any>
#include<map>
#include <cstdlib>
#include <Windows.h>
#include<string>
#include<fstream>
#include<sstream>
#include <random>
#include <unordered_map>
using namespace std;
//user类可被Customer，Admin继承，作为他们的父类
class User {
protected:
    string username;
    string password;
public:
    User(string username, string password) {
        this->username = username;
        this->password = password;
    }
    string getUsername() {
        return username;
    }
    string getPassword() {
        return password;
    }
    void setPassword(string newPassword) {
        password = newPassword;
    }
};
//顾客类，还有name，address的属性
class Customer : public User {
private:
    string name;
    string address;
public:
    Customer(string username, string password, string name, string address)
        : User(username, password) {
        this->name = name;
        this->address = address;
    }
    string getName() {
        return name;
    }
    string getAddress() {
        return address;
    }
};
//管理员
class Admin : public User {
public:
    Admin(string username, string password)
        : User(username, password) {}
};
//产品类
class Product {
public:
    string name;
    double price;
    int num;//商品名，价格，数量
    Product(string name, double price,int num) {
        this->name = name;
        this->price = price;
        this->num = num;
    }
};
//购物商城，可以展示商品内容，购买商品
class shoppingmall {
public:
    vector<Product> products;//用于储存商城里的商品
    void addItem(Product item) {//向向量里添加商品
        int falg = 0;
        for (int i = 0; i < products.size(); i++) {
            if (products[i].name == item.name) {
                products[i].num++;
                falg = 1;
            }
        }
        if (!falg) {
            products.push_back(item);//push_back() 是 C++ 中 vector 容器的成员函数，用于在容器的末尾添加一个元素。
            products[products.size() - 1].num++;
        }
    }
    int searchProductByName(const string& name) const {//直接查找某商品
        bool found = false;
        //遍历向量
        for (const auto& product : products) {
            if (product.name == name) {
                cout << "Name: " << product.name << endl;
                cout << "Price: " << product.price << endl;
                cout << "Num: " << product.num << endl;
               
                found = true;//更改状态为已找到
                return 1;
                break;
            }
        }

        if (!found) {
            cout << "Product not found." << endl;//否则，未找到
            return 0;
        }
    }
    //根据关键词搜索并推荐
    void recommendProductByKeyword(const string& keyword) const {
       vector<Product> recommendedProducts;

        for (const auto& product : products) {
            if (product.name.find(keyword) != string::npos) {
                recommendedProducts.push_back(product);
            }
        }
        //推荐向量为空
        if (recommendedProducts.empty()) {
            cout << "No recommended products found." << endl;
        }
        else {
            cout << "Recommended Products:" << endl;

            for (const auto& product : recommendedProducts) {
                cout << "Name: " << product.name << endl;
                cout << "Price: " << product.price <<endl;
                cout << "Num: " << product.num << endl;
            }
        }
    }
    int return_num() {//返回购物商城商品品类数
        return products.size();
    }
    void removeItem(int index) {//移除商品
        if (index >= 0 && index < products.size()) {
            products.erase(products.begin() + index);
        }
    }
    void printCart() {//打印购物车
        cout << "Contents:" << endl;
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << " - $" 
                << products[i].price << " 库存: "<< products[i].num <<endl;
        }
    }
    void resise_name(int i,string name) {//修改商品名字
        products[i].name = name;
    }
    void resise_num(int i, int num) {//修改商品数量
        products[i].num = num;
    }
    void resise_pri(int i, double price) {//修改商品价格
        products[i].price = price;
    }
    void buysomething(int i) {//购买商品后数量减少
        products[i].num--;
    }
    int return_pronum(int i) {//返回商品库存
        return products[i].num;
    }
    Product return_p(int i) {//返回Product
        return products[i];
    }
    void save(const string& filename, Product i) {//将商品信息添加进文件
        ofstream file(filename, ios::app);
        if (file.is_open()) {

            file << i.name << "," << i.price << "," << i.num << "\n";

            file.close();
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
    void restoreFromDisk(const string& filename) {//将商品信息从文件中导至向量
        ifstream file(filename);
        products.clear();//先对向量内容进行清空
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {//按行读取内容
                string name;
                double price;
                int num;
                size_t pos = line.find(",");//寻找以，分割的第一个位置
                if (pos != string::npos) {
                    name = line.substr(0, pos);
                    price = stod(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);//第2个逗号
                    num = stoi(line.substr(pos + 1));
                    products.push_back(Product(name, price, num));
                }
            }
            file.close();
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
};
struct PurchaseHistory {//购买历史，包括商品以及购买时间
   vector<Product> products;
   string purchasetime;
};

//购物车，可以增删物品，获取物品总价格，以及展示商品
class ShoppingCart{
public:
    double discountRate=0;
    vector<Product> shproducts;
    unordered_map<string, double> coupons;//优惠券
    vector<PurchaseHistory> purchaseHistories;
    void init_pro() {//初始化，使加入购物车的商品数量为1
        for (int i = 0; i < shproducts.size(); i++) {
            shproducts[i].num = 1;
        }
    }
    Product return_p(int i) {
        return shproducts[i];
    }
    //erase()用于删除从begin开始后的index的物品
    void removeItem(int index) {
        if (index >= 0 && index < shproducts.size()) {
            shproducts.erase(shproducts.begin() + index);
        }
    }
    void addItem(Product& product) {
        int falg = 0;
        for (int i = 0; i < shproducts.size(); i++) {
            if (shproducts[i].name == product.name) {
                shproducts[i].num++;
                falg = 1;
            }
        }
        if (!falg) {
            shproducts.push_back(product);//push_back() 是 C++ 中 vector 容器的成员函数，用于在容器的末尾添加一个元素。
            shproducts[shproducts.size()-1].num++;
        }
    }
    double buysomething(int i) {//购买商品返回其总价格      
        double pri = shproducts[i].price * shproducts[i].num;
        return pri;
    }
    void revise_item(int index,int num) {//修改数量
        shproducts[index].num=num;
    }
    int return_num() {//返回购物车商品数量
        return shproducts.size();
    }
    double getTotalPrice() {//计算总价
        double totalPrice = 0;
        for (int i = 0; i < shproducts.size(); i++) {
            totalPrice += shproducts[i].price* shproducts[i].num;
        }
        return totalPrice;
    }
    void printCart() {//打印购物车内容
        cout << "Shopping Cart Contents:" << endl;
        for (int i = 0; i < shproducts.size(); i++) {
            cout << i + 1 << ". " << shproducts[i].name <<" 数量：" << shproducts[i].num << "件 " 
                << " - $" << shproducts[i].price << endl;
        }
        cout << "Total Price: $" << getTotalPrice() << endl;
    }
    void addCoupon(const string& couponCode, double discount) {//添加优惠券以及对应优惠
        coupons[couponCode] = discount;
    }
    void setDiscountRate(double rate) {
        discountRate = rate;
    }
    double calculateTotalPriceAfterCoupon(double totalPrice, const string& couponCode) const {
        auto itr = coupons.find(couponCode);//计算打完折后的总价钱

        if (itr != coupons.end()) {
            double discount = itr->second;
            return totalPrice * (1 - discount)*(1-discountRate);
        }
        else {
            return totalPrice * (1 - discountRate);
        }
    }
    void saveToDisk(const string& filename,Product i) {//将商品信息追加到文件里
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            
                file << i.name << "," << i.price << "," << 1<< "\n";
            
            file.close();
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
    void savepurchase(const string& filename, Product i,string time) {
        ofstream file(filename, ios::app);//将购买记录储存到文件里
        if (file.is_open()) {

            file << i.name << "," << i.price << "," <<i.num<<"," << time << "\n";

            file.close();
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
    void restoreFromDisk(const string& filename) {
        ifstream file(filename);//将文件信息导入至向量里
        shproducts.clear();
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                string name;
                double price;
                int num;
                size_t pos = line.find(",");//以，分割
                if(pos != string::npos) {
                    name = line.substr(0, pos);
                    price =stod(line.substr(pos + 1));
                    pos = line.find(",",pos+1);//第二个，
                    num= stoi(line.substr(pos +1));
                    shproducts.push_back(Product(name, price,num));
                }
            }
            file.close();//关闭文件
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
    void restorepurchase(const string& filename) {
        ifstream file(filename);
        purchaseHistories.clear();
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {//按行读取
                string name;
                double price;
                string time;
                int num;
                size_t pos = line.find(",");//以，进行分割
                if (pos != string::npos) {
                    name = line.substr(0, pos);
                    price = stod(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);
                    num = stoi(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);//第2个，
                    time = line.substr(pos + 1);
                    PurchaseHistory purchaseHistory;
                    vector<Product> temp;
                    temp.push_back(Product(name, price, num));
                    purchaseHistory.products = temp;
                    purchaseHistory.purchasetime = time;
                    purchaseHistories.push_back(purchaseHistory);//把包含产品购买时间和商品传入
                }
            }
            file.close();
        }
        else {
            cerr << "无法打开文件 " << filename << endl;
        }
    }
    vector<Product> temp(int i) {//返回向量
        vector<Product> temp;
        temp.clear();
        temp.push_back(shproducts[i]);
        return temp;
    }

    // 添加购买记录
    void addPurchaseRecord(const vector<Product>& products, const string& purchaseTime) {
        PurchaseHistory purchaseHistory;
        purchaseHistory.products = products;
        purchaseHistory.purchasetime = purchaseTime;
        purchaseHistories.push_back(purchaseHistory);
    }

    // 显示购物历史记录
    void displayPurchaseHistory() {
        if (purchaseHistories.empty()!=true) {
            for (const auto& purchaseHistory : purchaseHistories) {
                cout << "购买时间：" << purchaseHistory.purchasetime << endl;
                for (const auto& product : purchaseHistory.products) {
                    cout << "商品名称：" << product.name;
                    cout << "，商品价格：" << product.price << endl;
                }
                cout << endl;
            }
        }
        else
        {
            cout << "您并没有购买记录。" << endl;
        }
    }

    // 根据时间展示购物数据分析
    void displayPurchaseAnalysis(const string& purchaseTime) {
        map<string, double> categoryAmounts;
        if (!purchaseHistories.empty()) {//如果为不为空的话才能操作
            for (const auto& purchaseHistory : purchaseHistories) {
                if (purchaseHistory.purchasetime == purchaseTime) {
                    for (const auto& product : purchaseHistory.products) {
                        categoryAmounts[product.name] += product.price;
                    }
                }
            }
            cout << "购买时间：" << purchaseTime << std::endl;
            for (const auto& entry : categoryAmounts) {
                cout << "商品名称：" << entry.first;
                cout << "，购买金额：" << entry.second << std::endl;
            }
        }
        else {
            cout << "您并没有购买数据。" << endl;
        }
    }
    
};

int main() {
    Admin admin("admin", "admin123");
    vector<Customer> customers;
    
    // 注册顾客
    string username, password,name,address="null";
    User* currentUser = nullptr;
    // 模拟购买过程
    Product item1("iPhone-phone", 5999.99,1000);
    Product item2("ASUS-Laptop", 6499.99,1000);
    Product item3("daiannaflower", 60.99,50);
    Product item4("cabuqinuoflower", 80.99, 50);
    //添加商品
    shoppingmall cart;
    cart.addItem(item1);
    cart.addItem(item2);
    cart.addItem(item3);
    cart.addItem(item4);
    // 给顾客随机派送优惠券
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 0.2);
    
    ShoppingCart content;
    string couponCode = "I Love XD";
    double couponValue = dis(gen); // 随机生成优惠券面值
    content.addCoupon(couponCode, couponValue);
    //活动优惠
    double discountRate = 0.2; // 假设活动折扣为 20%
    content.setDiscountRate(discountRate);
    int pnum;
    string filename;
    string hisname;
    //封面
    int temp=-1;
    for (int i = 0;i < 5;i++) cout << endl;
    cout << "                            ***       ***   " << "                            ***       ***   " << endl;
    cout << "                          ******     ****** " << "                          ******     ****** " << endl;
    cout << "                         ********   ********" << "                         ********   ********" << endl;
    cout << "                          ******** ******** " << "                          ******** ******** " << endl;
    cout << "                           ***************  " << "                           ***************  " << endl;
    cout << "                             ***********    " << "                             ***********    " << endl;
    cout << "                               *******      " << "                               *******      " << endl;
    cout << "                                 ***        " << "                                 ***        " << endl;
    cout << "                              太好逛了吧    " << "                              太好逛了吧    " << endl;
    cout << endl;
    //菜单选择
    while (temp) {
        switch (temp) {
        case -2: {//管理员功能
            int choise;
            string name;
            double price;
            int num=0;
            int list;
            int flag = 1;
            string mallfile;
            while (flag) {
                cout << "选择1.上架物品  2.修改信息  3.删除物品 4.退出 ：";
                cin >> choise;
                cart.printCart();
                switch (choise) {
                case 1:{//上架物品
                    mallfile = "mall_item.txt";
                    ofstream input(mallfile, ios::app);//追加的形式添加
                    if (input.is_open()) {
                        cout << "上架物品。输入商品名 价格 库存数量：";
                        cin >> name >> price >> num;
                        Product item(name, price, num);
                        cart.addItem(item);//添置
                        cart.save(mallfile, item);//加入文件
                        cart.restoreFromDisk(mallfile);//导入至向量
                        input << name << " " << price << " " << num << endl;
                        input.close();
                    }
                    else {
                        cout << "无法打开文件" << endl;
                    }                    
                   
                    break;
                }
                case 2: {//修改
                    string filename = "mall_item.txt"; // 文件名
                    string tempFilename = "temp.txt"; // 临时文件名
                    int lineIndex; // 需要修改的行号，从0开始
                    int elementIndex; // 需要修改的元素在行中的位置，从0开始

                    ifstream input(filename); // 打开源文件读取内容
                    ofstream tempOutput(tempFilename); // 打开临时文件准备写入内容
                    if (!input.is_open() || !tempOutput.is_open()) {
                        cout << "打开文件失败。" << endl;
                        return 0;
                    }
                    string line;
                    cout << "修改信息。输入要修改物品序号：";
                    cin >> list;
                    lineIndex = list - 1;
                    int currentLineIndex = 0;
                    while (getline(input, line)) {
                        
                        if (currentLineIndex == lineIndex) {
                            stringstream ss(line);
                            string element;
                            vector<string> elements;
                            int currentElementIndex = 0; 
                            cout << "修改 1.商品名 2.价格 3.库存数量 ";
                            cin >> choise;
                            while (ss >> element) {//以空格分割直接遍历读取
                                elements.push_back(element);
                            }
                            if (choise == 1) {
                                cout << "请输入修改后的商品名：";
                                cin >> name;
                                cart.resise_name(lineIndex, name);//把行号和需要修改的传入
                                elements[0] = name;//修改name
                                
                            }
                            else if (choise == 2) {
                                cout << "请输入修改后的价格：";
                                cin >> price;
                                cart.resise_pri(lineIndex, price);//把行号和需要修改的传入
                                elements[1] = to_string(price);//修改price
                            }
                            else if (choise == 3) {
                                cout << "请输入修改后的库存：";
                                cin >> num;
                                cart.resise_num(lineIndex, num);//把行号和需要修改的传入
                                elements[2] = to_string(num);//修改num
                            }
                            for (int i = 0;i < 3;i++) {
                                tempOutput << element[i] << " ";
                            }//只更正需要修改的那个，其余的照常加入
                            }
                        else {//如果不是需要修正的行，照常添加进临时文件中
                            tempOutput << line <<endl;
                        }
                        currentLineIndex++;//继续遍历
                    }

                    input.close();
                    tempOutput.close();

                    remove(filename.c_str()); // 删除原始文件
                    rename(tempFilename.c_str(), filename.c_str()); // 重命名临时文件为原始文件的名称
                    break;
                }
                case 3: {
                    cout << "删除物品。输入要删除物品序号：";
                    cin >> list;
                    cart.removeItem(list);
                    string filename = "mall_item.txt"; // 文件名
                    string tempFilename = "temp.txt"; // 临时文件名
                    int lineIndex; // 需要修改的行号，从0开始
                    ifstream input(filename); // 打开源文件读取内容
                    ofstream tempOutput(tempFilename); // 打开临时文件准备写入内容
                    if (!input.is_open() || !tempOutput.is_open()) {
                        cout << "打开文件失败。" << endl;
                        return 0;
                    }
                    string line;
                    lineIndex = list - 1;
                    int currentLineIndex = 0;
                    while (getline(input, line)) {//按行遍历
                        if (currentLineIndex != lineIndex) {
                            tempOutput << line << endl;
                        }
                        currentLineIndex++;
                    }
                    input.close();
                    tempOutput.close();

                    remove(filename.c_str()); // 删除原始文件
                    rename(tempFilename.c_str(), filename.c_str()); // 重命名临时文件为原始文件的名称
                    break;
                }
                case 4: {
                    flag = 0;
                    temp = -1;
                    break;
                }
                }
            }
            break;
        }
        case -1: {
//打印主菜单
         
            cout << "           ==============================================================================================" << endl;
            cout << "                                                        菜单" << endl;
            cout << "           ==============================================================================================" << endl;
            // 设置列宽和对齐方式
            cout << "           | " << setw(4) << left << "选项"
                << "           | " << setw(73) << left << "                                  功能" << " |" << endl;
            cout << "          ----------------------------------------------------------------------------------------------" << endl;
            cout << "           | " << setw(4) << left << " 1"
                << "           | " << setw(73) << left << "选项一 登录账号" << " |" << endl;

            cout << "           | " << setw(4) << left << " 2"
                << "           | " << setw(73) << left << "选项二 注册账号" << " |" << endl;
            cout << "           | " << setw(4) << ::left << " 3"
                << "           | " << setw(73) << std::left << "选项三 进入购物商场" << " |" << endl;
            cout << "           | " << setw(4) << left << " 4"
                << "           | " << setw(73) << left << "选项四 查看购物车" << " |" << endl;
            cout << "           | " << setw(4) << left << " 5"
                << "           | " << setw(73) << left << "选项五 用户管理" << " |" << endl;
            cout << "           | " << setw(4) << left << " 6"
                << "           | " << setw(73) << left << "选项六 查询商品" << " |" << endl;
            cout << "           =============================================================================================" << endl;
            cout << "            请输入您的选择：";
            cin >> temp;
            system("cls");
            break;
        }
         //登录界面
        case 1: {
            system("cls");
            cout << "登录中......." << endl;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            int flag = 0;

            //管理员登录
            if (username == admin.getUsername() && password == admin.getPassword()) {
                currentUser = &admin;
                cout << "Admin login successful" << endl;
                temp = -2;
            }
            //遍历user文件，登录用户账号
            else {
                int i;
                string line;
                ifstream inputfile("user_inf.txt");
                while (getline(inputfile, line)) {
                    istringstream iss(line);
                    vector<string> elements;
                    string element;
                    while (iss >> element) {
                        elements.push_back(element);
                    }
                    if (elements[0] == username && elements[1] == password) {
                        cout << "成功登录" << endl;
                        temp = 3;
                        static Customer customer(elements[0], elements[1], elements[2], elements[3]);
                        customers.push_back(customer);
                        currentUser = &customer;
                        filename = currentUser->getUsername() + ".txt";//以用户名命名购物车文件
                        hisname = currentUser->getUsername() + "pur.txt";//以用户名命名购物历史文件
                        content.restoreFromDisk(filename);//将文件内容导入向量
                        system("cls");
                    }
                }//如果用户不在文件中，则转至注册界面
                    if(temp!=3) {
                        char ch;
                        cout << "对不起，您好像还没有注册。请问需要注册账号么？（y/n）："<<endl;
                        cin >> ch;
                        if (ch == 'y' || ch == 'Y') {
                            temp = 2;//注册选项
                            system("cls");//清屏
                        }
                    }
            }           
            break;
        }//注册界面
        case 2: {
            cout << "注册账户中......" << endl;
            ofstream input("user_inf.txt",ios::app);
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            cout << "name: ";
            cin >> name;
            cout << "address: ";
            cin >> address;
            if (input.is_open()) {
                input << username << " " << password << " " << name << " " << address<<endl;
                input.close();
            }
            else {
                cout << "无法打开文件" << endl;
            }
            static Customer customer(username, password, name, address);
            customers.push_back(customer);
            currentUser = &customer; 
            system("cls");
            cout << "恭喜您注册成功！！";
            temp = 3;//自动转至购物商城界面
           
            break;
        }
              //购物商城
        case 3: {
            
            char ch;
            string filename = currentUser->getUsername() + ".txt";
            cout << "Welcome, " << static_cast<Customer*>(currentUser)->getName() << "!!!" << endl;
            cout << "Address: " << static_cast<Customer*>(currentUser)->getAddress() << endl;
            cart.printCart();//打印购物商城内容
            cout << "今天优惠大促，全场优惠" << content.discountRate * 100 << '%' << "并享受随机金额优惠卷叠加使用！"
                << "走过路过，不要错过！" << endl;
            string checkpass;
            int a=0;
            cout << "请问您有心仪的物品么？（y/n）： ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                if (currentUser == &admin) {
                    cout << "Only customers can buy products." << endl;//管理员无法操作
                    temp = -2;
                }
                else {     
                    while (1) {
                        if((a++)) cart.printCart();//只有第一次进来的时候不打印，
                        cout << "请输入物品序号： ";
                        cin >> pnum;
                        if (pnum <= cart.return_num()) {
                            system("cls");
                            cout << "加入购物车成功!" << endl;
                            Product a = cart.return_p(pnum - 1);
                            content.saveToDisk(filename, a);//将商品添加至文件中
                            content.restoreFromDisk(filename);//并将文件内容导入向量
                            content.init_pro();
                           
                            
                            break;
                        }
                        else {
                            system("cls");
                            cout << "输入序号有误,请重新输入" << endl;

                        }
                    }
                    
                                      
                }
            }
            else {
                system("cls");
                temp = -1;
            }
            break;
        }
//查看购物车
        case 4: {
            
            if(currentUser== &admin) {
                cout << "Only customers can check their shoppingcar" << endl;
                temp = -2;
            }
            else if (currentUser == NULL) {
                cout << "不好意思，您还没有登录。" << endl;
                temp = 1;
                
            }
            else{
                int t=1;
                int a,num;
                char ch;
                string checkpass;
                string inputCouponCode;
                
                while (t!=4) {
                    system("cls");
                    time_t currentTime = time(nullptr);

                    // 格式化时间戳为字符串
                    char buffer[80];
                    strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&currentTime));
                    
                    cout << "购物车(" << content.return_num() << ")" << endl;
                    // 打印购物车内容和总价
                    content.printCart();
                    cout << "1.购买 2.修改 3.删除 4.返回主界面:";
                    cin >> t;
                    switch (t) {
                    case 1: {
                        
                        cout << "请输入物品序号： ";
                        cin >> a;
                        //输入密码才能正常购买
                        
                        while (1) {
                            cout << "请输入您的账户密码以保证正常支付： ";
                            cin >> checkpass;
                            cout << "请问您有要使用的优惠券嘛？(y/n):";
                            cin >> ch;
                            if (ch == 'y' || ch == 'Y') {
                                //输入优惠券码
                                cout << "Enter your coupon code: ";
                                cin.ignore();
                                getline(cin,inputCouponCode);
                                if (checkpass == currentUser->getPassword()&& inputCouponCode== couponCode) {
                                    content.addPurchaseRecord(content.temp(a - 1), buffer);
                                    double num = content.buysomething(a - 1);
                                    double calnum = content.calculateTotalPriceAfterCoupon(num, inputCouponCode);
                                    cout << "恭喜您支付成功！！原价为：" << num << "元 "<<"折后价为："<<calnum<<"元" << endl;
                                    content.savepurchase(hisname, content.return_p(a - 1), buffer);
                                    content.removeItem(a - 1); //移除购物车中已购买的商品
                                    break;
                                }
                                else if(checkpass != currentUser->getPassword()&& inputCouponCode == couponCode) {
                                    cout << "密码错误，请重新输入。" << endl;
                                }
                                else if (inputCouponCode != couponCode&& checkpass == currentUser->getPassword()) {
                                    cout << "优惠码错误，请重新输入。" << endl;
                                }
                            }
                            else {
                                if (checkpass == currentUser->getPassword()) {
                                    content.addPurchaseRecord(content.temp(a - 1), buffer);
                                    cout << "恭喜您支付成功！！支付价为：" << content.buysomething(a - 1) << "元"<< endl;
                                    content.savepurchase(hisname, content.return_p(a - 1), buffer);
                                    content.removeItem(a - 1);
                                    
                                    break;
                                }
                                else {
                                    cout << "密码错误，请重新输入。" << endl;
                                }
                            }
                            
                        }
                        break;}
                    case 2: {
                        cout << "修改商品序号为：";
                        cin >> a;
                        cout << "数量更改为： ";
                        cin >> num;
                        content.revise_item(a - 1, num);

                        string tempFilename = "temp.txt"; // 临时文件名
                        int lineIndex; // 需要修改的行号，从0开始
                        int elementIndex; // 需要修改的元素在行中的位置，从0开始

                        ifstream input(filename); // 打开源文件读取内容
                        ofstream tempOutput(tempFilename); // 打开临时文件准备写入内容
                        if (!input.is_open() || !tempOutput.is_open()) {
                            cout << "打开文件失败。" << endl;
                            return 0;
                        }
                        string line;
                       

                        lineIndex = a - 1;
                        int currentLineIndex = 0;
                        while (getline(input, line)) {
                            
                            if (currentLineIndex == lineIndex) {
                                stringstream ss(line);
                                string element;
                                vector<string> elements;
                                int currentElementIndex = 0;                              
                                while (getline(ss, element, ',')) {
                                    elements.push_back(element);
                                }                              
                                elements[2] = to_string(num);
                                for (int i = 0;i < 3;i++) {
                                    if (i != 2) tempOutput << elements[i] << ",";
                                    else tempOutput << elements[i]<<endl;
                                }
                            }
                            else {
                                tempOutput << line << endl;
                            }
                            currentLineIndex++;
                        }
                        input.close();
                        tempOutput.close();

                        remove(filename.c_str()); // 删除原始文件
                        rename(tempFilename.c_str(), filename.c_str()); // 重命名临时文件为原始文件的名称

                        
                        system("cls");
                        cout << "页面刷新中，这是你新的购物车。" << endl;
                        cout << "购物车(" << content.return_num() << ")" << endl;
                        content.printCart();
                        break;
                    }
                    case 3: {
                        cout << "移除商品序号为：";
                        cin >> a;
                        content.removeItem(a - 1);

                        string tempFilename = "temp.txt"; // 临时文件名
                        int lineIndex; // 需要修改的行号，从0开始
                        ifstream input(filename); // 打开源文件读取内容
                        ofstream tempOutput(tempFilename); // 打开临时文件准备写入内容
                        if (!input.is_open() || !tempOutput.is_open()) {
                            cout << "打开文件失败。" << endl;
                            return 0;
                        }
                        string line;
                        lineIndex = a - 1;
                        int currentLineIndex = 0;
                        while (getline(input, line)) {
                            
                            if (currentLineIndex != lineIndex) {
                                tempOutput << line << endl;
                            }
                            currentLineIndex++;
                        }
                        input.close();
                        tempOutput.close();
                        
                        remove(filename.c_str()); // 删除原始文件
                        rename(tempFilename.c_str(), filename.c_str()); // 重命名临时文件为原始文件的名称
                        
                        system("cls");
                        cout << "页面刷新中，这是你新的购物车。" << endl;
                        cout << "购物车(" << content.return_num() << ")" << endl;
                        content.printCart();
                        break;
                    }
                    case 4: {
                        temp = -1;
                        system("cls");
                        break;
                    }
                    }
                    
                }
            } 
            break;
        }
              //用户管理，可以修改用户密码
        case 5: {
            string newPassword;
            int choice;
            cout << "1.修改购物密码 2.查看购物历史 3.获取购物数据分析 4.返回主菜单: ";
            
            if (currentUser == NULL) {
                cout << "不好意思，您还没有登录。" << endl;
                temp = 1;
                break;
            }
            else {
                cin >> choice;
            }
            switch (choice) {
            case 1: {
                
                /*修改顾客密码*/
                if (Customer* currentCustomer = static_cast<Customer*>(currentUser)) {

                    cout << "Enter new password: ";
                    cin >> newPassword;
                    currentCustomer->setPassword(newPassword);
                    cout << "Password updated." << endl;
                }
                else {
                    temp = 1;
                }
                string line;

                string filename = "user_inf.txt";
                string tempFilename = "temp.txt"; // 临时文件名
                ifstream input(filename); // 打开源文件读取内容
                ofstream tempOutput(tempFilename); // 打开临时文件准备写入内容

                if (!input.is_open() || !tempOutput.is_open()) {
                    cout << "打开文件失败。" << endl;
                    return 0;
                }
                while (getline(input, line)) {
                    istringstream iss(line);
                    vector<string> elements;
                    string element;
                    while (iss >> element) {
                        elements.push_back(element);
                    }
                    if (elements[0] == username && elements[1] == password) {
                        elements[1] = newPassword;
                    }
                }
                tempOutput << input.rdbuf();//通过调用rdbuf()函数可以获得streambuf的指针，
                //进而可以直接对缓冲区进行操作，如读取或写入数据，跳转位置等
                input.close();
                tempOutput.close();
                remove(filename.c_str()); // 删除原始文件
                rename(tempFilename.c_str(), filename.c_str()); // 重命名临时文件为原始文件的名称
                break;
            }
            case 2: {//展示购物历史
                content.restorepurchase(hisname);
                content.displayPurchaseHistory();
                break;
            }
            case 3: {//查询一定时间下的购物数据，并分析
                string time;
                content.restorepurchase(hisname);
                cout << "请输入你想查询的日期,格式为年-月-日: " ;
                cin >> time;
                content.displayPurchaseAnalysis(time);
                break;
            }
            case 4: {
                temp = -1;
                break;
            }
            }
            break;
        }
        case 6: {
            string searchName;
            string recommendKeyword;
            int falg = 0;
            // 查询商品信息
            cout << "Enter the name of the product to search: ";
            cin >> searchName;
            falg=cart.searchProductByName(searchName);
            if (!falg) {//查询关键信息
                cout << "Enter a keyword for product recommendation: ";
                cin >> recommendKeyword;
                cart.recommendProductByKeyword(recommendKeyword);
            }
            break;
            }
        default: {
            cout << "输入错误，请重新输入"<<endl;
            temp = -1;
        }
        }
        }
    

    return 0;
}