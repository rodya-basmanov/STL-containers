#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../include/skip_list.h"

// Example usage of SkipList with integers
void integerExample() {
    std::cout << "=== SkipList with integers ===" << std::endl;
    
    try {
        stl_containers::SkipList<int> list;
        
        std::cout << "About to insert 30" << std::endl;
        list.insert(30);
        std::cout << "Inserted 30" << std::endl;
        
        std::cout << "About to insert 10" << std::endl;
        list.insert(10);
        std::cout << "Inserted 10" << std::endl;
        
        std::cout << "Elements after inserting 10 and 30: ";
        for (const auto& value : list) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        
        std::cout << "About to insert 50" << std::endl;
        try {
            list.insert(50);
            std::cout << "Inserted 50" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception while inserting 50: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception while inserting 50" << std::endl;
        }
        
        std::cout << "About to insert 20" << std::endl;
        list.insert(20);
        std::cout << "Inserted 20" << std::endl;
        
        std::cout << "About to insert 40" << std::endl;
        list.insert(40);
        std::cout << "Inserted 40" << std::endl;
    
        // Print elements (should be in sorted order)
        std::cout << "Elements: ";
        for (const auto& value : list) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        
        // Lookup operations
        std::cout << "Contains 20: " << (list.contains(20) ? "Yes" : "No") << std::endl;
        std::cout << "Contains 25: " << (list.contains(25) ? "Yes" : "No") << std::endl;
        
        // Erase element
        std::cout << "About to erase 30" << std::endl;
        list.erase(30);
        std::cout << "Erased 30" << std::endl;
        
        std::cout << "After erasing 30: ";
        for (const auto& value : list) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
}

// Example with custom comparator for reverse ordering
void reverseOrderExample() {
    std::cout << "\n=== SkipList with reverse order ===" << std::endl;
    
    try {
        stl_containers::SkipList<int, std::greater<int>> list;
        
        std::cout << "About to insert 30" << std::endl;
        list.insert(30);
        std::cout << "Inserted 30" << std::endl;
        
        std::cout << "About to insert 10" << std::endl;
        list.insert(10);
        std::cout << "Inserted 10" << std::endl;
        
        std::cout << "Elements (reverse order): ";
        for (const auto& value : list) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        
        std::cout << "About to insert 50" << std::endl;
        list.insert(50);
        std::cout << "Inserted 50" << std::endl;
        
        std::cout << "About to insert 20" << std::endl;
        list.insert(20);
        std::cout << "Inserted 20" << std::endl;
        
        std::cout << "About to insert 40" << std::endl;
        list.insert(40);
        std::cout << "Inserted 40" << std::endl;
        
        // Print elements (should be in reverse sorted order)
        std::cout << "Elements (reverse order): ";
        for (const auto& value : list) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
}

// Example with custom struct
struct Person {
    std::string name;
    int age;
    
    // Default constructor needed for some STL operations
    Person() : name(""), age(0) {}
    
    Person(const std::string& n, int a) : name(n), age(a) {}
    
    // Required for ordered containers when using default comparator
    bool operator<(const Person& other) const {
        return age < other.age; // Order by age
    }
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.name << " (" << p.age << ")";
    return os;
}

void customStructExample() {
    std::cout << "\n=== SkipList with custom struct ===" << std::endl;
    
    try {
        stl_containers::SkipList<Person> list;
        
        std::cout << "About to insert Alice" << std::endl;
        list.insert(Person("Alice", 30));
        std::cout << "Inserted Alice" << std::endl;
        
        std::cout << "About to insert Bob" << std::endl;
        list.insert(Person("Bob", 25));
        std::cout << "Inserted Bob" << std::endl;
        
        // Print elements (should be ordered by age)
        std::cout << "People ordered by age: ";
        for (const auto& person : list) {
            std::cout << person << " ";
        }
        std::cout << std::endl;
        
        std::cout << "About to insert Charlie" << std::endl;
        list.insert(Person("Charlie", 40));
        std::cout << "Inserted Charlie" << std::endl;
        
        std::cout << "About to insert Dave" << std::endl;
        list.insert(Person("Dave", 35));
        std::cout << "Inserted Dave" << std::endl;
        
        // Print elements (should be ordered by age)
        std::cout << "People ordered by age: ";
        for (const auto& person : list) {
            std::cout << person << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
}

int main() {
    try {
        integerExample();
        reverseOrderExample();
        customStructExample();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }
    
    return 0;
} 