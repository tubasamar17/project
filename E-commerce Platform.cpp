#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// --------------------- Custom Stack ---------------------
template <typename T>
class MyStack {
private:
    T* arr;
    int topIndex;
    int capacity;

public:
    MyStack(int cap = 100) {
        capacity = cap;
        arr = new T[capacity];
        topIndex = -1;
    }

    void push(T dataItem) {
        if (topIndex == capacity - 1) return;
        arr[++topIndex] = dataItem;
    }

    void pop() {
        if (topIndex == -1) return;
        topIndex--;
    }

    T top() {
        return arr[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    ~MyStack() {
        delete[] arr;
    }
};

// --------------------- Custom Queue ---------------------
template <typename T>
class MyQueue {
private:
    T* arr;
    int frontIndex;
    int rearIndex;
    int capacity;
    int count;

public:
    MyQueue(int cap = 100) {
        capacity = cap;
        arr = new T[capacity];
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    void push(T dataItem) {
        if (count == capacity) return;
        rearIndex = (rearIndex + 1) % capacity;
        arr[rearIndex] = dataItem;
        count++;
    }

    void pop() {
        if (count == 0) return;
        frontIndex = (frontIndex + 1) % capacity;
        count--;
    }

    T front() {
        return arr[frontIndex];
    }

    bool isEmpty() {
        return count == 0;
    }

    ~MyQueue() {
        delete[] arr;
    }
};

// --------------------- Product Class ---------------------
class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int stock;

public:
    Product(int productId, string productName, string productCategory, double productPrice, int productStock) {
        id = productId;
        name = productName;
        category = productCategory;
        price = productPrice;
        stock = productStock;
    }

    int getId() { return id; }
    string getName() { return name; }
    string getCategory() { return category; }
    double getPrice() { return price; }
    int getStock() { return stock; }
    void setStock(int newStock) { stock = newStock; }

    void display() {
        cout << "ID: " << id
             << " Name: " << name
             << " Category: " << category
             << " Price: $" << price
             << " Stock: " << stock << endl;
    }
};

// --------------------- CartItem Node ---------------------
struct CartItem {
    Product* product;
    int quantity;
    CartItem* next;
    CartItem(Product* productItem, int quantityItem)
        : product(productItem), quantity(quantityItem), next(NULL) {}
};

// --------------------- Custom Linked List ---------------------
class MyLinkedList {
private:
    CartItem* head;

public:
    MyLinkedList() { head = NULL; }

    void add(CartItem* newItem) {
        CartItem* temp = head;
        while (temp != NULL) {
            if (temp->product->getId() == newItem->product->getId()) {
                temp->quantity += newItem->quantity;
                return;
            }
            temp = temp->next;
        }

        if (head == NULL) {
            head = newItem;
        } else {
            temp = head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newItem;
        }
    }

    void remove(int productId, int quantityToRemove) {
        CartItem* temp = head;
        CartItem* previous = NULL;

        while (temp != NULL) {
            if (temp->product->getId() == productId) {
                if (quantityToRemove >= temp->quantity) {
                    if (previous == NULL) head = temp->next;
                    else previous->next = temp->next;
                    delete temp;
                } else {
                    temp->quantity -= quantityToRemove;
                }
                return;
            }
            previous = temp;
            temp = temp->next;
        }
    }

    void display() {
        if (head == NULL) {
            cout << "Cart is empty." << endl;
            return;
        }

        CartItem* temp = head;
        cout << "Current Cart:" << endl;
        while (temp != NULL) {
            cout << temp->product->getName() << " x" << temp->quantity
                 << " - $" << temp->product->getPrice()
                 << " each, Total: $" << temp->quantity * temp->product->getPrice()
                 << endl;
            temp = temp->next;
        }
    }

    CartItem* getHead() { return head; }

    void clear() {
        CartItem* temp = head;
        while (temp != NULL) {
            CartItem* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        head = NULL;
    }
};

// --------------------- User Class ---------------------
class User {
private:
    string name;
    string email;
    string password;

public:
    User(string userName, string userEmail, string userPassword) {
        name = userName;
        email = userEmail;
        password = userPassword;
    }

    string getEmail() { return email; }
    string getName() { return name; }

    bool login(string loginEmail, string loginPassword) {
        return (email == loginEmail && password == loginPassword);
    }
};

// --------------------- CartAction ---------------------
struct CartAction {
    string actionType; // "add" or "remove"
    Product* product;
    int quantity;

    CartAction() {
        actionType = "";
        product = nullptr;
        quantity = 0;
    }

    CartAction(string type, Product* productItem, int quantityItem)
        : actionType(type), product(productItem), quantity(quantityItem) {}
};

// --------------------- Cart Class ---------------------
class Cart {
private:
    MyLinkedList cartItems;
    MyStack<CartAction> undoStack;

public:
    void addToCart(Product* product, int quantityToAdd) {
        if (quantityToAdd > product->getStock()) {
            cout << "Not enough stock. Available: " << product->getStock() << endl;
            return;
        }
        CartItem* newItem = new CartItem(product, quantityToAdd);
        cartItems.add(newItem);
        undoStack.push(CartAction("add", product, quantityToAdd));
        cout << "Added " << product->getName() << " x" << quantityToAdd << " to cart." << endl;
    }

    void removeFromCart(Product* product, int quantityToRemove) {
        undoStack.push(CartAction("remove", product, quantityToRemove));
        cartItems.remove(product->getId(), quantityToRemove);
        cout << "Removed " << product->getName() << " x" << quantityToRemove << " from cart." << endl;
    }

    void undoLastAction() {
        if (undoStack.isEmpty()) {
            cout << "Nothing to undo." << endl;
            return;
        }
        CartAction lastAction = undoStack.top();
        undoStack.pop();

        if (lastAction.actionType == "add") {
            cartItems.remove(lastAction.product->getId(), lastAction.quantity);
        } else if (lastAction.actionType == "remove") {
            cartItems.add(new CartItem(lastAction.product, lastAction.quantity));
        }

        cout << "Undo action performed." << endl;
    }

    void displayCart() { cartItems.display(); }
    MyLinkedList& getItems() { return cartItems; }
    void clear() { cartItems.clear(); }
};

// --------------------- Order Class ---------------------
class Order {
private:
    int orderId;
    MyLinkedList items;
    double totalPrice;

public:
    Order() {
        orderId = 0;
        totalPrice = 0.0;
    }

    Order(int newOrderId, MyLinkedList& cartItems) {
        orderId = newOrderId;
        totalPrice = 0.0;

        CartItem* temp = cartItems.getHead();
        while (temp != NULL) {
            items.add(new CartItem(temp->product, temp->quantity));
            totalPrice += temp->product->getPrice() * temp->quantity;
            temp = temp->next;
        }
    }

    void display() {
        cout << "Order ID: " << orderId << " Total: $" << totalPrice << endl;
        CartItem* temp = items.getHead();
        while (temp != NULL) {
            cout << " - " << temp->product->getName()
                 << " x" << temp->quantity
                 << " - $" << temp->product->getPrice() << endl;
            temp = temp->next;
        }
    }
};

// --------------------- Helper Function ---------------------
bool isEmailUnique(vector<User>& users, string emailToCheck) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].getEmail() == emailToCheck) return false;
    }
    return true;
}

