#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

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
        }
    }
};

// Order Class
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
        for (const string& product : products) {
            cout << "- " << product << endl;
        }
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
    vector<string> cart;
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

    void addToCart(string product) {
        cart.push_back(product);
        cout << product << " added to cart!\n";
    }

    void checkout(vector<Order>& orders) {
        if (cart.empty()) {
            cout << "Your cart is empty!\n";
            return;
        }

        Order newOrder(username);
        for (const auto& product : cart) {
            newOrder.addProduct(product);
        }
        orders.push_back(newOrder);
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

// Helper function to clear the input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Main Function
int main() {
    vector<Product> catalog;
    vector<Order> orders;
    Admin admin("admin", "1234");
    Customer customer("john_doe", "password");

    bool running = true;
    while (running) {
        cout << "\nE-Commerce System Menu:\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Upload Products (Admin)\n";
        cout << "4. Browse Products (Customer)\n";
        cout << "5. Add Product to Cart (Customer)\n";
        cout << "6. Checkout (Customer)\n";
        cout << "7. Save Account (Customer)\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter Admin Username: ";
                getline(cin, username);
                cout << "Enter Admin Password: ";
                getline(cin, password);
                if (username == admin.getUsername() && password == admin.getPassword()) {
                    admin.login();
                } else {
                    cout << "Invalid Admin credentials.\n";
                }
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter Customer Username: ";
                getline(cin, username);
                cout << "Enter Customer Password: ";
                getline(cin, password);
                if (username == customer.getUsername() && password == customer.getPassword()) {
                    customer.login();
                } else {
                    cout << "Invalid Customer credentials.\n";
                }
                break;
            }
            case 3: {
                string filename;
                cout << "Enter the path of the CSV file to upload products: ";
                getline(cin, filename);
                admin.uploadProductsFromCSV(catalog, filename);
                break;
            }
            case 4: {
                customer.browseProducts(catalog);
                break;
            }
            case 5: {
                string productName;
                cout << "Enter the name of the product to add to cart: ";
                getline(cin, productName);
                customer.addToCart(productName);
                break;
            }
            case 6: {
                customer.checkout(orders);
                break;
            }
            case 7: {
                customer.saveAccountToFile("accounts.txt");
                break;
            }
            case 8:
                running = false;
                cout << "Exiting the E-Commerce System.\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
                break;
        }
    }

    // Display orders after exiting
    cout << "\nAll Orders Placed:\n";
    for (const auto& order : orders) {
        order.displayOrder();
    }

    return 0;
}
