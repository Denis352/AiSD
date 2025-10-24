#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
#include <sstream>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;

    Node(T value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int list_size;

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        list_size = 0;
    }

    ~DoublyLinkedList() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(T value) {
        Node<T>* new_node = new Node<T>(value);

        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        list_size++;
    }

    void push_front(T value) {
        Node<T>* new_node = new Node<T>(value);

        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        }
        else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        list_size++;
    }

    void pop_back() {
        if (tail == nullptr) return;

        Node<T>* temp = tail;
        tail = tail->prev;

        if (tail != nullptr) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }

        delete temp;
        list_size--;
    }

    void pop_front() {
        if (head == nullptr) return;

        Node<T>* temp = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }

        delete temp;
        list_size--;
    }

    T front() {
        if (head == nullptr) throw "List is empty";
        return head->data;
    }

    T back() {
        if (tail == nullptr) throw "List is empty";
        return tail->data;
    }

    bool empty() {
        return list_size == 0;
    }

    int size() {
        return list_size;
    }

    void print() {
        if (head == nullptr) {
            cout << "Список пуст" << endl;
            return;
        }

        Node<T>* current = head;
        cout << "Список: ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " <-> ";
            }
            current = current->next;
        }
        cout << endl;
    }
};

template <typename T>
class DynamicArray {
private:
    T* arr;
    int capacity;
    int length;

public:
    DynamicArray() {
        capacity = 2;
        length = 0;
        arr = new T[capacity];
    }

    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        length = other.length;
        arr = new T[capacity];
        for (int i = 0; i < length; i++) {
            arr[i] = other.arr[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            length = other.length;
            arr = new T[capacity];
            for (int i = 0; i < length; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void push_back(T value) {
        if (length >= capacity) {
            capacity *= 2;
            T* new_arr = new T[capacity];

            for (int i = 0; i < length; i++) {
                new_arr[i] = arr[i];
            }

            delete[] arr;
            arr = new_arr;
        }

        arr[length] = value;
        length++;
    }

    void pop_back() {
        if (length > 0) {
            length--;
        }
    }

    T& operator[](int index) {
        if (index < 0 || index >= length) throw "Index out of bounds";
        return arr[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= length) throw "Index out of bounds";
        return arr[index];
    }

    T back() {
        if (length == 0) throw "Array is empty";
        return arr[length - 1];
    }

    bool empty() {
        return length == 0;
    }

    int size() const {
        return length;
    }

    void print() {
        if (length == 0) {
            cout << "Массив пуст" << endl;
            return;
        }

        cout << "Массив [" << length << "]: ";
        for (int i = 0; i < length; i++) {
            cout << arr[i];
            if (i < length - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
};

template <typename T>
class Stack {
private:
    DoublyLinkedList<T> list;

public:
    void push(T value) {
        list.push_back(value);
    }

    void pop() {
        if (empty()) throw "Stack is empty";
        list.pop_back();
    }

    T top() {
        if (empty()) throw "Stack is empty";
        return list.back();
    }

    bool empty() {
        return list.empty();
    }

    int size() {
        return list.size();
    }

    void print() {
        cout << "Стек: ";
        if (empty()) {
            cout << "пуст" << endl;
        }
        else {
            Stack<T> temp;

            while (!empty()) {
                T value = top();
                temp.push(value);
                pop();
            }

            bool first = true;
            while (!temp.empty()) {
                T value = temp.top();
                if (!first) cout << " <- ";
                cout << value;
                push(value);
                temp.pop();
                first = false;
            }
            cout << " (верх)" << endl;
        }
    }
};

bool isOperator(string token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

bool isFunction(string token) {
    return token == "sin" || token == "cos";
}

int getPriority(string op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    if (isFunction(op)) return 4;
    if (op == "u-") return 5;
    return 0;
}

DynamicArray<string> infixToPostfix(string expression) {
    Stack<string> stack;
    DynamicArray<string> output;

    stringstream ss(expression);
    string token;

    while (ss >> token) {
        if (token == "(") {
            stack.push(token);
        }
        else if (token == ")") {
            while (!stack.empty() && stack.top() != "(") {
                output.push_back(stack.top());
                stack.pop();
            }
            if (!stack.empty() && stack.top() == "(") {
                stack.pop();
            }
        }
        else if (isOperator(token)) {
            if (token == "-" && (output.empty() || output.back() == "(")) {
                token = "u-";
            }

            while (!stack.empty() && stack.top() != "(" &&
                getPriority(stack.top()) >= getPriority(token)) {
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else {
            output.push_back(token);
        }
    }

    while (!stack.empty()) {
        if (stack.top() == "(") {
            throw "Несбалансированные скобки";
        }
        output.push_back(stack.top());
        stack.pop();
    }

    return output;
}

double evaluatePostfix(DynamicArray<string> postfix) {
    Stack<double> stack;

    for (int i = 0; i < postfix.size(); i++) {
        string token = postfix[i];

        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            stack.push(stod(token));
        }
        else if (token == "u-") {
            if (stack.empty()) throw "Недостаточно операндов для унарного минуса";
            double a = stack.top(); stack.pop();
            stack.push(-a);
        }
        else if (isOperator(token)) {
            if (stack.size() < 2) throw "Недостаточно операндов для оператора";

            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();

            if (token == "+") stack.push(a + b);
            else if (token == "-") stack.push(a - b);
            else if (token == "*") stack.push(a * b);
            else if (token == "/") {
                if (b == 0) throw "Деление на ноль";
                stack.push(a / b);
            }
            else if (token == "^") stack.push(pow(a, b));
        }
    }

    if (stack.size() != 1) throw "Некорректное выражение";

    return stack.top();
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "1. Тестирование двусвязного списка:" << endl;
    DoublyLinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(30);
    list.print();
    cout << "Первый элемент: " << list.front() << endl;
    cout << "Последний элемент: " << list.back() << endl;
    cout << "Размер: " << list.size() << endl;


    cout << "\n2. Тестирование динамического массива:" << endl;
    DynamicArray<int> arr;
    arr.push_back(100);
    arr.push_back(200);
    arr.push_back(300);
    arr.push_back(400);
    arr.print();
    cout << "Элемент с индексом 1: " << arr[1] << endl;
    cout << "Размер: " << arr.size() << endl;


    cout << "\n3. Тестирование стека:" << endl;
    Stack<string> stack_test;
    stack_test.push("первый");
    stack_test.push("второй");
    stack_test.push("третий");
    stack_test.print();
    cout << "Верхний элемент: " << stack_test.top() << endl;
    cout << "Размер: " << stack_test.size() << endl;


    cout << "\n4. Тестирование сортировочной станции:" << endl;

    string expressions[] = {
        "3 + 4 * 2",
    };

    cout << "\nВыражение: 3 + 4 * 2" << endl << expressions[0] << endl;

    try {
        DynamicArray<string> postfix = infixToPostfix(expressions[0]);
        cout << "Обратная польская запись: ";
        postfix.print();

        double result = evaluatePostfix(postfix);
        cout << "Результат: " << result << endl;
    }
    catch (const char* error) {
        cout << "Ошибка: " << error << endl;
    }


    cout << "\n5. Ввод пользовательского выражения:" << endl;
    cout << "=====================================" << endl;

    string user_expression;
    cout << "Введите математическое выражение (разделяйте все символы пробелами):" << endl;
    cout << "Пример: 3 + 4 * 2" << endl;
    cout << "Ваше выражение: ";

    getline(cin, user_expression);

    if (user_expression.empty()) {
        user_expression = "2 + 3 * 4";
        cout << "Используется выражение по умолчанию: " << user_expression << endl;
    }

    cout << "\nРезультат обработки вашего выражения:" << endl;
    cout << "Исходное выражение: " << user_expression << endl;

    try {
        DynamicArray<string> user_postfix = infixToPostfix(user_expression);
        cout << "Обратная польская запись: ";
        user_postfix.print();

        double user_result = evaluatePostfix(user_postfix);
        cout << "Результат вычисления: " << user_result << endl;
    }
    catch (const char* error) {
        cout << "Ошибка при обработке выражения: " << error << endl;
    }

    return 0;
}
