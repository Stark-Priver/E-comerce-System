#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class User {
protected:
    string username;
    string password;

public:
    User(string uname = "", string pass = "") : username(uname), password(pass) {}
    virtual void login() = 0; // Pure virtual function for role-specific login
};

class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass) {}

    void login() override {
        cout << "Admin login successful!\n";
    }

    void manageProducts() {
        cout << "Managing products...\n";
        // Add product management logic here
    }

    void viewOrders() {
        cout << "Viewing orders...\n";
        // Add order viewing logic here
    }
};

class Customer : public User {
    vector<string> cart;

public:
    Customer(string uname, string pass) : User(uname, pass) {}

    void login() override {
        cout << "Customer login successful!\n";
    }

    void browseProducts() {
        cout << "Browsing products...\n";
        // Add product browsing logic here
    }

    void addToCart(string product) {
        cart.push_back(product);
        cout << product << " added to cart!\n";
    }

    void checkout() {
        cout << "Checking out...\n";
        // Add checkout logic here
    }
};

class Product {
    string name;
    double price;
    int stock;

public:
    Product(string pname = "", double pprice = 0.0, int pstock = 0)
        : name(pname), price(pprice), stock(pstock) {}

    void displayProduct() const {
        cout << "Product: " << name << ", Price: $" << price
             << ", Stock: " << stock << endl;
    }
};

class Order {
    string customerName;
    vector<string> products;

public:
    Order(string cname) : customerName(cname) {}

    void addProduct(string product) {
        products.push_back(product);
    }

    void displayOrder() const {
        cout << "Order for " << customerName << ":\n";
        for (const string &product : products) {
            cout << "- " << product << endl;
        }
    }
};

int main() {
    Admin admin("admin", "1234");
    Customer customer("john_doe", "password");

    Product product1("Laptop", 1200.50, 10);
    Product product2("Phone", 800.99, 20);

    cout << "E-Commerce System\n";

    admin.login();
    admin.manageProducts();

    customer.login();
    customer.browseProducts();
    customer.addToCart("Laptop");
    customer.checkout();

    return 0;
}
