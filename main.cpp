#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

// Function to clear input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


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

    // Getter for saving products to CSV
    string toCSV() const {
        return name + "," + to_string(price) + "," + to_string(stock);
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
    virtual void saveCredentials(const string& filename) const {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cout << "Failed to open file for saving user credentials.\n";
            return;
        }
        file << username << "," << password << endl;
        file.close();
    }
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

    // Save the product catalog to CSV
    void saveProductsToCSV(const vector<Product>& catalog, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open CSV file: " << filename << "\n";
            return;
        }

        for (const auto& product : catalog) {
            file << product.toCSV() << endl;
        }

        file.close();
        cout << "Product catalog saved to " << filename << "!\n";
    }

    void addProduct(vector<Product>& catalog) {
        string name;
        double price;
        int stock;
        cout << "Enter product name: ";
        getline(cin, name);
        cout << "Enter product price: ";
        cin >> price;
        cout << "Enter product stock: ";
        cin >> stock;
        clearInputBuffer();

        catalog.push_back(Product(name, price, stock));
        cout << "Product added successfully.\n";
    }

private:
    // Helper function to clear the input buffer
    void clearInputBuffer() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

    bool verifyCredentials(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open credentials file: " << filename << "\n";
            return false;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string savedUsername, savedPassword;
            getline(ss, savedUsername, ',');
            getline(ss, savedPassword);

            if (savedUsername == username && savedPassword == password) {
                return true;
            }
        }

        return false;
    }

    bool registerUser(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open credentials file: " << filename << "\n";
            return false;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string savedUsername;
            getline(ss, savedUsername, ',');

            if (savedUsername == username) {
                cout << "Username already exists! Please try again with a different username.\n";
                return false;
            }
        }

        file.close();
        saveAccountToFile(filename);
        cout << "Registration successful!\n";
        return true;
    }
};

// Main Function
int main() {
    vector<Product> catalog;
    vector<Order> orders;
    Admin admin("admin", "1234");
    Customer customer("john_doe", "password");

    bool adminLoggedIn = false;
    bool customerLoggedIn = false;
    bool running = true;
    const string credentialsFile = "accounts.txt";
    const string productCSVFile = "products.csv";

    while (running) {
        cout << "\nE-Commerce System Menu:\n";
        if (!adminLoggedIn && !customerLoggedIn) {
            cout << "1. Admin Login\n";
            cout << "2. Customer Login\n";
            cout << "3. Register as Customer\n";
            cout << "4. Exit\n";
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
                        adminLoggedIn = true;
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
                    customer = Customer(username, password);

                    if (customer.verifyCredentials(credentialsFile)) {
                        customer.login();
                        customerLoggedIn = true;
                    } else {
                        cout << "Invalid Customer credentials.\n";
                    }
                    break;
                }
                case 3: {
                    string username, password;
                    cout << "Enter new Customer Username: ";
                    getline(cin, username);
                    cout << "Enter new Customer Password: ";
                    getline(cin, password);
                    customer = Customer(username, password);

                    if (!customer.registerUser(credentialsFile)) {
                        cout << "Registration failed.\n";
                    } else {
                        customerLoggedIn = true;
                    }
                    break;
                }
                case 4:
                    running = false;
                    cout << "Exiting the E-Commerce System.\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    break;
            }
        } else {
            // Logged-in user menu
            if (adminLoggedIn) {
                cout << "\nAdmin Menu:\n";
                cout << "1. Add Product\n";
                cout << "2. Upload Products from CSV\n";
                cout << "3. Save Product Catalog to CSV\n";
                cout << "4. Log Out (Admin)\n";
                cout << "Enter your choice: ";

                int choice;
                cin >> choice;
                clearInputBuffer();

                switch (choice) {
                    case 1:
                        admin.addProduct(catalog);
                        break;
                    case 2:
                        admin.uploadProductsFromCSV(catalog, productCSVFile);
                        break;
                    case 3:
                        admin.saveProductsToCSV(catalog, productCSVFile);
                        break;
                    case 4:
                        adminLoggedIn = false;
                        cout << "Admin logged out.\n";
                        break;
                    default:
                        cout << "Invalid choice! Please try again.\n";
                        break;
                }
            }

            if (customerLoggedIn) {
                cout << "\nCustomer Menu:\n";
                cout << "1. Browse Products\n";
                cout << "2. Add to Cart\n";
                cout << "3. Checkout\n";
                cout << "4. Log Out (Customer)\n";
                cout << "Enter your choice: ";

                int choice;
                cin >> choice;
                clearInputBuffer();

                switch (choice) {
                    case 1:
                        customer.browseProducts(catalog);
                        break;
                    case 2: {
                        string productName;
                        cout << "Enter product name to add to cart: ";
                        getline(cin, productName);
                        customer.addToCart(productName);
                        break;
                    }
                    case 3:
                        customer.checkout(orders);
                        break;
                    case 4:
                        customerLoggedIn = false;
                        cout << "Customer logged out.\n";
                        break;
                    default:
                        cout << "Invalid choice! Please try again.\n";
                        break;
                }
            }
        }
    }

    return 0;
}
