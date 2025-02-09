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
//user��ɱ�Customer��Admin�̳У���Ϊ���ǵĸ���
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
//�˿��࣬����name��address������
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
//����Ա
class Admin : public User {
public:
    Admin(string username, string password)
        : User(username, password) {}
};
//��Ʒ��
class Product {
public:
    string name;
    double price;
    int num;//��Ʒ�����۸�����
    Product(string name, double price,int num) {
        this->name = name;
        this->price = price;
        this->num = num;
    }
};
//�����̳ǣ�����չʾ��Ʒ���ݣ�������Ʒ
class shoppingmall {
public:
    vector<Product> products;//���ڴ����̳������Ʒ
    void addItem(Product item) {//�������������Ʒ
        int falg = 0;
        for (int i = 0; i < products.size(); i++) {
            if (products[i].name == item.name) {
                products[i].num++;
                falg = 1;
            }
        }
        if (!falg) {
            products.push_back(item);//push_back() �� C++ �� vector �����ĳ�Ա������������������ĩβ���һ��Ԫ�ء�
            products[products.size() - 1].num++;
        }
    }
    int searchProductByName(const string& name) const {//ֱ�Ӳ���ĳ��Ʒ
        bool found = false;
        //��������
        for (const auto& product : products) {
            if (product.name == name) {
                cout << "Name: " << product.name << endl;
                cout << "Price: " << product.price << endl;
                cout << "Num: " << product.num << endl;
               
                found = true;//����״̬Ϊ���ҵ�
                return 1;
                break;
            }
        }

        if (!found) {
            cout << "Product not found." << endl;//����δ�ҵ�
            return 0;
        }
    }
    //���ݹؼ����������Ƽ�
    void recommendProductByKeyword(const string& keyword) const {
       vector<Product> recommendedProducts;

        for (const auto& product : products) {
            if (product.name.find(keyword) != string::npos) {
                recommendedProducts.push_back(product);
            }
        }
        //�Ƽ�����Ϊ��
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
    int return_num() {//���ع����̳���ƷƷ����
        return products.size();
    }
    void removeItem(int index) {//�Ƴ���Ʒ
        if (index >= 0 && index < products.size()) {
            products.erase(products.begin() + index);
        }
    }
    void printCart() {//��ӡ���ﳵ
        cout << "Contents:" << endl;
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << " - $" 
                << products[i].price << " ���: "<< products[i].num <<endl;
        }
    }
    void resise_name(int i,string name) {//�޸���Ʒ����
        products[i].name = name;
    }
    void resise_num(int i, int num) {//�޸���Ʒ����
        products[i].num = num;
    }
    void resise_pri(int i, double price) {//�޸���Ʒ�۸�
        products[i].price = price;
    }
    void buysomething(int i) {//������Ʒ����������
        products[i].num--;
    }
    int return_pronum(int i) {//������Ʒ���
        return products[i].num;
    }
    Product return_p(int i) {//����Product
        return products[i];
    }
    void save(const string& filename, Product i) {//����Ʒ��Ϣ��ӽ��ļ�
        ofstream file(filename, ios::app);
        if (file.is_open()) {

            file << i.name << "," << i.price << "," << i.num << "\n";

            file.close();
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
    void restoreFromDisk(const string& filename) {//����Ʒ��Ϣ���ļ��е�������
        ifstream file(filename);
        products.clear();//�ȶ��������ݽ������
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {//���ж�ȡ����
                string name;
                double price;
                int num;
                size_t pos = line.find(",");//Ѱ���ԣ��ָ�ĵ�һ��λ��
                if (pos != string::npos) {
                    name = line.substr(0, pos);
                    price = stod(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);//��2������
                    num = stoi(line.substr(pos + 1));
                    products.push_back(Product(name, price, num));
                }
            }
            file.close();
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
};
struct PurchaseHistory {//������ʷ��������Ʒ�Լ�����ʱ��
   vector<Product> products;
   string purchasetime;
};

//���ﳵ��������ɾ��Ʒ����ȡ��Ʒ�ܼ۸��Լ�չʾ��Ʒ
class ShoppingCart{
public:
    double discountRate=0;
    vector<Product> shproducts;
    unordered_map<string, double> coupons;//�Ż�ȯ
    vector<PurchaseHistory> purchaseHistories;
    void init_pro() {//��ʼ����ʹ���빺�ﳵ����Ʒ����Ϊ1
        for (int i = 0; i < shproducts.size(); i++) {
            shproducts[i].num = 1;
        }
    }
    Product return_p(int i) {
        return shproducts[i];
    }
    //erase()����ɾ����begin��ʼ���index����Ʒ
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
            shproducts.push_back(product);//push_back() �� C++ �� vector �����ĳ�Ա������������������ĩβ���һ��Ԫ�ء�
            shproducts[shproducts.size()-1].num++;
        }
    }
    double buysomething(int i) {//������Ʒ�������ܼ۸�      
        double pri = shproducts[i].price * shproducts[i].num;
        return pri;
    }
    void revise_item(int index,int num) {//�޸�����
        shproducts[index].num=num;
    }
    int return_num() {//���ع��ﳵ��Ʒ����
        return shproducts.size();
    }
    double getTotalPrice() {//�����ܼ�
        double totalPrice = 0;
        for (int i = 0; i < shproducts.size(); i++) {
            totalPrice += shproducts[i].price* shproducts[i].num;
        }
        return totalPrice;
    }
    void printCart() {//��ӡ���ﳵ����
        cout << "Shopping Cart Contents:" << endl;
        for (int i = 0; i < shproducts.size(); i++) {
            cout << i + 1 << ". " << shproducts[i].name <<" ������" << shproducts[i].num << "�� " 
                << " - $" << shproducts[i].price << endl;
        }
        cout << "Total Price: $" << getTotalPrice() << endl;
    }
    void addCoupon(const string& couponCode, double discount) {//����Ż�ȯ�Լ���Ӧ�Ż�
        coupons[couponCode] = discount;
    }
    void setDiscountRate(double rate) {
        discountRate = rate;
    }
    double calculateTotalPriceAfterCoupon(double totalPrice, const string& couponCode) const {
        auto itr = coupons.find(couponCode);//��������ۺ���ܼ�Ǯ

        if (itr != coupons.end()) {
            double discount = itr->second;
            return totalPrice * (1 - discount)*(1-discountRate);
        }
        else {
            return totalPrice * (1 - discountRate);
        }
    }
    void saveToDisk(const string& filename,Product i) {//����Ʒ��Ϣ׷�ӵ��ļ���
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            
                file << i.name << "," << i.price << "," << 1<< "\n";
            
            file.close();
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
    void savepurchase(const string& filename, Product i,string time) {
        ofstream file(filename, ios::app);//�������¼���浽�ļ���
        if (file.is_open()) {

            file << i.name << "," << i.price << "," <<i.num<<"," << time << "\n";

            file.close();
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
    void restoreFromDisk(const string& filename) {
        ifstream file(filename);//���ļ���Ϣ������������
        shproducts.clear();
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                string name;
                double price;
                int num;
                size_t pos = line.find(",");//�ԣ��ָ�
                if(pos != string::npos) {
                    name = line.substr(0, pos);
                    price =stod(line.substr(pos + 1));
                    pos = line.find(",",pos+1);//�ڶ�����
                    num= stoi(line.substr(pos +1));
                    shproducts.push_back(Product(name, price,num));
                }
            }
            file.close();//�ر��ļ�
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
    void restorepurchase(const string& filename) {
        ifstream file(filename);
        purchaseHistories.clear();
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {//���ж�ȡ
                string name;
                double price;
                string time;
                int num;
                size_t pos = line.find(",");//�ԣ����зָ�
                if (pos != string::npos) {
                    name = line.substr(0, pos);
                    price = stod(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);
                    num = stoi(line.substr(pos + 1));
                    pos = line.find(",", pos + 1);//��2����
                    time = line.substr(pos + 1);
                    PurchaseHistory purchaseHistory;
                    vector<Product> temp;
                    temp.push_back(Product(name, price, num));
                    purchaseHistory.products = temp;
                    purchaseHistory.purchasetime = time;
                    purchaseHistories.push_back(purchaseHistory);//�Ѱ�����Ʒ����ʱ�����Ʒ����
                }
            }
            file.close();
        }
        else {
            cerr << "�޷����ļ� " << filename << endl;
        }
    }
    vector<Product> temp(int i) {//��������
        vector<Product> temp;
        temp.clear();
        temp.push_back(shproducts[i]);
        return temp;
    }

    // ��ӹ����¼
    void addPurchaseRecord(const vector<Product>& products, const string& purchaseTime) {
        PurchaseHistory purchaseHistory;
        purchaseHistory.products = products;
        purchaseHistory.purchasetime = purchaseTime;
        purchaseHistories.push_back(purchaseHistory);
    }

    // ��ʾ������ʷ��¼
    void displayPurchaseHistory() {
        if (purchaseHistories.empty()!=true) {
            for (const auto& purchaseHistory : purchaseHistories) {
                cout << "����ʱ�䣺" << purchaseHistory.purchasetime << endl;
                for (const auto& product : purchaseHistory.products) {
                    cout << "��Ʒ���ƣ�" << product.name;
                    cout << "����Ʒ�۸�" << product.price << endl;
                }
                cout << endl;
            }
        }
        else
        {
            cout << "����û�й����¼��" << endl;
        }
    }

    // ����ʱ��չʾ�������ݷ���
    void displayPurchaseAnalysis(const string& purchaseTime) {
        map<string, double> categoryAmounts;
        if (!purchaseHistories.empty()) {//���Ϊ��Ϊ�յĻ����ܲ���
            for (const auto& purchaseHistory : purchaseHistories) {
                if (purchaseHistory.purchasetime == purchaseTime) {
                    for (const auto& product : purchaseHistory.products) {
                        categoryAmounts[product.name] += product.price;
                    }
                }
            }
            cout << "����ʱ�䣺" << purchaseTime << std::endl;
            for (const auto& entry : categoryAmounts) {
                cout << "��Ʒ���ƣ�" << entry.first;
                cout << "�������" << entry.second << std::endl;
            }
        }
        else {
            cout << "����û�й������ݡ�" << endl;
        }
    }
    
};

int main() {
    Admin admin("admin", "admin123");
    vector<Customer> customers;
    
    // ע��˿�
    string username, password,name,address="null";
    User* currentUser = nullptr;
    // ģ�⹺�����
    Product item1("iPhone-phone", 5999.99,1000);
    Product item2("ASUS-Laptop", 6499.99,1000);
    Product item3("daiannaflower", 60.99,50);
    Product item4("cabuqinuoflower", 80.99, 50);
    //�����Ʒ
    shoppingmall cart;
    cart.addItem(item1);
    cart.addItem(item2);
    cart.addItem(item3);
    cart.addItem(item4);
    // ���˿���������Ż�ȯ
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 0.2);
    
    ShoppingCart content;
    string couponCode = "I Love XD";
    double couponValue = dis(gen); // ��������Ż�ȯ��ֵ
    content.addCoupon(couponCode, couponValue);
    //��Ż�
    double discountRate = 0.2; // �����ۿ�Ϊ 20%
    content.setDiscountRate(discountRate);
    int pnum;
    string filename;
    string hisname;
    //����
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
    cout << "                              ̫�ù��˰�    " << "                              ̫�ù��˰�    " << endl;
    cout << endl;
    //�˵�ѡ��
    while (temp) {
        switch (temp) {
        case -2: {//����Ա����
            int choise;
            string name;
            double price;
            int num=0;
            int list;
            int flag = 1;
            string mallfile;
            while (flag) {
                cout << "ѡ��1.�ϼ���Ʒ  2.�޸���Ϣ  3.ɾ����Ʒ 4.�˳� ��";
                cin >> choise;
                cart.printCart();
                switch (choise) {
                case 1:{//�ϼ���Ʒ
                    mallfile = "mall_item.txt";
                    ofstream input(mallfile, ios::app);//׷�ӵ���ʽ���
                    if (input.is_open()) {
                        cout << "�ϼ���Ʒ��������Ʒ�� �۸� ���������";
                        cin >> name >> price >> num;
                        Product item(name, price, num);
                        cart.addItem(item);//����
                        cart.save(mallfile, item);//�����ļ�
                        cart.restoreFromDisk(mallfile);//����������
                        input << name << " " << price << " " << num << endl;
                        input.close();
                    }
                    else {
                        cout << "�޷����ļ�" << endl;
                    }                    
                   
                    break;
                }
                case 2: {//�޸�
                    string filename = "mall_item.txt"; // �ļ���
                    string tempFilename = "temp.txt"; // ��ʱ�ļ���
                    int lineIndex; // ��Ҫ�޸ĵ��кţ���0��ʼ
                    int elementIndex; // ��Ҫ�޸ĵ�Ԫ�������е�λ�ã���0��ʼ

                    ifstream input(filename); // ��Դ�ļ���ȡ����
                    ofstream tempOutput(tempFilename); // ����ʱ�ļ�׼��д������
                    if (!input.is_open() || !tempOutput.is_open()) {
                        cout << "���ļ�ʧ�ܡ�" << endl;
                        return 0;
                    }
                    string line;
                    cout << "�޸���Ϣ������Ҫ�޸���Ʒ��ţ�";
                    cin >> list;
                    lineIndex = list - 1;
                    int currentLineIndex = 0;
                    while (getline(input, line)) {
                        
                        if (currentLineIndex == lineIndex) {
                            stringstream ss(line);
                            string element;
                            vector<string> elements;
                            int currentElementIndex = 0; 
                            cout << "�޸� 1.��Ʒ�� 2.�۸� 3.������� ";
                            cin >> choise;
                            while (ss >> element) {//�Կո�ָ�ֱ�ӱ�����ȡ
                                elements.push_back(element);
                            }
                            if (choise == 1) {
                                cout << "�������޸ĺ����Ʒ����";
                                cin >> name;
                                cart.resise_name(lineIndex, name);//���кź���Ҫ�޸ĵĴ���
                                elements[0] = name;//�޸�name
                                
                            }
                            else if (choise == 2) {
                                cout << "�������޸ĺ�ļ۸�";
                                cin >> price;
                                cart.resise_pri(lineIndex, price);//���кź���Ҫ�޸ĵĴ���
                                elements[1] = to_string(price);//�޸�price
                            }
                            else if (choise == 3) {
                                cout << "�������޸ĺ�Ŀ�棺";
                                cin >> num;
                                cart.resise_num(lineIndex, num);//���кź���Ҫ�޸ĵĴ���
                                elements[2] = to_string(num);//�޸�num
                            }
                            for (int i = 0;i < 3;i++) {
                                tempOutput << element[i] << " ";
                            }//ֻ������Ҫ�޸ĵ��Ǹ���������ճ�����
                            }
                        else {//���������Ҫ�������У��ճ���ӽ���ʱ�ļ���
                            tempOutput << line <<endl;
                        }
                        currentLineIndex++;//��������
                    }

                    input.close();
                    tempOutput.close();

                    remove(filename.c_str()); // ɾ��ԭʼ�ļ�
                    rename(tempFilename.c_str(), filename.c_str()); // ��������ʱ�ļ�Ϊԭʼ�ļ�������
                    break;
                }
                case 3: {
                    cout << "ɾ����Ʒ������Ҫɾ����Ʒ��ţ�";
                    cin >> list;
                    cart.removeItem(list);
                    string filename = "mall_item.txt"; // �ļ���
                    string tempFilename = "temp.txt"; // ��ʱ�ļ���
                    int lineIndex; // ��Ҫ�޸ĵ��кţ���0��ʼ
                    ifstream input(filename); // ��Դ�ļ���ȡ����
                    ofstream tempOutput(tempFilename); // ����ʱ�ļ�׼��д������
                    if (!input.is_open() || !tempOutput.is_open()) {
                        cout << "���ļ�ʧ�ܡ�" << endl;
                        return 0;
                    }
                    string line;
                    lineIndex = list - 1;
                    int currentLineIndex = 0;
                    while (getline(input, line)) {//���б���
                        if (currentLineIndex != lineIndex) {
                            tempOutput << line << endl;
                        }
                        currentLineIndex++;
                    }
                    input.close();
                    tempOutput.close();

                    remove(filename.c_str()); // ɾ��ԭʼ�ļ�
                    rename(tempFilename.c_str(), filename.c_str()); // ��������ʱ�ļ�Ϊԭʼ�ļ�������
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
//��ӡ���˵�
         
            cout << "           ==============================================================================================" << endl;
            cout << "                                                        �˵�" << endl;
            cout << "           ==============================================================================================" << endl;
            // �����п�Ͷ��뷽ʽ
            cout << "           | " << setw(4) << left << "ѡ��"
                << "           | " << setw(73) << left << "                                  ����" << " |" << endl;
            cout << "          ----------------------------------------------------------------------------------------------" << endl;
            cout << "           | " << setw(4) << left << " 1"
                << "           | " << setw(73) << left << "ѡ��һ ��¼�˺�" << " |" << endl;

            cout << "           | " << setw(4) << left << " 2"
                << "           | " << setw(73) << left << "ѡ��� ע���˺�" << " |" << endl;
            cout << "           | " << setw(4) << ::left << " 3"
                << "           | " << setw(73) << std::left << "ѡ���� ���빺���̳�" << " |" << endl;
            cout << "           | " << setw(4) << left << " 4"
                << "           | " << setw(73) << left << "ѡ���� �鿴���ﳵ" << " |" << endl;
            cout << "           | " << setw(4) << left << " 5"
                << "           | " << setw(73) << left << "ѡ���� �û�����" << " |" << endl;
            cout << "           | " << setw(4) << left << " 6"
                << "           | " << setw(73) << left << "ѡ���� ��ѯ��Ʒ" << " |" << endl;
            cout << "           =============================================================================================" << endl;
            cout << "            ����������ѡ��";
            cin >> temp;
            system("cls");
            break;
        }
         //��¼����
        case 1: {
            system("cls");
            cout << "��¼��......." << endl;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            int flag = 0;

            //����Ա��¼
            if (username == admin.getUsername() && password == admin.getPassword()) {
                currentUser = &admin;
                cout << "Admin login successful" << endl;
                temp = -2;
            }
            //����user�ļ�����¼�û��˺�
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
                        cout << "�ɹ���¼" << endl;
                        temp = 3;
                        static Customer customer(elements[0], elements[1], elements[2], elements[3]);
                        customers.push_back(customer);
                        currentUser = &customer;
                        filename = currentUser->getUsername() + ".txt";//���û����������ﳵ�ļ�
                        hisname = currentUser->getUsername() + "pur.txt";//���û�������������ʷ�ļ�
                        content.restoreFromDisk(filename);//���ļ����ݵ�������
                        system("cls");
                    }
                }//����û������ļ��У���ת��ע�����
                    if(temp!=3) {
                        char ch;
                        cout << "�Բ���������û��ע�ᡣ������Ҫע���˺�ô����y/n����"<<endl;
                        cin >> ch;
                        if (ch == 'y' || ch == 'Y') {
                            temp = 2;//ע��ѡ��
                            system("cls");//����
                        }
                    }
            }           
            break;
        }//ע�����
        case 2: {
            cout << "ע���˻���......" << endl;
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
                cout << "�޷����ļ�" << endl;
            }
            static Customer customer(username, password, name, address);
            customers.push_back(customer);
            currentUser = &customer; 
            system("cls");
            cout << "��ϲ��ע��ɹ�����";
            temp = 3;//�Զ�ת�������̳ǽ���
           
            break;
        }
              //�����̳�
        case 3: {
            
            char ch;
            string filename = currentUser->getUsername() + ".txt";
            cout << "Welcome, " << static_cast<Customer*>(currentUser)->getName() << "!!!" << endl;
            cout << "Address: " << static_cast<Customer*>(currentUser)->getAddress() << endl;
            cart.printCart();//��ӡ�����̳�����
            cout << "�����Żݴ�٣�ȫ���Ż�" << content.discountRate * 100 << '%' << "�������������Żݾ����ʹ�ã�"
                << "�߹�·������Ҫ�����" << endl;
            string checkpass;
            int a=0;
            cout << "�����������ǵ���Ʒô����y/n���� ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                if (currentUser == &admin) {
                    cout << "Only customers can buy products." << endl;//����Ա�޷�����
                    temp = -2;
                }
                else {     
                    while (1) {
                        if((a++)) cart.printCart();//ֻ�е�һ�ν�����ʱ�򲻴�ӡ��
                        cout << "��������Ʒ��ţ� ";
                        cin >> pnum;
                        if (pnum <= cart.return_num()) {
                            system("cls");
                            cout << "���빺�ﳵ�ɹ�!" << endl;
                            Product a = cart.return_p(pnum - 1);
                            content.saveToDisk(filename, a);//����Ʒ������ļ���
                            content.restoreFromDisk(filename);//�����ļ����ݵ�������
                            content.init_pro();
                           
                            
                            break;
                        }
                        else {
                            system("cls");
                            cout << "�����������,����������" << endl;

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
//�鿴���ﳵ
        case 4: {
            
            if(currentUser== &admin) {
                cout << "Only customers can check their shoppingcar" << endl;
                temp = -2;
            }
            else if (currentUser == NULL) {
                cout << "������˼������û�е�¼��" << endl;
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

                    // ��ʽ��ʱ���Ϊ�ַ���
                    char buffer[80];
                    strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&currentTime));
                    
                    cout << "���ﳵ(" << content.return_num() << ")" << endl;
                    // ��ӡ���ﳵ���ݺ��ܼ�
                    content.printCart();
                    cout << "1.���� 2.�޸� 3.ɾ�� 4.����������:";
                    cin >> t;
                    switch (t) {
                    case 1: {
                        
                        cout << "��������Ʒ��ţ� ";
                        cin >> a;
                        //�������������������
                        
                        while (1) {
                            cout << "�����������˻������Ա�֤����֧���� ";
                            cin >> checkpass;
                            cout << "��������Ҫʹ�õ��Ż�ȯ�(y/n):";
                            cin >> ch;
                            if (ch == 'y' || ch == 'Y') {
                                //�����Ż�ȯ��
                                cout << "Enter your coupon code: ";
                                cin.ignore();
                                getline(cin,inputCouponCode);
                                if (checkpass == currentUser->getPassword()&& inputCouponCode== couponCode) {
                                    content.addPurchaseRecord(content.temp(a - 1), buffer);
                                    double num = content.buysomething(a - 1);
                                    double calnum = content.calculateTotalPriceAfterCoupon(num, inputCouponCode);
                                    cout << "��ϲ��֧���ɹ�����ԭ��Ϊ��" << num << "Ԫ "<<"�ۺ��Ϊ��"<<calnum<<"Ԫ" << endl;
                                    content.savepurchase(hisname, content.return_p(a - 1), buffer);
                                    content.removeItem(a - 1); //�Ƴ����ﳵ���ѹ������Ʒ
                                    break;
                                }
                                else if(checkpass != currentUser->getPassword()&& inputCouponCode == couponCode) {
                                    cout << "����������������롣" << endl;
                                }
                                else if (inputCouponCode != couponCode&& checkpass == currentUser->getPassword()) {
                                    cout << "�Ż���������������롣" << endl;
                                }
                            }
                            else {
                                if (checkpass == currentUser->getPassword()) {
                                    content.addPurchaseRecord(content.temp(a - 1), buffer);
                                    cout << "��ϲ��֧���ɹ�����֧����Ϊ��" << content.buysomething(a - 1) << "Ԫ"<< endl;
                                    content.savepurchase(hisname, content.return_p(a - 1), buffer);
                                    content.removeItem(a - 1);
                                    
                                    break;
                                }
                                else {
                                    cout << "����������������롣" << endl;
                                }
                            }
                            
                        }
                        break;}
                    case 2: {
                        cout << "�޸���Ʒ���Ϊ��";
                        cin >> a;
                        cout << "��������Ϊ�� ";
                        cin >> num;
                        content.revise_item(a - 1, num);

                        string tempFilename = "temp.txt"; // ��ʱ�ļ���
                        int lineIndex; // ��Ҫ�޸ĵ��кţ���0��ʼ
                        int elementIndex; // ��Ҫ�޸ĵ�Ԫ�������е�λ�ã���0��ʼ

                        ifstream input(filename); // ��Դ�ļ���ȡ����
                        ofstream tempOutput(tempFilename); // ����ʱ�ļ�׼��д������
                        if (!input.is_open() || !tempOutput.is_open()) {
                            cout << "���ļ�ʧ�ܡ�" << endl;
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

                        remove(filename.c_str()); // ɾ��ԭʼ�ļ�
                        rename(tempFilename.c_str(), filename.c_str()); // ��������ʱ�ļ�Ϊԭʼ�ļ�������

                        
                        system("cls");
                        cout << "ҳ��ˢ���У��������µĹ��ﳵ��" << endl;
                        cout << "���ﳵ(" << content.return_num() << ")" << endl;
                        content.printCart();
                        break;
                    }
                    case 3: {
                        cout << "�Ƴ���Ʒ���Ϊ��";
                        cin >> a;
                        content.removeItem(a - 1);

                        string tempFilename = "temp.txt"; // ��ʱ�ļ���
                        int lineIndex; // ��Ҫ�޸ĵ��кţ���0��ʼ
                        ifstream input(filename); // ��Դ�ļ���ȡ����
                        ofstream tempOutput(tempFilename); // ����ʱ�ļ�׼��д������
                        if (!input.is_open() || !tempOutput.is_open()) {
                            cout << "���ļ�ʧ�ܡ�" << endl;
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
                        
                        remove(filename.c_str()); // ɾ��ԭʼ�ļ�
                        rename(tempFilename.c_str(), filename.c_str()); // ��������ʱ�ļ�Ϊԭʼ�ļ�������
                        
                        system("cls");
                        cout << "ҳ��ˢ���У��������µĹ��ﳵ��" << endl;
                        cout << "���ﳵ(" << content.return_num() << ")" << endl;
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
              //�û����������޸��û�����
        case 5: {
            string newPassword;
            int choice;
            cout << "1.�޸Ĺ������� 2.�鿴������ʷ 3.��ȡ�������ݷ��� 4.�������˵�: ";
            
            if (currentUser == NULL) {
                cout << "������˼������û�е�¼��" << endl;
                temp = 1;
                break;
            }
            else {
                cin >> choice;
            }
            switch (choice) {
            case 1: {
                
                /*�޸Ĺ˿�����*/
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
                string tempFilename = "temp.txt"; // ��ʱ�ļ���
                ifstream input(filename); // ��Դ�ļ���ȡ����
                ofstream tempOutput(tempFilename); // ����ʱ�ļ�׼��д������

                if (!input.is_open() || !tempOutput.is_open()) {
                    cout << "���ļ�ʧ�ܡ�" << endl;
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
                tempOutput << input.rdbuf();//ͨ������rdbuf()�������Ի��streambuf��ָ�룬
                //��������ֱ�ӶԻ��������в��������ȡ��д�����ݣ���תλ�õ�
                input.close();
                tempOutput.close();
                remove(filename.c_str()); // ɾ��ԭʼ�ļ�
                rename(tempFilename.c_str(), filename.c_str()); // ��������ʱ�ļ�Ϊԭʼ�ļ�������
                break;
            }
            case 2: {//չʾ������ʷ
                content.restorepurchase(hisname);
                content.displayPurchaseHistory();
                break;
            }
            case 3: {//��ѯһ��ʱ���µĹ������ݣ�������
                string time;
                content.restorepurchase(hisname);
                cout << "�����������ѯ������,��ʽΪ��-��-��: " ;
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
            // ��ѯ��Ʒ��Ϣ
            cout << "Enter the name of the product to search: ";
            cin >> searchName;
            falg=cart.searchProductByName(searchName);
            if (!falg) {//��ѯ�ؼ���Ϣ
                cout << "Enter a keyword for product recommendation: ";
                cin >> recommendKeyword;
                cart.recommendProductByKeyword(recommendKeyword);
            }
            break;
            }
        default: {
            cout << "�����������������"<<endl;
            temp = -1;
        }
        }
        }
    

    return 0;
}