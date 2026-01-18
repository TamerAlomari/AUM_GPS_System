#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

// ------------------------ ENUM ------------------------
enum NodeType {
    ROOM,
    FLOOR,
    BUILDING,
    UNIVERSITY
};

string NodeTypeToString(NodeType type) {
    switch (type) {
        case ROOM: return "ROOM";
        case FLOOR: return "FLOOR";
        case BUILDING: return "BUILDING";
        case UNIVERSITY: return "UNIVERSITY";
        default: return "UNKNOWN";
    }
}

// ------------------------ NODE STRUCT ------------------------
struct Node {
    NodeType type;
    string name;
    bool isAvailable;
    Node* firstChild;
    Node* nextSibling;

    Node(NodeType t, const string& n)
        : type(t), name(n), isAvailable(true), firstChild(nullptr), nextSibling(nullptr) {}
};

// ------------------------ GPS CLASS ------------------------
class AUMGps {
public:
    Node* root;

    AUMGps() { root = new Node(UNIVERSITY, "AUM"); }
    ~AUMGps() { deleteTree(root); }

    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->firstChild);
        deleteTree(node->nextSibling);
        delete node;
    }

    bool canAdd(Node* parent, NodeType type, const string& name) {
        if (!parent || name.empty()) return false;
        if (type == ROOM && parent->type != FLOOR) return false;
        if (type == FLOOR && parent->type != BUILDING) return false;
        if (type == BUILDING && parent->type != UNIVERSITY) return false;

        Node* current = parent->firstChild;
        while (current) {
            if (current->name == name && current->type == type) return false;
            current = current->nextSibling;
        }
        return true;
    }

    Node* findNode(Node* node, NodeType type, const string& name) {
        if (!node) return nullptr;
        if (node->type == type && node->name == name) return node;

        Node* found = findNode(node->firstChild, type, name);
        if (found) return found;
        return findNode(node->nextSibling, type, name);
    }

    void addNode(Node* parent, NodeType type, const string& name) {
        if (!canAdd(parent, type, name)) {
            cout << "\nCannot add node: Invalid parent or duplicate name.\n";
            return;
        }

        Node* newNode = new Node(type, name);
        if (!parent->firstChild) parent->firstChild = newNode;
        else {
            Node* current = parent->firstChild;
            while (current->nextSibling) current = current->nextSibling;
            current->nextSibling = newNode;
        }

        cout << NodeTypeToString(type) << " \"" << name << "\" added successfully.\n";
    }

    bool areParentsOpen(Node* target) {
        vector<Node*> path;
        findPathVector(root, target->name, path);
        for (size_t i = 0; i + 1 < path.size(); i++) {
            if (!path[i]->isAvailable) return false;
        }
        return true;
    }

    void setSubtreeAvailability(Node* node, bool status) {
        if (!node) return;
        node->isAvailable = status;
        setSubtreeAvailability(node->firstChild, status);
        setSubtreeAvailability(node->nextSibling, status);
    }

    void setNodeStatus(NodeType type, bool status) {
        string name;
        cout << "Enter Name: ";
        getline(cin, name);
        transform(name.begin(), name.end(), name.begin(), ::toupper);

        Node* node = findNode(root, type, name);
        if (!node) { cout << "Node not found.\n"; return; }

        if (node->isAvailable == status) {
            cout << name << (status ? " is already open.\n" : " is already closed.\n");
            return;
        }

        if (status && !areParentsOpen(node)) {
            cout << "Cannot open. One of the parent locations is closed.\n";
            return;
        }

        if (type == ROOM) {
            node->isAvailable = status;
            cout << name << (status ? " is now open.\n" : " is now closed.\n");
            return;
        }

        setSubtreeAvailability(node, status);
        cout << name << (status ? " and all sub-locations are now open.\n" 
                                : " and all sub-locations are now closed.\n");
    }

    void printDetails(Node* node, int depth = 0) {
        if (!node) return;
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "- " << node->name << " ( " << NodeTypeToString(node->type)
             << (node->isAvailable ? " - Open" : " - Closed") << " )\n";
        printDetails(node->firstChild, depth + 1);
        printDetails(node->nextSibling, depth);
    }

    bool findPathVector(Node* node, const string& target, vector<Node*>& path) {
        if (!node) return false;
        path.push_back(node);
        if (node->name == target) return true;
        if (findPathVector(node->firstChild, target, path)) return true;
        path.pop_back();
        return findPathVector(node->nextSibling, target, path);
    }

    void printRealPath(const string& from, const string& to) {
        vector<Node*> pathFrom, pathTo;
        if (!findPathVector(root, from, pathFrom) || !findPathVector(root, to, pathTo)) {
            cout << "One or both locations not found.\n"; return;
        }

        int i = 0;
        while (i < pathFrom.size() && i < pathTo.size() && pathFrom[i] == pathTo[i]) i++;

        cout << "Path from " << from << " to " << to << ":\n";
        for (int j = pathFrom.size() - 1; j >= i; j--) cout << pathFrom[j]->name << " -> ";
        for (int j = i; j < pathTo.size(); j++) {
            cout << pathTo[j]->name;
            if (j != pathTo.size() - 1) cout << " -> ";
        }
        cout << endl;
    }

    void searchRoom(const string& roomName) {
        Node* room = findNode(root, ROOM, roomName);
        if (!room) { cout << "Room not found.\n"; return; }

        cout << "Room: " << room->name << "\nStatus: "
             << (room->isAvailable ? "Available" : "Closed") << endl;

        vector<Node*> path;
        findPathVector(root, roomName, path);
        cout << "Location: ";
        for (auto n : path) {
            cout << n->name;
            if (n != path.back()) cout << " -> ";
        }
        cout << endl;
    }

    void saveToFile(Node* node, ofstream& file, int level = 0) {
        if (!node) return;
        for (int i = 0; i < level; i++) file << " ";
        file << NodeTypeToString(node->type) << " " << node->name << endl;
        saveToFile(node->firstChild, file, level + 1);
        saveToFile(node->nextSibling, file, level);
    }

    void loadFromFile(ifstream& file) {
        deleteTree(root);
        root = nullptr;
        string line;
        Node* parentNodes[100] = { nullptr };

        while (getline(file, line)) {
            int level = 0;
            while (line[level] == ' ') level++;
            line = line.substr(level);

            size_t pos = line.find(' ');
            string typeStr = line.substr(0, pos);
            string name = line.substr(pos + 1);

            NodeType type;
            if (typeStr == "ROOM") type = ROOM;
            else if (typeStr == "FLOOR") type = FLOOR;
            else if (typeStr == "BUILDING") type = BUILDING;
            else type = UNIVERSITY;

            Node* newNode = new Node(type, name);
            if (level == 0) root = newNode;
            else {
                Node* parent = parentNodes[level - 1];
                if (!parent->firstChild) parent->firstChild = newNode;
                else {
                    Node* sibling = parent->firstChild;
                    while (sibling->nextSibling) sibling = sibling->nextSibling;
                    sibling->nextSibling = newNode;
                }
            }
            parentNodes[level] = newNode;
        }
    }
};

