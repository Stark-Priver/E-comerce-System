#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// Forward declarations
class Product;
class Order;

// Product Class
class Product {
    string name;
    double price;
    int stock;

public:
    Product(string pname = "", double pprice = 0.0, int pstock = 0)
        : name(pname), price(pprice), stock(pstock) {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    void displayProduct() const {
        cout << "Product: " << name << ", Price: $" << price
             << ", Stock: " << stock << endl;
    }

    void reduceStock() {
        if (stock > 0) {
            stock--;
        } else {
            cout << "Out of stock for " << name << "!\n";
        }
    }
};

// Order Class
class Order {
    string customerName;
    vector<Product> products;

public:
    Order(string cname) : customerName(cname) {}

    void addProduct(const Product& product) {
        products.push_back(product);
    }

    void displayOrder() const {
        cout << "Order for " << customerName << ":\n";
        for (const auto& product : products) {
            cout << "- " << product.getName() << " (Price: $" << product.getPrice() << ")\n";
        }
    }

    void saveOrderToFile() const {
        ofstream file("orders.txt", ios::app);
        if (!file.is_open()) {
            cout << "Failed to open file for saving order!\n";
            return;
        }

        file << "Order for " << customerName << ":\n";
        for (const auto& product : products) {
            file << "- " << product.getName() << " (Price: $" << product.getPrice() << ")\n";
        }
        file.close();
    }
};

// Base User Class
class User {
protected:
    string username;
    string password;

public:
    User(string uname = "", string pass = "") : username(uname), password(pass) {}
    virtual void login() = 0; // Pure virtual function for role-specific login

    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

// Admin Class
class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass) {}

    void login() override {
        cout << "Admin login successful!\n";
    }

    void uploadProductsFromCSV(vector<Product>& catalog, const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open CSV file: " << filename << "\n";
            return;
        }

        string line, name;
        double price;
        int stock;

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, name, ',');
            if (!(ss >> price)) {
                cout << "Invalid format for price in line: " << line << "\n";
                continue;
            }
            ss.ignore(); // Ignore the comma
            if (!(ss >> stock)) {
                cout << "Invalid format for stock in line: " << line << "\n";
                continue;
            }

            catalog.push_back(Product(name, price, stock));
        }

        file.close();
        cout << "Products uploaded successfully from " << filename << "!\n";
    }
};

// Customer Class
class Customer : public User {
    vector<Product> cart;
    vector<string> orderHistory;

public:
    Customer(string uname, string pass) : User(uname, pass) {}

    void login() override {
        cout << "Customer login successful!\n";
    }

    void browseProducts(const vector<Product>& catalog) {
        cout << "Product Catalog:\n";
        for (const auto& product : catalog) {
            product.displayProduct();
        }
    }

    void addToCart(Product product) {
        cart.push_back(product);
        cout << product.getName() << " added to cart!\n";
    }

    void checkout(vector<Order>& orders) {
        if (cart.empty()) {
            cout << "Your cart is empty!\n";
            return;
        }

        Order newOrder(username);
        for (const auto& product : cart) {
            newOrder.addProduct(product);
            product.reduceStock();  // Reduce stock for each product in the cart
        }

        orders.push_back(newOrder);
        newOrder.saveOrderToFile();  // Save the order to file
        cart.clear();
        cout << "Order placed successfully!\n";
    }

    void saveAccountToFile(const string& filename) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cout << "Failed to open file for saving user account: " << filename << "\n";
            return;
        }

        file << username << "," << password << endl;
        file.close();
        cout << "Account saved to " << filename << "!\n";
    }
};

// Main Function
int main() {
    vector<Product> catalog;
    vector<Order> orders;

    Admin admin("admin", "1234");
    Customer customer("john_doe", "password");

    cout << "E-Commerce System\n";

    // Admin operations
    admin.login();
    admin.uploadProductsFromCSV(catalog, "products.csv"); // Replace with actual CSV file path

    // Customer operations
    customer.login();
    customer.saveAccountToFile("accounts.txt"); // Save customer account details
    customer.browseProducts(catalog);
    customer.addToCart(catalog[0]);  // Example: Adding first product in catalog
    customer.addToCart(catalog[1]);  // Example: Adding second product in catalog
    customer.checkout(orders);

    // Display orders
    for (const auto& order : orders) {
        order.displayOrder();
    }

    return 0;
}
