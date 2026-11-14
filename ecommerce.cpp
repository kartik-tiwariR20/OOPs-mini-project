#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Product {
protected:
    int id, stock;
    string name;
    double price;
public:
    Product(int i, string n, double p, int s) : id(i), stock(s), name(n), price(p) {}
    virtual void display() const {
        cout << left << setw(6) << id << setw(22) << name
             << "Rs." << setw(9) << fixed << setprecision(2) << price
             << "Stock: " << stock << endl;
    }
    virtual double getPrice() const { return price; }
    int getId() const { return id; }
    string getName() const { return name; }
    bool available() const { return stock > 0; }
    void reduceStock() { if (stock > 0) --stock; }
    virtual ~Product() {}
};

class Electronics : public Product {
public:
    Electronics(int i, string n, double p, int s) : Product(i, n, p, s) {}
    void display() const override {
        cout << "[Electronics] ";
        Product::display();
    }
};

class Clothing : public Product {
public:
    Clothing(int i, string n, double p, int s) : Product(i, n, p, s) {}
    void display() const override {
        cout << "[Clothing] ";
        Product::display();
    }
};

class Customer {
private:
    int id;
    string name, email;
public:
    Customer(int i, string n, string e) : id(i), name(n), email(e) {}
    void display() const {
        cout << "Customer: " << name << " | Email: " << email << endl;
    }
};

class Cart {
private:
    vector<Product*> items;
public:
    void add(Product* p) {
        if (p && p->available()) {
            items.push_back(p);
            p->reduceStock();
            cout << "Product added to cart.\n";
        }
        else cout << "Product unavailable.\n";
    }
    double total() const {
        double t = 0;
        for (auto p : items) t += p->getPrice();
        return t;
    }
    void show() const {
        if (items.empty()) { cout << "Cart is empty.\n"; return; }
        cout << "\n--- CART ---\n";
        for (auto p : items) cout << p->getName() << " Rs." << p->getPrice() << endl;
        cout << "Total: Rs." << fixed << setprecision(2) << total() << endl;
    }
    bool empty() const { return items.empty(); }
};

class Order {
private:
    int orderId;
    Customer customer;
    double amount;
public:
    Order(int id, const Customer& c, double a) : orderId(id), customer(c), amount(a) {}
    void confirm() const {
        cout << "\n--- ORDER CONFIRMED ---\n";
        cout << "Order ID: " << orderId << " | Amount: Rs."
             << fixed << setprecision(2) << amount << endl;
        customer.display();
    }
    void save() const {
        ofstream file("orders.txt", ios::app);
        file << "Order " << orderId << " | Amount Rs." << amount << endl;
    }
};

int main() {
    vector<Product*> products = {
        new Electronics(101, "Wireless Mouse", 799, 10),
        new Electronics(102, "Keyboard", 1299, 8),
        new Clothing(201, "T-Shirt", 599, 15),
        new Clothing(202, "Hoodie", 1199, 7)
    };
    Customer customer(1, "Demo Customer", "customer@example.com");
    Cart cart;
    int choice, id;
    do {
        cout << "\n===== E-COMMERCE MANAGEMENT SYSTEM =====\n"
             << "1. Display Products\n2. Add to Cart\n"
             << "3. View Cart\n4. Place Order\n5. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            cout << "\nID Product Price Stock\n";
            for (auto p : products) p->display(); // polymorphism
        } else if (choice == 2) {
            cout << "Enter Product ID: ";
            cin >> id;
            bool found = false;
            for (auto p : products)
                if (p->getId() == id) { cart.add(p); found = true; break; }
            if (!found) cout << "Product not found.\n";
        } else if (choice == 3) cart.show();
        else if (choice == 4) {
            if (cart.empty()) cout << "Cart is empty.\n";
            else {
                Order order(1001, customer, cart.total());
                order.confirm();
                order.save();
                cout << "Order saved to orders.txt.\n";
            }
        }
    } while (choice != 5);
    for (auto p : products) delete p; // destructor
    cout << "Thank you for using the system.\n";
    return 0;
}
