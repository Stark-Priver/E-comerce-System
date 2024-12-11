#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> // For search functionality
using namespace std;

// Base User Class
class User {
protected:
    string username;
    string password;

public:
    User(string uname = "", string pass = "") : username(uname), password(pass) {}
    virtual void login() = 0; // Pure virtual function for role-specific login
};

// Admin Class
class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass) {}

    void login() override {
        cout << "Admin login successful!\n";
    }

    void manageProducts(vector<class Product>& catalog) {
        cout << "Managing products...\n";
        // Product management logic (Add, Edit, Delete products)
    }

    void viewOrders(vector<class Order>& orders) {
        cout << "Viewing all orders:\n";
        for (const auto& order : orders) {
            order.displayOrder();
        }
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

    void browseProducts(const vector<class Product>& catalog) {
        cout << "Product Catalog:\n";
        for (const auto& product : catalog) {
            product.displayProduct();
        }
    }

    void addToCart(string product) {
        cart.push_back(product);
        cout << product << " added to cart!\n";
    }

    void checkout(vector<class Order>& orders) {
        cout << "Checking out...\n";
        if (cart.empty()) {
            cout << "Your cart is empty!\n";
            return;
        }

        Order newOrder(username);
        for (const auto& product : cart) {
            newOrder.addProduct(product);
        }
        orders.push_back(newOrder);
        orderHistory.push_back("Order placed");
        cart.clear();
        cout << "Order placed successfully!\n";
    }

    void viewOrderHistory() const {
        cout << "Order History:\n";
        for (const auto& order : orderHistory) {
            cout << "- " << order << endl;
        }
    }
};

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

// Utility Functions
void searchProducts(const vector<Product>& catalog, const string& query) {
    cout << "Search Results for \"" << query << "\":\n";
    for (const auto& product : catalog) {
        if (product.getName().find(query) != string::npos) {
            product.displayProduct();
        }
    }
}

// Main Function
int main() {
    vector<Product> catalog = {
        Product("Laptop", 1200.50, 10),
        Product("Phone", 800.99, 20),
        Product("Headphones", 150.75, 50)
    };

    vector<Order> orders;

    Admin admin("admin", "1234");
    Customer customer("john_doe", "password");

    cout << "E-Commerce System\n";

    // Admin operations
    admin.login();
    admin.manageProducts(catalog);
    admin.viewOrders(orders);

    // Customer operations
    customer.login();
    customer.browseProducts(catalog);
    customer.addToCart("Laptop");
    customer.checkout(orders);
    customer.viewOrderHistory();

    // Searching products
    string searchQuery;
    cout << "\nEnter product name to search: ";
    cin >> searchQuery;
    searchProducts(catalog, searchQuery);

    return 0;
}
