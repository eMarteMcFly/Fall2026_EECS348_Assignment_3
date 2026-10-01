/*
 * Program Name: EECS 348 Assignment 3 - CEO Email Priority Queue
 * Program Description: 
 *   This C++ program reads commands and emails from standard input, managing a priority queue
 *   for a CEO's inbox. Priority is determined first by sender category (Boss > Subordinate > 
 *   Peer > Important Person > Other Person) and second by date (newer emails have higher priority).
 *   The queue is managed using an object-oriented, list-based MaxHeap implemented from scratch.
 * 
 * Inputs: 
 *   Commands from standard input (stdin) including EMAIL, NEXT, READ, and COUNT.
 * 
 * Outputs: 
 *   Formatted string messages displayed on the terminal stdout.
 * 
 * Collaborators: 
 *   - ChatGPT (Used as GenAI Baseline 1 for code generation and analysis)
 *   - Claude 3.5 Sonnet (Used as GenAI Baseline 2 for code comparison)

 * Other Sources: 
 *   None
 * 
 * Author's Full Name: Erick Marte
 * Creation Date: October 1, 2026
 * Revision Date: October 1, 2026
 * Revisions:
 *   - Oct 01, 2026: Initial implementation of MaxHeap and Email object structure.
 */

#include <iostream>  // Included for standard console input/output operations
#include <string>    // Included for std::string handling and operations
#include <sstream>   // Included for string stream parsing of commands
#include <vector>    // Included for dynamic list-based underlying container

using namespace std; // Using standard namespace to simplify terminal I/O code

// Enum representing the priority rank of different sender categories
// Author: [Your Full Name Here] / GenAI Baseline Hybrid
enum PriorityRank {
    BOSS = 5,             // Highest priority category
    SUBORDINATE = 4,      // Second priority category
    PEER = 3,             // Third priority category
    IMPORTANT_PERSON = 2, // Fourth priority category
    OTHER_PERSON = 1      // Lowest priority category
};

// Class representing an individual Email object
// Author: [Your Full Name Here]
class Email {
private:
    string senderCategory; // Raw string representation of sender category
    string subjectLine;    // Subject line of the email
    string dateStr;        // Date in MM-DD-YYYY format
    int categoryRank;      // Priority rank integer derived from sender category
    string formattedDate;  // Canonical date string (YYYYMMDD) used for lexicographical comparison

    // Helper method to convert category string to priority rank enum value
    // Author: [Your Full Name Here]
    int parseCategoryRank(const string& category) {
        if (category == "Boss") return BOSS;                        // Map "Boss" to rank 5
        if (category == "Subordinate") return SUBORDINATE;         // Map "Subordinate" to rank 4
        if (category == "Peer") return PEER;                       // Map "Peer" to rank 3
        if (category == "Important Person" || category == "ImportantPerson") 
            return IMPORTANT_PERSON;                               // Map "Important Person" to rank 2
        return OTHER_PERSON;                                       // Default remaining categories to rank 1
    }

    // Helper method to reformat MM-DD-YYYY to YYYYMMDD for easier comparison
    // Author: [Your Full Name Here]
    string convertDateToCanonical(const string& date) {
        if (date.length() != 10) return date;                      // Return as-is if string isn't standard length
        string month = date.substr(0, 2);                          // Extract month digits (MM)
        string day = date.substr(3, 2);                            // Extract day digits (DD)
        string year = date.substr(6, 4);                           // Extract year digits (YYYY)
        return year + month + day;                                 // Combine to create sortable string YYYYMMDD
    }

public:
    // Default constructor
    // Author: [Your Full Name Here]
    Email() : senderCategory(""), subjectLine(""), dateStr(""), categoryRank(0), formattedDate("") {}

    // Parameterized constructor initializing all fields and derived metrics
    // Author: [Your Full Name Here]
    Email(string category, string subject, string date) {
        senderCategory = category;                                 // Store raw sender category string
        subjectLine = subject;                                     // Store subject line string
        dateStr = date;                                            // Store original date string
        categoryRank = parseCategoryRank(category);                // Calculate and store category rank
        formattedDate = convertDateToCanonical(date);              // Calculate and store canonical date string
    }

    // Getter methods for private members
    // Author: [Your Full Name Here]
    string getSenderCategory() const { return senderCategory; }    // Returns sender category string
    string getSubjectLine() const { return subjectLine; }          // Returns subject line string
    string getDateStr() const { return dateStr; }                  // Returns original date string
    int getCategoryRank() const { return categoryRank; }           // Returns integer category rank
    string getFormattedDate() const { return formattedDate; }      // Returns sortable date string

    // Greater-than operator overloaded to define MaxHeap priority evaluation
    // Priority rule: Higher category rank first; if tied, newer date (lexicographically greater) first.
    // Author: [Your Full Name Here]
    bool operator>(const Email& other) const {
        if (this->categoryRank != other.categoryRank) {            // Compare priority categories first
            return this->categoryRank > other.categoryRank;        // Higher category rank takes higher priority
        }
        return this->formattedDate > other.formattedDate;          // Newer date string takes higher priority when categories match
    }

    // Less-than operator definition
    // Author: [Your Full Name Here]
    bool operator<(const Email& other) const {
        if (this->categoryRank != other.categoryRank) {            // Compare priority categories first
            return this->categoryRank < other.categoryRank;        // Lower category rank takes lower priority
        }
        return this->formattedDate < other.formattedDate;          // Older date string takes lower priority when categories match
    }
};

// Object-Oriented List-Based MaxHeap implementation for Email objects
// Author: [Your Full Name Here]
class MaxHeap {
private:
    vector<Email> heap; // Internal list-based dynamic array container

