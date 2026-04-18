#include <iostream>
#include <climits>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// ============== CUSTOM STRING CLASS (Simple wrapper to avoid char[]) ==============
// We'll use std::string as requested

// ============== HASH NODE FOR CITY ==============
class CityHashNode {
public:
    int key;
    string name;
    int distance;
    CityHashNode* next;
    
    CityHashNode(int k, string n) : key(k), name(n), distance(INT_MAX), next(nullptr) {}
};

// ============== HASH TABLE FOR CITIES ==============
class CityHashTable {
private:
    static const int TABLE_SIZE = 100;
    CityHashNode** table;
    
    int hashFunction(int key) {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }
    
public:
    CityHashTable() {
        table = new CityHashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    ~CityHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            CityHashNode* current = table[i];
            while (current != nullptr) {
                CityHashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    void insert(int key, string name) {
        int index = hashFunction(key);
        CityHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                current->name = name;
                return;
            }
            current = current->next;
        }
        
        CityHashNode* newNode = new CityHashNode(key, name);
        newNode->next = table[index];
        table[index] = newNode;
    }
    
    bool find(int key, string& name) {
        int index = hashFunction(key);
        CityHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                name = current->name;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    bool exists(int key) {
        int index = hashFunction(key);
        CityHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            CityHashNode* current = table[i];
            while (current != nullptr) {
                CityHashNode* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }
};

// ============== HASH NODE FOR INTEGERS ==============
class IntHashNode {
public:
    int key;
    int value;
    IntHashNode* next;
    
    IntHashNode(int k, int v) : key(k), value(v), next(nullptr) {}
};

// ============== HASH TABLE FOR INTEGERS ==============
class IntHashTable {
private:
    static const int TABLE_SIZE = 100;
    IntHashNode** table;
    
    int hashFunction(int key) {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }
    
public:
    IntHashTable() {
        table = new IntHashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    ~IntHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            IntHashNode* current = table[i];
            while (current != nullptr) {
                IntHashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    void insert(int key, int value) {
        int index = hashFunction(key);
        IntHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        IntHashNode* newNode = new IntHashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }
    
    bool find(int key, int& value) {
        int index = hashFunction(key);
        IntHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    bool exists(int key) {
        int index = hashFunction(key);
        IntHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void remove(int key) {
        int index = hashFunction(key);
        IntHashNode* current = table[index];
        IntHashNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
};

// ============== ARRAY LIST FOR INTEGERS ==============
class IntArrayList {
private:
    int* arr;
    int capacity;
    int length;
    
    void resize() {
        capacity *= 2;
        int* newArr = new int[capacity];
        for (int i = 0; i < length; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }
    
public:
    IntArrayList() {
        capacity = 10;
        length = 0;
        arr = new int[capacity];
    }
    
    ~IntArrayList() {
        delete[] arr;
    }
    
    void push_back(int value) {
        if (length >= capacity) {
            resize();
        }
        arr[length++] = value;
    }
    
    int get(int index) {
        return arr[index];
    }
    
    int size() {
        return length;
    }
    
    bool empty() {
        return length == 0;
    }
};

// ============== ROUTE CLASS ==============
class Route {
public:
    int neighbor;
    int distance;
    int traffic;
    bool isBlocked;
    Route* next;
    
    Route(int n, int d) : neighbor(n), distance(d), traffic(0), 
                          isBlocked(false), next(nullptr) {}
};

// ============== LINKED LIST FOR ROUTES ==============
class RouteList {
private:
    Route* head;
    
public:
    RouteList() : head(nullptr) {}
    
    ~RouteList() {
        Route* current = head;
        while (current != nullptr) {
            Route* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    void push_back(Route* route) {
        if (head == nullptr) {
            head = route;
        } else {
            Route* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = route;
        }
    }
    
    Route* getHead() {
        return head;
    }
    
    bool empty() {
        return head == nullptr;
    }
};

// ============== HASH NODE FOR ROUTE LISTS ==============
class RouteListHashNode {
public:
    int key;
    RouteList* value;
    RouteListHashNode* next;
    
    RouteListHashNode(int k, RouteList* v) : key(k), value(v), next(nullptr) {}
};

// ============== HASH TABLE FOR ADJACENCY LIST ==============
class AdjacencyHashTable {
private:
    static const int TABLE_SIZE = 100;
    RouteListHashNode** table;
    
    int hashFunction(int key) {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }
    
public:
    AdjacencyHashTable() {
        table = new RouteListHashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    ~AdjacencyHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            RouteListHashNode* current = table[i];
            while (current != nullptr) {
                RouteListHashNode* temp = current;
                current = current->next;
                delete temp->value;
                delete temp;
            }
        }
        delete[] table;
    }
    
    void insert(int key, RouteList* value) {
        int index = hashFunction(key);
        RouteListHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                delete current->value;
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        RouteListHashNode* newNode = new RouteListHashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }
    
    bool find(int key, RouteList*& value) {
        int index = hashFunction(key);
        RouteListHashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            RouteListHashNode* current = table[i];
            while (current != nullptr) {
                RouteListHashNode* temp = current;
                current = current->next;
                delete temp->value;
                delete temp;
            }
            table[i] = nullptr;
        }
    }
};

// ============== HEAP NODE CLASS (replaces struct) ==============
class HeapNode {
public:
    int distance;
    int nodeId;
    
    HeapNode() : distance(0), nodeId(0) {}
    HeapNode(int d, int n) : distance(d), nodeId(n) {}
};

// ============== MIN HEAP ==============
class MinHeap {
private:
    HeapNode* heap;
    int capacity;
    int heapSize;
    IntHashTable indexMap;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    void swap(int i, int j) {
        indexMap.insert(heap[i].nodeId, j);
        indexMap.insert(heap[j].nodeId, i);
        
        HeapNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }
    
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)].distance > heap[i].distance) {
            swap(i, parent(i));
            i = parent(i);
        }
    }
    
    void heapifyDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);
        
        if (left < heapSize && heap[left].distance < heap[minIndex].distance) {
            minIndex = left;
        }
        if (right < heapSize && heap[right].distance < heap[minIndex].distance) {
            minIndex = right;
        }
        
        if (i != minIndex) {
            swap(i, minIndex);
            heapifyDown(minIndex);
        }
    }
    
public:
    MinHeap(int cap = 1000) {
        capacity = cap;
        heapSize = 0;
        heap = new HeapNode[capacity];
    }
    
    ~MinHeap() {
        delete[] heap;
    }
    
    void push(int distance, int nodeId) {
        int idx;
        if (indexMap.find(nodeId, idx)) {
            if (distance < heap[idx].distance) {
                heap[idx].distance = distance;
                heapifyUp(idx);
            }
            return;
        }
        
        if (heapSize >= capacity) {
            cout << "Heap is full!\n";
            return;
        }
        
        heap[heapSize].distance = distance;
        heap[heapSize].nodeId = nodeId;
        indexMap.insert(nodeId, heapSize);
        heapifyUp(heapSize);
        heapSize++;
    }
    
    void pop() {
        if (heapSize == 0) return;
        
        indexMap.remove(heap[0].nodeId);
        
        if (heapSize == 1) {
            heapSize--;
            return;
        }
        
        heap[0] = heap[heapSize - 1];
        heapSize--;
        
        if (heapSize > 0) {
            indexMap.insert(heap[0].nodeId, 0);
            heapifyDown(0);
        }
    }
    
    int getTopDistance() {
        return heap[0].distance;
    }
    
    int getTopNode() {
        return heap[0].nodeId;
    }
    
    bool empty() {
        return heapSize == 0;
    }
};

// ============== GRAPH CLASS ==============
class Graph {
private:
    static const int MAX_CITIES = 100;
    CityHashTable cities;
    AdjacencyHashTable adj;
    int nextCityId;
    int cityIds[MAX_CITIES];
    int cityCount;
    
public:
    Graph() : nextCityId(1), cityCount(0) {}
    
    int addCity(const string& name) {
        if (name.empty()) {
            cout << "Error: City name cannot be empty!\n";
            return -1;
        }
        
        // Check for duplicate
        for (int i = 0; i < cityCount; i++) {
            string cityName;
            cities.find(cityIds[i], cityName);
            if (cityName == name) {
                cout << "Error: City '" << name << "' already exists with ID: " << cityIds[i] << endl;
                return -1;
            }
        }
        
        int id = nextCityId++;
        cities.insert(id, name);
        cityIds[cityCount++] = id;
        adj.insert(id, new RouteList());
        
        cout << "City '" << name << "' added with ID: " << id << endl;
        return id;
    }
    
    void addEdge(int u, int v, int w, bool direction) {
        string cityU, cityV;
        if (!cities.find(u, cityU)) {
            cout << "Error: Source city with ID " << u << " does not exist!\n";
            return;
        }
        if (!cities.find(v, cityV)) {
            cout << "Error: Destination city with ID " << v << " does not exist!\n";
            return;
        }
        if (u == v) {
            cout << "Error: Cannot create a route from a city to itself!\n";
            return;
        }
        if (w <= 0) {
            cout << "Error: Distance must be positive!\n";
            return;
        }
        
        RouteList* routes;
        adj.find(u, routes);
        
        // Check for duplicate
        Route* current = routes->getHead();
        while (current != nullptr) {
            if (current->neighbor == v) {
                cout << "Warning: Route already exists between " << cityU 
                     << " and " << cityV << ". Updating distance to " << w << endl;
                current->distance = w;
                
                if (!direction) {
                    RouteList* reverseRoutes;
                    adj.find(v, reverseRoutes);
                    Route* revCurrent = reverseRoutes->getHead();
                    while (revCurrent != nullptr) {
                        if (revCurrent->neighbor == u) {
                            revCurrent->distance = w;
                            break;
                        }
                        revCurrent = revCurrent->next;
                    }
                }
                return;
            }
            current = current->next;
        }
        
        routes->push_back(new Route(v, w));
        if (!direction) {
            RouteList* reverseRoutes;
            adj.find(v, reverseRoutes);
            reverseRoutes->push_back(new Route(u, w));
        }
        
        cout << "Route added between " << cityU << " and " << cityV 
             << " with distance: " << w << endl;
    }
    
    void blockRoute(int u, int v) {
        string cityU, cityV;
        if (!cities.find(u, cityU) || !cities.find(v, cityV)) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }
        
        RouteList* routes;
        adj.find(u, routes);
        Route* current = routes->getHead();
        bool found = false;
        
        while (current != nullptr) {
            if (current->neighbor == v) {
                if (current->isBlocked) {
                    cout << "Route from " << cityU << " to " << cityV 
                         << " is already blocked!\n";
                } else {
                    current->isBlocked = true;
                    cout << "Route from " << cityU << " to " << cityV 
                         << " has been blocked!\n";
                }
                found = true;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "Error: No route exists from " << cityU << " to " << cityV << "!\n";
        }
    }
    
    void unblockRoute(int u, int v) {
        string cityU, cityV;
        if (!cities.find(u, cityU) || !cities.find(v, cityV)) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }
        
        RouteList* routes;
        adj.find(u, routes);
        Route* current = routes->getHead();
        bool found = false;
        
        while (current != nullptr) {
            if (current->neighbor == v) {
                if (!current->isBlocked) {
                    cout << "Route from " << cityU << " to " << cityV 
                         << " is already open!\n";
                } else {
                    current->isBlocked = false;
                    cout << "Route from " << cityU << " to " << cityV 
                         << " has been unblocked!\n";
                }
                found = true;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "Error: No route exists from " << cityU << " to " << cityV << "!\n";
        }
    }
    
    void setTraffic(int u, int v, int trafficLevel) {
        string cityU, cityV;
        if (!cities.find(u, cityU) || !cities.find(v, cityV)) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }
        
        if (trafficLevel < 0 || trafficLevel > 10) {
            cout << "Error: Traffic level must be between 0 and 10!\n";
            return;
        }
        
        RouteList* routes;
        adj.find(u, routes);
        Route* current = routes->getHead();
        bool found = false;
        
        while (current != nullptr) {
            if (current->neighbor == v) {
                current->traffic = trafficLevel;
                
                if (trafficLevel >= 8 && !current->isBlocked) {
                    current->isBlocked = true;
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cityU << " to " << cityV 
                         << ". Route AUTO-BLOCKED due to high traffic!\n";
                } else if (trafficLevel < 8 && current->isBlocked) {
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cityU << " to " << cityV 
                         << ". Route is still BLOCKED (use unblock to open).\n";
                } else {
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cityU << " to " << cityV << endl;
                }
                found = true;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "Error: No route exists from " << cityU << " to " << cityV << "!\n";
        }
    }
    
    void displayCities() {
        if (cityCount == 0) {
            cout << "No cities in the graph.\n";
            return;
        }
        cout << "\n=== Cities in Graph ===\n";
        for (int i = 0; i < cityCount; i++) {
            string cityName;
            cities.find(cityIds[i], cityName);
            cout << "ID: " << cityIds[i] << " - Name: " << cityName << endl;
        }
    }
    
    void displayGraph() {
        if (cityCount == 0) {
            cout << "No cities in the graph.\n";
            return;
        }
        cout << "\n=== Graph Structure ===\n";
        for (int i = 0; i < cityCount; i++) {
            string cityName;
            cities.find(cityIds[i], cityName);
            cout << cityName << " (ID: " << cityIds[i] << ") -> ";
            
            RouteList* routes;
            adj.find(cityIds[i], routes);
            Route* current = routes->getHead();
            
            if (current == nullptr) {
                cout << "No connections";
            } else {
                while (current != nullptr) {
                    string neighborName;
                    cities.find(current->neighbor, neighborName);
                    cout << neighborName << "(Dist:" << current->distance 
                         << ", Traffic:" << current->traffic;
                    if (current->isBlocked) {
                        cout << ", BLOCKED";
                    }
                    cout << ") ";
                    current = current->next;
                }
            }
            cout << endl;
        }
    }
    
    int calculateEffectiveCost(int distance, int traffic) {
        return distance + (distance * traffic / 10);
    }
    
    void dijkstra(int src, int dest) {
        if (cityCount == 0) {
            cout << "Error: No cities in the graph!\n";
            return;
        }
        
        string cityS, cityD;
        if (!cities.find(src, cityS)) {
            cout << "Error: Source city with ID " << src << " does not exist!\n";
            return;
        }
        if (!cities.find(dest, cityD)) {
            cout << "Error: Destination city with ID " << dest << " does not exist!\n";
            return;
        }
        
        if (src == dest) {
            cout << "\nSource and destination are the same!\n";
            cout << "City: " << cityS << " (ID: " << src << ")\n";
            cout << "Total Distance: 0 units\n";
            return;
        }
        
        IntHashTable parents;
        IntHashTable distances;
        MinHeap minHeap;
        
        for (int i = 0; i < cityCount; i++) {
            distances.insert(cityIds[i], INT_MAX);
        }
        
        distances.insert(src, 0);
        parents.insert(src, src);
        minHeap.push(0, src);
        
        while (!minHeap.empty()) {
            int nodeDist = minHeap.getTopDistance();
            int node = minHeap.getTopNode();
            minHeap.pop();
            
            int storedDist;
            distances.find(node, storedDist);
            if (nodeDist > storedDist) continue;
            
            RouteList* routes;
            adj.find(node, routes);
            Route* route = routes->getHead();
            
            while (route != nullptr) {
                int nbr = route->neighbor;
                
                if (!route->isBlocked) {
                    int effectiveCost = calculateEffectiveCost(route->distance, route->traffic);
                    int currentDist;
                    distances.find(node, currentDist);
                    int nbrDist;
                    distances.find(nbr, nbrDist);
                    
                    if (currentDist != INT_MAX && currentDist + effectiveCost < nbrDist) {
                        distances.insert(nbr, currentDist + effectiveCost);
                        parents.insert(nbr, node);
                        minHeap.push(currentDist + effectiveCost, nbr);
                    }
                }
                route = route->next;
            }
        }
        
        int finalDist;
        distances.find(dest, finalDist);
        
        if (finalDist == INT_MAX) {
            cout << "\nNo path exists between " << cityS 
                 << " (ID: " << src << ") and " << cityD 
                 << " (ID: " << dest << ").\n";
            cout << "These cities are in different disconnected components or all routes are blocked.\n";
            return;
        }
        
        IntArrayList path;
        int currentNode = dest;
        while (currentNode != src) {
            path.push_back(currentNode);
            parents.find(currentNode, currentNode);
        }
        path.push_back(src);
        
        cout << "\n=== Shortest Path Result ===\n";
        cout << "From: " << cityS << " (ID: " << src << ")\n";
        cout << "To: " << cityD << " (ID: " << dest << ")\n";
        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            string cityName;
            cities.find(path.get(i), cityName);
            cout << cityName;
            if (i > 0) cout << " -> ";
        }
        cout << "\nTotal Effective Cost (with traffic): " << finalDist << " units\n";
        cout << "Number of hops: " << path.size() - 1 << endl;
    }
    
    void loadSampleData() {
        addCity("City1");
        addCity("City2");
        addCity("City3");
        addCity("City4");
        addCity("City5");
        addCity("City6");
        addCity("City7");
        addCity("City8");
        
        addEdge(1, 3, 2, false);
        addEdge(2, 3, 4, false);
        addEdge(3, 5, 6, false);
        addEdge(3, 4, 3, false);
        addEdge(4, 6, 2, false);
        addEdge(5, 6, 1, false);
        addEdge(7, 8, 1, false);
        
        cout << "\nSample data loaded successfully!\n";
    }
    
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        
        if (!outFile.is_open()) {
            cout << "Error: Unable to create/open file '" << filename << "'!\n";
            return;
        }
        
        outFile << "NEXT_ID " << nextCityId << endl;
        
        outFile << "CITIES " << cityCount << endl;
        for (int i = 0; i < cityCount; i++) {
            string cityName;
            cities.find(cityIds[i], cityName);
            outFile << cityIds[i] << " " << cityName << endl;
        }
        
        int edgeCount = 0;
        for (int i = 0; i < cityCount; i++) {
            RouteList* routes;
            adj.find(cityIds[i], routes);
            Route* current = routes->getHead();
            while (current != nullptr) {
                edgeCount++;
                current = current->next;
            }
        }
        
        outFile << "EDGES " << edgeCount << endl;
        for (int i = 0; i < cityCount; i++) {
            RouteList* routes;
            adj.find(cityIds[i], routes);
            Route* current = routes->getHead();
            while (current != nullptr) {
                outFile << cityIds[i] << " " << current->neighbor << " " 
                       << current->distance << " " << current->traffic << " " 
                       << current->isBlocked << endl;
                current = current->next;
            }
        }
        
        outFile.close();
        cout << "Graph data saved successfully to '" << filename << "'!\n";
        cout << "Saved " << cityCount << " cities and " << edgeCount << " routes.\n";
    }
    
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        
        if (!inFile.is_open()) {
            cout << "Error: Unable to open file '" << filename << "'!\n";
            return;
        }
        