// ------------------------ UTILITY ------------------------
void save(AUMGps& gps) {
    try {
        ofstream file("AUM_GPS_Data.txt");
        if (!file) throw runtime_error("Failed to open save file.");
        gps.saveToFile(gps.root, file);
        file.close();
    } catch (const exception& e) {
        cout << "Save error: " << e.what() << endl;
    }
}

void load(AUMGps& gps) {
    try {
        ifstream file("AUM_GPS_Data.txt");
        if (!file) throw runtime_error("Save file not found.");
        gps.loadFromFile(file);
        file.close();
    } catch (const exception& e) {
        cout << "Load warning: " << e.what() << endl;
        cout << "Starting with empty map...\n";
        gps.root = new Node(UNIVERSITY, "AUM");
    }
}

// ------------------------ ADMIN FUNCTIONS ------------------------
bool isAdmin() {
    const string password = "JOTM";
    for (int i = 0; i < 3; i++) {
        string input;
        cout << "\nEnter Admin Password: ";
        cin >> input;
        if (input == password) {
            cout << "Login Successful.\n";
            return true;
        }
        cout << "Wrong Password.\n";
    }
    return false;
}

void addFromAdmin(AUMGps& gps) {
    string choice, name;
    cout << "\n1. Add Building\n2. Add Floor\n3. Add Room\nChoice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == "1") {
        cout << "Enter Building Name: ";
        getline(cin, name);
        transform(name.begin(), name.end(), name.begin(), ::toupper);
        gps.addNode(gps.root, BUILDING, name);
    } else if (choice == "2") {
        string buildingName;
        cout << "Enter Parent Building Name: ";
        getline(cin, buildingName);
        transform(buildingName.begin(), buildingName.end(), buildingName.begin(), ::toupper);

        Node* parent = gps.findNode(gps.root, BUILDING, buildingName);
        if (!parent) { cout << "Building not found.\n"; return; }

        cout << "Enter Floor Name: ";
        getline(cin, name);
        transform(name.begin(), name.end(), name.begin(), ::toupper);
        gps.addNode(parent, FLOOR, name);
    } else if (choice == "3") {
        string buildingName, floorName;
        cout << "Enter Parent Building Name: ";
        getline(cin, buildingName);
        transform(buildingName.begin(), buildingName.end(), buildingName.begin(), ::toupper);

        Node* buildingNode = gps.findNode(gps.root, BUILDING, buildingName);
        if (!buildingNode) { cout << "Building not found.\n"; return; }

        cout << "Enter Parent Floor Name: ";
        getline(cin, floorName);
        transform(floorName.begin(), floorName.end(), floorName.begin(), ::toupper);

        Node* floorNode = gps.findNode(buildingNode, FLOOR, floorName);
        if (!floorNode) { cout << "Floor not found.\n"; return; }

        cout << "Enter Room Name: ";
        getline(cin, name);
        transform(name.begin(), name.end(), name.begin(), ::toupper);

        gps.addNode(floorNode, ROOM, name);
    } else {
        cout << "Invalid Choice.\n";
    }
}