// --------------------- Safe Input Functions ---------------------
int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input, please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input, please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// --------------------- Main Program ---------------------
int main() {
    vector<Product> products;
    products.push_back(Product(101, "Laptop", "Electronics", 1000, 5));
    products.push_back(Product(102, "Phone", "Electronics", 500, 10));
    products.push_back(Product(103, "Shoes", "Clothing", 80, 20));
    products.push_back(Product(104, "Bags", "Assessories", 1700, 6));

    vector<User> users;
    users.push_back(User("Admin", "admin", "admin"));

    Cart cart;
    MyQueue<Order> orders;
    int orderCounter = 1;

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n--- Welcome to MyShop ---" << endl;
        cout << "1. Register\n2. Login\n0. Exit\n";
        mainChoice = getIntInput("Choice: ");

        switch (mainChoice) {
            case 1: {
                string userName, userEmail, userPassword;
                cout << "Enter Name: "; getline(cin, userName);
                cout << "Enter Email: "; getline(cin, userEmail);
                cout << "Enter Password: "; getline(cin, userPassword);

                if (!isEmailUnique(users, userEmail)) {
                    cout << "Email already registered." << endl;
                } else {
                    users.push_back(User(userName, userEmail, userPassword));
                    cout << "Registration successful!" << endl;
                }
                break;
            }

            case 2: {
                string loginEmail, loginPassword;
                cout << "Enter Email: "; getline(cin, loginEmail);
                cout << "Enter Password: "; getline(cin, loginPassword);

                bool loginSuccess = false;
                bool isAdmin = false;
                User* currentUser = nullptr;

                for (int i = 0; i < users.size(); i++) {
                    if (users[i].login(loginEmail, loginPassword)) {
                        loginSuccess = true;
                        currentUser = &users[i];
                        if (loginEmail == "admin") isAdmin = true;
                        break;
                    }
                }

                if (!loginSuccess) {
                    cout << "Login failed." << endl;
                    break;
                }

                cout << "Login successful!" << endl;

                // --------------------- Admin Menu ---------------------
                if (isAdmin) {
                    int adminChoice = -1;
                    while (adminChoice != 0) {
                        cout << "\n--- Admin Menu ---" << endl;
                        cout << "1. Display Products\n2. Add Product\n3. Update Stock\n4. Remove Product\n5. View Orders\n0. Logout\n";
                        adminChoice = getIntInput("Choice: ");

                        switch (adminChoice) {
                            case 1:
                                for (int i = 0; i < products.size(); i++) products[i].display();
                                break;

                            case 2: {
                                int pid = getIntInput("Enter Product ID: ");
                                string name, category;
                                double price = getDoubleInput("Enter Price: ");
                                int stock = getIntInput("Enter Stock: ");
                                cin.ignore();
                                cout << "Enter Name: "; getline(cin, name);
                                cout << "Enter Category: "; getline(cin, category);
                                products.push_back(Product(pid, name, category, price, stock));
                                cout << "Product added." << endl;
                                break;
                            }

                            case 3: {
                                int pid = getIntInput("Enter Product ID: ");
                                int newStock = getIntInput("Enter new stock: ");
                                bool found = false;
                                for (int i = 0; i < products.size(); i++) {
                                    if (products[i].getId() == pid) {
                                        products[i].setStock(newStock);
                                        found = true;
                                        cout << "Stock updated." << endl;
                                        break;
                                    }
                                }
                                if (!found) cout << "Product not found." << endl;
                                break;
                            }

                            case 4: {
                                int pid = getIntInput("Enter Product ID: ");
                                bool found = false;
                                for (int i = 0; i < products.size(); i++) {
                                    if (products[i].getId() == pid) {
                                        products.erase(products.begin() + i);
                                        found = true;
                                        cout << "Product removed." << endl;
                                        break;
                                    }
                                }
                                if (!found) cout << "Product not found." << endl;
                                break;
                            }

                            case 5: {
                                if (orders.isEmpty()) cout << "No orders yet." << endl;
                                else {
                                    MyQueue<Order> tempQueue = orders;
                                    while (!tempQueue.isEmpty()) {
                                        tempQueue.front().display();
                                        tempQueue.pop();
                                    }
                                }
                                break;
                            }

                            case 0: cout << "Admin logged out." << endl; break;
                            default: cout << "Invalid choice." << endl; break;
                        }
                    }
                } 
                // --------------------- User Menu ---------------------
                else {
                    int userChoice = -1;
                    while (userChoice != 0) {
                        cout << "\n--- User Menu ---" << endl;
                        cout << "1. Display Products\n2. Add to Cart\n3. Remove from Cart\n4. Undo\n5. View Cart\n6. Checkout\n7. View Orders\n0. Logout\n";
                        userChoice = getIntInput("Choice: ");

                        switch (userChoice) {
                            case 1:
                                for (int i = 0; i < products.size(); i++) products[i].display();
                                break;

                            case 2: {
                                int pid = getIntInput("Enter Product ID: ");
                                int qty = getIntInput("Enter Quantity: ");
                                bool found = false;
                                for (int i = 0; i < products.size(); i++) {
                                    if (products[i].getId() == pid) {
                                        cart.addToCart(&products[i], qty);
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found) cout << "Product not found." << endl;
                                break;
                            }

                            case 3: {
                                int pid = getIntInput("Enter Product ID: ");
                                int qty = getIntInput("Enter Quantity: ");
                                bool found = false;
                                for (int i = 0; i < products.size(); i++) {
                                    if (products[i].getId() == pid) {
                                        cart.removeFromCart(&products[i], qty);
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found) cout << "Product not found in cart." << endl;
                                break;
                            }

                            case 4: cart.undoLastAction(); break;
                            case 5: cart.displayCart(); break;

                            case 6: {
                                if (cart.getItems().getHead() == NULL) {
                                    cout << "Cart empty." << endl;
                                    break;
                                }
                                CartItem* tempItem = cart.getItems().getHead();
                                while (tempItem != NULL) {
                                    for (int i = 0; i < products.size(); i++) {
                                        if (products[i].getId() == tempItem->product->getId()) {
                                            products[i].setStock(products[i].getStock() - tempItem->quantity);
                                            break;
                                        }
                                    }
                                    tempItem = tempItem->next;
                                }
                                orders.push(Order(orderCounter, cart.getItems()));
                                orderCounter++;
                                cart.clear();
                                cout << "Order placed successfully!" << endl;
                                break;
                            }

                            case 7: {
                                if (orders.isEmpty()) cout << "No orders yet." << endl;
                                else {
                                    MyQueue<Order> tempQueue = orders;
                                    while (!tempQueue.isEmpty()) {
                                        tempQueue.front().display();
                                        tempQueue.pop();
                                    }
                                }
                                break;
                            }

                            case 0: cout << "User logged out." << endl; break;
                            default: cout << "Invalid choice." << endl; break;
                        }
                    }
                }

                break;
            }

            case 0: cout << "Exiting program." << endl; break;
            default: cout << "Invalid choice." << endl; break;
        }
    }

    return 0;
}