        clearGraph();
        
        string line, keyword;
        int count;
        
        inFile >> keyword >> nextCityId;
        inFile.ignore();
        
        inFile >> keyword >> count;
        inFile.ignore();
        
        for (int i = 0; i < count; i++) {
            getline(inFile, line);
            istringstream iss(line);
            int id;
            string name;
            
            iss >> id;
            getline(iss, name);
            
            if (!name.empty() && name[0] == ' ') {
                name = name.substr(1);
            }
            
            cities.insert(id, name);
            cityIds[cityCount++] = id;
            adj.insert(id, new RouteList());
        }
        
        inFile >> keyword >> count;
        inFile.ignore();
        
        for (int i = 0; i < count; i++) {
            int u, v, distance, traffic;
            bool isBlocked;
            inFile >> u >> v >> distance >> traffic >> isBlocked;
            inFile.ignore();
            
            RouteList* routes;
            adj.find(u, routes);
            Route* route = new Route(v, distance);
            route->traffic = traffic;
            route->isBlocked = isBlocked;
            routes->push_back(route);
        }
        
        inFile.close();
        cout << "Graph data loaded successfully from '" << filename << "'!\n";
        cout << "Loaded " << cityCount << " cities and " << count << " routes.\n";
    }
    
    void clearGraph() {
        cities.clear();
        adj.clear();
        cityCount = 0;
        nextCityId = 1;
        cout << "Graph cleared successfully!\n";
    }
};

