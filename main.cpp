#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <limits>
#include <iomanip>

using namespace std;

// Specification B2 -RandNo Class
// Specification B3 - Singleton Design Pattern
class RandNo {
private:
    bool seedInitialized;

    RandNo() {
        // Makes sure to initialize the seed just once to ensure the time set seed code runs only once.
        srand(static_cast<unsigned>(time(0)));
        seedInitialized = true;
    }

public:
    static RandNo& getInstance() {
        static RandNo instance;
        return instance;
    }

    bool isSeedInitialized() const {
        return seedInitialized;
    }

    double generateRandomCost(double min, double max) {
        return min + (static_cast<double>(rand()) / RAND_MAX) * (max - min);
    }
};

// Specification B1 - Date class
class Date {
public:
    static int getYear() {
        time_t currentTime = time(nullptr);
        tm* timeInfo = localtime(&currentTime);
        return timeInfo->tm_year + 1900; // Adjust for years since 1900
    }
};

// Specification B4 - Inventory Entry Input Validation
namespace InVal {
    bool isValidItemId(const char* itemId) {
        return strlen(itemId) == 5 && isdigit(itemId[0]) && isdigit(itemId[1]) &&
               isdigit(itemId[2]) && isdigit(itemId[3]) && isdigit(itemId[4]);
    }

    bool isValidQuantity(int quantity) {
        return quantity >= 0;
    }

    bool isValidCost(double cost) {
        return cost >= 0.0;
    }
};

// The struct that represents the Inventory Items
struct InventoryItem {
    char itemId[6]; // 5 digits + '\0' for string termination
    int quantityAtHand;
    double wholesaleCost;
    double retailCost;
    time_t dateAdded; // Store date time
};

// The Program Greeting
void ProgramGreeting() {
    cout << "Welcome to your Inventory Inquisitor" << endl;
    cout << "Created by Khalid Abid" << endl;
    cout << "NOTE: This program is designed to only intake the first 5 Item ID #'s" << endl;
    cout << "anything after will not be displayed." << endl;
    cout << endl;
}

// Specification C1 - Alpha Menu
void displayMenu() {
    cout << "Main Menu" << endl;
    cout << "--------------------------------" << endl;
    cout << "A: Add Inventory" << endl;
    cout << "D: Delete Inventory" << endl;
    cout << "E: Edit Inventory" << endl;
    cout << "P: Print Inventory" << endl;
    cout << "Q: Quit Program" << endl;
    cout << "--------------------------------" << endl;
}