// ------------------------ ADMIN MENU ------------------------
void adminMenu(AUMGps& gps) {
    string choice;
    do {
        cout << "\n--- Admin Menu ---\n1. Add\n2. View Map\n3. Close\n4. Open\n5. Back\n6. Exit\nChoice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "1") addFromAdmin(gps);
        else if (choice == "2") gps.printDetails(gps.root);
        else if (choice == "3") {
            cout << "1. Building\n2. Floor\n3. Room\nChoice to Close: ";
            string c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (c == "1") gps.setNodeStatus(BUILDING, false);
            else if (c == "2") gps.setNodeStatus(FLOOR, false);
            else if (c == "3") gps.setNodeStatus(ROOM, false);
            else cout << "Invalid Choice.\n";
        }
        else if (choice == "4") {
            cout << "Note: Cannot open a node if its parent is closed.\n";
            cout << "1. Building\n2. Floor\n3. Room\nChoice to Open: ";
            string c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (c == "1") gps.setNodeStatus(BUILDING, true);
            else if (c == "2") gps.setNodeStatus(FLOOR, true);
            else if (c == "3") gps.setNodeStatus(ROOM, true);
            else cout << "Invalid Choice.\n";
        }
        else if (choice == "5") { save(gps); return; }
        else if (choice == "6") { save(gps); exit(0); }
        else cout << "Invalid Choice.\n";
    } while (true);
}

// ------------------------ USER MENU ------------------------
void userMenu(AUMGps& gps) {
    string choice;
    do {
        cout << "\n--- User Menu ---\n1. View Map\n2. Search Room\n3. Find Path\n4. Back\n5. Exit\nChoice: ";
        cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "1") gps.printDetails(gps.root);
        else if (choice == "2") {
            string roomName;
            cout << "Room Name: ";
            getline(cin, roomName);
            transform(roomName.begin(), roomName.end(), roomName.begin(), ::toupper);
            gps.searchRoom(roomName);
        }
        else if (choice == "3") {
            string from, to;
            cout << "From: "; getline(cin, from); transform(from.begin(), from.end(), from.begin(), ::toupper);
            cout << "To: "; getline(cin, to); transform(to.begin(), to.end(), to.begin(), ::toupper);
            gps.printRealPath(from, to);
        }
        else if (choice == "4") { save(gps); return; }
        else if (choice == "5") { save(gps); exit(0); }
        else cout << "Invalid Choice.\n";
    } while (true);
}

// ------------------------ MAIN ------------------------
int main() {
    AUMGps gps;
    load(gps);

    cout << "Welcome to AUM GPS System\n";
    string choice;
    while (true) {
        cout << "\n1. Admin\n2. User\n3. Exit\nChoice: ";
        cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "1" && isAdmin()) adminMenu(gps);
        else if (choice == "2") userMenu(gps);
        else if (choice == "3") break;
        else cout << "Invalid Choice.\n";
    }

    return 0;
}