void displayMenu() {
    cout << "\n================================================\n";
    cout << "   SHORTEST PATH FINDER - DIJKSTRA\n";
    cout << "   (with Traffic Management & Route Blocking)\n";
    cout << "================================================\n";
    cout << "1.  Add a City\n";
    cout << "2.  Add a Route (Edge)\n";
    cout << "3.  Display All Cities\n";
    cout << "4.  Display Graph Structure\n";
    cout << "5.  Find Shortest Path\n";
    cout << "6.  Block a Route\n";
    cout << "7.  Unblock a Route\n";
    cout << "8.  Set Traffic Level on Route\n";
    cout << "9.  Load Sample Data\n";
    cout << "10. Save Graph to File\n";
    cout << "11. Load Graph from File\n";
    cout << "12. Clear Graph\n";
    cout << "13. Exit\n";
    cout << "================================================\n";
    cout << "Note: Traffic level 0-7 (normal), 8-10 (auto-blocks)\n";
    cout << "      Cost = Distance + (Distance x Traffic x 10%)\n";
    cout << "================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    Graph g;
    int choice;
    
    do {
        displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number between 1 and 13.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                string name;
                cout << "Enter City Name: ";
                cin.ignore();
                getline(cin, name);
                
                size_t start = name.find_first_not_of(" \t\n\r");
                size_t end = name.find_last_not_of(" \t\n\r");
                if (start != string::npos && end != string::npos) {
                    name = name.substr(start, end - start + 1);
                }

                // yahan name trim hoke jaiga
                g.addCity(name);
                break;
            }
            case 2: {
                int u, v, w;
                char dirChoice;
                
                cout << "Enter Source City ID: ";
                if (!(cin >> u)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                
                cout << "Enter Destination City ID: ";
                if (!(cin >> v)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                
                cout << "Enter Distance: ";
                if (!(cin >> w)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                
                cout << "Is it a one-way route? (y/n): ";
                cin >> dirChoice;
                bool direction = (dirChoice == 'y' || dirChoice == 'Y');
                g.addEdge(u, v, w, direction);
                break;
            }
            case 3:
                g.displayCities();
                break;
            case 4:
                g.displayGraph();
                break;
            case 5: {
                int src, dest;
                cout << "Enter Source City ID: ";
                if (!(cin >> src)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                cout << "Enter Destination City ID: ";
                if (!(cin >> dest)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                g.dijkstra(src, dest);
                break;
            }
            case 6: {
                int u, v;
                cout << "Enter Source City ID: ";
                if (!(cin >> u)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                cout << "Enter Destination City ID: ";
                if (!(cin >> v)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                g.blockRoute(u, v);
                break;
            }
            case 7: {
                int u, v;
                cout << "Enter Source City ID: ";
                if (!(cin >> u)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                cout << "Enter Destination City ID: ";
                if (!(cin >> v)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                g.unblockRoute(u, v);
                break;
            }
            case 8: {
                int u, v, traffic;
                cout << "Enter Source City ID: ";
                if (!(cin >> u)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                cout << "Enter Destination City ID: ";
                if (!(cin >> v)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                cout << "Enter Traffic Level (0-10): ";
                if (!(cin >> traffic)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input!\n";
                    break;
                }
                g.setTraffic(u, v, traffic);
                break;
            }
            case 9:
                g.loadSampleData();
                break;
            case 10: {
                string filename;
                cout << "Enter filename to save (e.g., graph.txt): ";
                cin >> filename;
                
                if (filename.empty()) {
                    filename = "graph.txt";
                    cout << "Using default filename: graph.txt\n";
                }
                
                g.saveToFile(filename);
                break;
            }
            case 11: {
                string filename;
                cout << "Enter filename to load (e.g., graph.txt): ";
                cin >> filename;
                
                if (filename.empty()) {
                    filename = "graph.txt";
                    cout << "Using default filename: graph.txt\n";
                }
                
                g.loadFromFile(filename);
                break;
            }
            case 12: {
                char confirm;
                cout << "Are you sure you want to clear the entire graph? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    g.clearGraph();
                } else {
                    cout << "Clear operation cancelled.\n";
                }
                break;
            }
            case 13:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 13);
    
    return 0;
}