// Function for basic adding items to the inventory
void addItem(InventoryItem*& inventory, int& size) {
    InventoryItem newItem;
    char itemIdInput[6];
    int quantity;
    double wholesaleCost;

    do {
        cout << "Enter the 5 digits of Item ID (5 #'s'): ";
        cin >> itemIdInput;

        if (cin.fail() || strlen(itemIdInput) != 5 || !isdigit(itemIdInput[0]) || !isdigit(itemIdInput[1]) ||
            !isdigit(itemIdInput[2]) || !isdigit(itemIdInput[3]) || !isdigit(itemIdInput[4])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (strlen(itemIdInput) != 5);

    strcpy(newItem.itemId, itemIdInput);

    cout << "The Quantity at Hand of item: ";
    cin >> quantity;

    if (InVal::isValidQuantity(quantity)) {
        newItem.quantityAtHand = quantity;

        cout << "The Wholesale Cost of item: $";
        cin >> wholesaleCost;

        if (InVal::isValidCost(wholesaleCost)) {
            newItem.wholesaleCost = wholesaleCost;
            newItem.retailCost = newItem.wholesaleCost * (1 + (rand() % 41 + 10) / 100.0);
            newItem.dateAdded = time(nullptr);

            // Specification C2 - Dynamic Array
            InventoryItem* newInventory = new InventoryItem[size + 1];

            // Copy the old inventory to the new one
            for (int i = 0; i < size; ++i) {
                newInventory[i] = inventory[i];
            }

            // Add the new item to the end
            newInventory[size] = newItem;

            // Specification C3 - Resize Array
            size++;

            // Delete the old inventory and update the pointer
            delete[] inventory;
            inventory = newInventory;

            cout << "Your Item has been added to inventory successfully :)." << endl;
        } else {
            cout << "Invalid Wholesale Cost! Wholesale cost can't be negative, enter a positive value." << endl;
        }
    } else {
        cout << "Invalid Quantity. Please enter a non-negative integer." << endl;
    }
}

// This function deletes items from inventory
void deleteItem(InventoryItem*& inventory, int& size) {
    if (size > 0) {
        // Create a new dynamic array with reduced size
        InventoryItem* newInventory = new InventoryItem[size - 1];

        // Copy the old inventory to the new one, minus the last item
        for (int i = 0; i < size - 1; ++i) {
            newInventory[i] = inventory[i];
        }

        // Since deleting, decrease the size of this array
        size--;

        // update pointer
        delete[] inventory;
        inventory = newInventory;

        cout << "Last item has been deleted." << endl;
    } else {
        cout << "There is nothing to delete! INVENTORY EMPTY." << endl;
    }
}

// Specification A1 - Edit Inventory
void editItem(InventoryItem* inventory, int size, int index) {
    if (index >= 0 && index < size) {
        // let user change value of desired field
        cout << "Edit Item at index " << index << endl;

        // Editing of specific fields
        char choice;
        cout << "Which field do you want to edit?" << endl;
        cout << "1. Quantity at Hand" << endl;
        cout << "2. Wholesale Cost" << endl;
        cout << "Enter your choice (1 or 2): ";

        cin >> choice;

        switch (choice) {
            case '1':
                cout << "Enter the new quantity at hand: ";
                cin >> inventory[index].quantityAtHand;
                cout << "Your Item has been updated successfully." << endl;
                break;
            case '2':
                cout << "Enter the new wholesale cost: $";
                cin >> inventory[index].wholesaleCost;
                cout << "Your Item has been updated successfully." << endl;
                break;
            default:
                cout << "Invalid choice. No Updates made." << endl;
        }
    } else {
        cout << "That item does not exist." << endl;
    }
}

// Displaying all of the items in the inventory
void displayInventory(InventoryItem* inventory, int size) {
    cout << "Item #   ID         Quantity   Wholesale Cost   Retail Cost" << endl;
    cout << "------------------------------------------------------------" << endl;

    // use setw to make everything line up and more user-friendly
    for (int i = 0; i < size; ++i) {
        cout << setw(8) << "Item " << i << setw(10) << inventory[i].itemId << setw(10) << inventory[i].quantityAtHand
             << setw(13) << "$" << inventory[i].wholesaleCost << setw(14) << "$" << inventory[i].retailCost << endl;
    }
}

// Void of componentTest so its called  in main
void componentTest() {
    int currentYear = Date::getYear();
    int expectedYear = 2023;

    if (currentYear == expectedYear) {
        cout << "Component Test: Year generation is correct." << endl;
    } else {
        cout << "Component Test: Year generation is incorrect. Expected: " << expectedYear << ", Actual: " << currentYear << endl;
    }
}

// Component Test function for the RandNo class
void componentTestRandNo() {
    RandNo& randNo = RandNo::getInstance();
    if (randNo.isSeedInitialized()) {
        cout << "Component Test: RandNo seed is initialized." << endl;
    } else {
        cout << "Component Test: RandNo seed is not initialized." << endl;
    }
}

void UnitTest() {
    componentTest(); // Test Date class
    componentTestRandNo(); // Test RandNo class

}
int main() {
    ProgramGreeting();

// Runs it automatically when started
// Specification A4 - UnitTest() function in main()
    UnitTest();

    InventoryItem* inventory = nullptr;
    int size = 0;
    char choice;

    while (true) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (toupper(choice)) {
            case 'A':
                addItem(inventory, size);
                break;
            case 'D':
                deleteItem(inventory, size);
                break;
            case 'E':
                int index;
                cout << "Enter the index of the item to edit: ";
                cin >> index;
                editItem(inventory, size, index);
                break;
            case 'P':
                displayInventory(inventory, size);
                break;
            case 'Q':
                // Clean up and exit
                delete[] inventory;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}