#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Contact {
public:
    Contact(string name, string phone) : name(name), phone(phone) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }

    void display() const {
        cout << "Name: " << name << ", Phone: " << phone << endl;
    }

private:
    string name;
    string phone;
};

void addContact(vector<Contact>& contacts) {
    string name, phone;
    cout << "Enter name: ";
    cin.ignore(); // To ignore the newline character left by the previous input
    getline(cin, name);
    cout << "Enter phone number: ";
    getline(cin, phone);
    contacts.emplace_back(name, phone);
}

void viewContacts(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "No contacts available." << endl;
    } else {
        for (const auto& contact : contacts) {
            contact.display();
        }
    }
}

void deleteContact(vector<Contact>& contacts) {
    string name;
    cout << "Enter the name of the contact to delete: ";
    cin.ignore();
    getline(cin, name);

    auto it = remove_if(contacts.begin(), contacts.end(), [&name](const Contact& contact) {
        return contact.getName() == name;
    });

    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        cout << "Contact deleted." << endl;
    } else {
        cout << "Contact not found." << endl;
    }
}

void displayMenu() {
    cout << "1. Add Contact" << endl;
    cout << "2. View Contacts" << endl;
    cout << "3. Delete Contact" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    vector<Contact> contacts;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            addContact(contacts);
            break;
        case 2:
            viewContacts(contacts);
            break;
        case 3:
            deleteContact(contacts);
            break;
        case 4:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