    // Helper method to return parent index for given node index
    // Author: [Your Full Name Here]
    int parent(int i) { return (i - 1) / 2; }                      // Calculate parent index in continuous heap storage

    // Helper method to return left child index for given node index
    // Author: [Your Full Name Here]
    int leftChild(int i) { return (2 * i) + 1; }                   // Calculate left child index in continuous heap storage

    // Helper method to return right child index for given node index
    // Author: [Your Full Name Here]
    int rightChild(int i) { return (2 * i) + 2; }                  // Calculate right child index in continuous heap storage

    // Sift node up to maintain heap property after insertion
    // Author: [Your Full Name Here]
    void siftUp(int i) {
        while (i > 0 && heap[i] > heap[parent(i)]) {               // Loop while node is greater than parent
            swap(heap[i], heap[parent(i)]);                        // Swap node with its parent element
            i = parent(i);                                         // Move index up to parent position
        }
    }

    // Sift node down to maintain heap property after removal
    // Author: [Your Full Name Here]
    void siftDown(int i) {
        int maxIndex = i;                                          // Assume current node is maximum
        int left = leftChild(i);                                   // Get index of left child
        int right = rightChild(i);                                 // Get index of right child

        if (left < heap.size() && heap[left] > heap[maxIndex]) {   // Check if left child exists and is larger
            maxIndex = left;                                       // Update maxIndex to left child
        }

        if (right < heap.size() && heap[right] > heap[maxIndex]) { // Check if right child exists and is larger
            maxIndex = right;                                      // Update maxIndex to right child
        }

        if (i != maxIndex) {                                       // If max is not current node
            swap(heap[i], heap[maxIndex]);                         // Swap current node with larger child
            siftDown(maxIndex);                                    // Recursively sift down at new index
        }
    }

public:
    // Constructor
    // Author: [Your Full Name Here]
    MaxHeap() {}                                                   // Default constructor initializing empty heap

    // Check if heap is empty
    // Author: [Your Full Name Here]
    bool isEmpty() const { return heap.empty(); }                  // Return true if internal list contains no elements

    // Get number of elements in heap
    // Author: [Your Full Name Here]
    size_t size() const { return heap.size(); }                    // Return current count of elements stored

    // Insert a new Email into heap
    // Author: [Your Full Name Here]
    void insert(const Email& email) {
        heap.push_back(email);                                     // Append new email to end of dynamic array
        siftUp(heap.size() - 1);                                   // Re-establish heap order starting from new element
    }

    // Peek at maximum priority element without removing it
    // Author: [Your Full Name Here]
    Email getMax() const {
        if (!heap.empty()) return heap[0];                         // Return root element if heap is non-empty
        return Email();                                            // Return default Email if heap is empty
    }

    // Extract and remove maximum priority element
    // Author: [Your Full Name Here]
    void extractMax() {
        if (heap.empty()) return;                                  // Do nothing if heap is empty
        heap[0] = heap.back();                                     // Overwrite root node with last element
        heap.pop_back();                                           // Remove last element from array
        siftDown(0);                                               // Re-establish heap order starting from root node
    }
};

// Utility function to trim leading and trailing spaces from input strings
// Author: [Your Full Name Here]
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");               // Locate first non-whitespace character
    if (first == string::npos) return "";                          // Return empty string if only whitespace
    size_t last = str.find_last_not_of(" \t\r\n");                 // Locate last non-whitespace character
    return str.substr(first, (last - first + 1));                  // Return sliced string without surrounding whitespace
}

// Main execution function
// Author: [Your Full Name Here]
int main() {
    MaxHeap inboxQueue;                                            // Instantiate MaxHeap priority queue object
    string line;                                                   // Buffer variable to hold each line read from stdin

    while (getline(cin, line)) {                                   // Read input stream line-by-line until EOF
        line = trim(line);                                         // Strip extra trailing/leading whitespace
        if (line.empty()) continue;                                // Skip processing if line is empty

        if (line.rfind("EMAIL", 0) == 0) {                         // Check if line begins with command "EMAIL"
            string dataPart = line.substr(5);                      // Extract argument payload following "EMAIL "
            stringstream ss(dataPart);                             // Create stringstream to tokenize arguments
            string category, subject, date;                        // Storage variables for email properties

            getline(ss, category, ',');                           // Parse category delimited by comma
            getline(ss, subject, ',');                            // Parse subject line delimited by comma
            getline(ss, date, ',');                               // Parse date string delimited by comma

            category = trim(category);                            // Trim category string
            subject = trim(subject);                             // Trim subject string
            date = trim(date);                                     // Trim date string

            Email newEmail(category, subject, date);               // Instantiate Email object with parsed values
            inboxQueue.insert(newEmail);                           // Push new email onto priority queue MaxHeap
        } 
        else if (line == "COUNT") {                                // Process "COUNT" command
            cout << "There are " << inboxQueue.size() 
                 << " emails to read." << endl;                    // Display current unread email count
        } 
        else if (line == "NEXT") {                                 // Process "NEXT" command
            if (!inboxQueue.isEmpty()) {                           // If queue has pending emails
                Email top = inboxQueue.getMax();                   // Retrieve highest priority email
                cout << "Next email:" << endl;                     // Print required output header
                cout << "Sender: " << top.getSenderCategory() << endl; // Display sender category
                cout << "Subject: " << top.getSubjectLine() << endl;   // Display subject line
                cout << "Date: " << top.getDateStr() << endl;          // Display email date
            }
        } 
        else if (line == "READ") {                                 // Process "READ" command
            if (!inboxQueue.isEmpty()) {                           // If queue has pending emails
                inboxQueue.extractMax();                           // Remove top priority email from heap queue
            }
        }
    }

    return 0;                                                      // Exit program execution successfully
}