#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <climits>
#include <string>
using namespace std;

class City {
public:
    int distance;
    string name;

    City(string name = "", int distance = INT_MAX) : name(name), distance(distance) {}
};

class Route {
public:
    int neighbor;
    int distance;
    int traffic;
    bool isBlocked;
    
    Route(int n, int d) : neighbor(n), distance(d), traffic(0), isBlocked(false) {}
};

class MinHeap {
private:
    vector<pair<int, int>> heap;
    unordered_map<int, int> indexMap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swap(int i, int j) {
        indexMap[heap[i].second] = j;
        indexMap[heap[j].second] = i;
        pair<int, int> temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)].first > heap[i].first) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left].first < heap[minIndex].first) {
            minIndex = left;
        }
        if (right < heap.size() && heap[right].first < heap[minIndex].first) {
            minIndex = right;
        }

        if (i != minIndex) {
            swap(i, minIndex);
            heapifyDown(minIndex);
        }
    }

public:
    void push(int distance, int node) {
        if (indexMap.find(node) != indexMap.end()) {
            int idx = indexMap[node];
            if (distance < heap[idx].first) {
                heap[idx].first = distance;
                heapifyUp(idx);
            }
            return;
        }

        heap.push_back({distance, node});
        int idx = heap.size() - 1;
        indexMap[node] = idx;
        heapifyUp(idx);
    }

    pair<int, int> top() {
        if (heap.empty()) {
            return {-1, -1};
        }
        return heap[0];
    }

    void pop() {
        if (heap.empty()) return;

        indexMap.erase(heap[0].second);

        if (heap.size() == 1) {
            heap.pop_back();
            return;
        }

        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            indexMap[heap[0].second] = 0;
            heapifyDown(0);
        }
    }

    bool empty() {
        return heap.size() == 0;
    }

    int size() {
        return heap.size();
    }
};

class Graph {
private:
    int nextCityId;

public:
    unordered_map<int, list<Route>> adj;
    unordered_map<int, City> cities;

    Graph() : nextCityId(1) {}

    int addCity(const string& name) {
        if (name.empty()) {
            cout << "Error: City name cannot be empty!\n";
            return -1;
        }
        
        for (auto& city : cities) {
            if (city.second.name == name) {
                cout << "Error: City '" << name << "' already exists with ID: " << city.first << endl;
                return -1;
            }
        }
        
        int id = nextCityId++;
        cities[id] = City(name);
        cout << "City '" << name << "' added with ID: " << id << endl;
        return id;
    }

    void addEdge(int u, int v, int w, bool direction) {
        if (cities.find(u) == cities.end()) {
            cout << "Error: Source city with ID " << u << " does not exist!\n";
            return;
        }
        if (cities.find(v) == cities.end()) {
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
        
        for (auto& route : adj[u]) {
            if (route.neighbor == v) {
                cout << "Warning: Route already exists between " << cities[u].name 
                     << " and " << cities[v].name << ". Updating distance to " << w << endl;
                route.distance = w;
                if (!direction) {
                    for (auto& reverseRoute : adj[v]) {
                        if (reverseRoute.neighbor == u) {
                            reverseRoute.distance = w;
                            break;
                        }
                    }
                }
                return;
            }
        }
        
        adj[u].push_back(Route(v, w));
        if (!direction) {
            adj[v].push_back(Route(u, w));
        }
        cout << "Route added between " << cities[u].name << " and " << cities[v].name 
             << " with distance: " << w << endl;
    }

    void blockRoute(int u, int v) {
        if (cities.find(u) == cities.end() || cities.find(v) == cities.end()) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }

        bool found = false;
        for (auto& route : adj[u]) {
            if (route.neighbor == v) {
                if (route.isBlocked) {
                    cout << "Route from " << cities[u].name << " to " << cities[v].name 
                         << " is already blocked!\n";
                } else {
                    route.isBlocked = true;
                    cout << "Route from " << cities[u].name << " to " << cities[v].name 
                         << " has been blocked!\n";
                }
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Error: No route exists from " << cities[u].name << " to " << cities[v].name << "!\n";
        }
    }

    void unblockRoute(int u, int v) {
        if (cities.find(u) == cities.end() || cities.find(v) == cities.end()) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }

        bool found = false;
        for (auto& route : adj[u]) {
            if (route.neighbor == v) {
                if (!route.isBlocked) {
                    cout << "Route from " << cities[u].name << " to " << cities[v].name 
                         << " is already open!\n";
                } else {
                    route.isBlocked = false;
                    cout << "Route from " << cities[u].name << " to " << cities[v].name 
                         << " has been unblocked!\n";
                }
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Error: No route exists from " << cities[u].name << " to " << cities[v].name << "!\n";
        }
    }

    void setTraffic(int u, int v, int trafficLevel) {
        if (cities.find(u) == cities.end() || cities.find(v) == cities.end()) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }

        if (trafficLevel < 0 || trafficLevel > 10) {
            cout << "Error: Traffic level must be between 0 and 10!\n";
            return;
        }

        bool found = false;
        for (auto& route : adj[u]) {
            if (route.neighbor == v) {
                route.traffic = trafficLevel;
                
                if (trafficLevel >= 8 && !route.isBlocked) {
                    route.isBlocked = true;
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cities[u].name << " to " << cities[v].name 
                         << ". Route AUTO-BLOCKED due to high traffic!\n";
                } else if (trafficLevel < 8 && route.isBlocked) {
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cities[u].name << " to " << cities[v].name 
                         << ". Route is still BLOCKED (use unblock to open).\n";
                } else {
                    cout << "Traffic set to " << trafficLevel << " on route from " 
                         << cities[u].name << " to " << cities[v].name << endl;
                }
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Error: No route exists from " << cities[u].name << " to " << cities[v].name << "!\n";
        }
    }

    void displayCities() {
        if (cities.empty()) {
            cout << "No cities in the graph.\n";
            return;
        }
        cout << "\n=== Cities in Graph ===\n";
        for (auto& city : cities) {
            cout << "ID: " << city.first << " - Name: " << city.second.name << endl;
        }
    }

    void displayGraph() {
        if (cities.empty()) {
            cout << "No cities in the graph.\n";
            return;
        }
        cout << "\n=== Graph Structure ===\n";
        for (auto& city : cities) {
            cout << cities[city.first].name << " (ID: " << city.first << ") -> ";
            if (adj[city.first].empty()) {
                cout << "No connections";
            } else {
                for (auto& route : adj[city.first]) {
                    cout << cities[route.neighbor].name << "(Dist:" << route.distance 
                         << ", Traffic:" << route.traffic;
                    if (route.isBlocked) {
                        cout << ", BLOCKED";
                    }
                    cout << ") ";
                }
            }
            cout << endl;
        }
    }

    int calculateEffectiveCost(int distance, int traffic) {
       return distance + (traffic * 10); 
    }

    void dijkstra(int src, int dest) {
        if (cities.empty()) {
            cout << "Error: No cities in the graph!\n";
            return;
        }
        if (cities.find(src) == cities.end()) {
            cout << "Error: Source city with ID " << src << " does not exist!\n";
            return;
        }
        if (cities.find(dest) == cities.end()) {
            cout << "Error: Destination city with ID " << dest << " does not exist!\n";
            return;
        }
        if (src == dest) {
            cout << "\nSource and destination are the same!\n";
            cout << "City: " << cities[src].name << " (ID: " << src << ")\n";
            cout << "Total Distance: 0 units\n";
            return;
        }

        unordered_map<int, int> parents;
        unordered_map<int, int> distances;
        MinHeap minHeap;

        for (auto& city : cities) {
            distances[city.first] = INT_MAX;
        }

        distances[src] = 0;
        parents[src] = src;
        minHeap.push(0, src);

        while (!minHeap.empty()) {
            pair<int, int> current = minHeap.top();
            int nodeDist = current.first;
            int node = current.second;
            minHeap.pop();

            if (nodeDist > distances[node]) continue;

            for (auto& route : adj[node]) {
                int nbr = route.neighbor;
                
                if (route.isBlocked) continue;
                
                int effectiveCost = calculateEffectiveCost(route.distance, route.traffic);

                if (distances[node] != INT_MAX && distances[node] + effectiveCost < distances[nbr]) {
                    distances[nbr] = distances[node] + effectiveCost;
                    parents[nbr] = node;
                    minHeap.push(distances[nbr], nbr);
                }
            }
        }

        if (distances[dest] == INT_MAX) {
            cout << "\nNo path exists between " << cities[src].name 
                 << " (ID: " << src << ") and " << cities[dest].name 
                 << " (ID: " << dest << ").\n";
            cout << "These cities are in different disconnected components or all routes are blocked.\n";
            return;
        }

        vector<int> path;
        int currentNode = dest;
        
        while (currentNode != src) {
            path.push_back(currentNode);
            currentNode = parents[currentNode];
        }
        path.push_back(src);

        cout << "\n=== Shortest Path Result ===\n";
        cout << "From: " << cities[src].name << " (ID: " << src << ")\n";
        cout << "To: " << cities[dest].name << " (ID: " << dest << ")\n";
        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << cities[path[i]].name;
            if (i > 0) cout << " -> ";
        }
        cout << "\nTotal Effective Cost (with traffic): " << distances[dest] << " units\n";
        cout << "Number of hops: " << path.size() - 1 << endl;
    }

    void loadSampleData() {
        addCity("karachi");
        addCity("hyderabad");
        addCity("sukkur");
        addCity("islamabad");
        addCity("lahore");
        addCity("wazirabad");

        addEdge(1, 2, 150, false);
        addEdge(1, 5, 220, false);
        addEdge(2, 3, 120, false);
        addEdge(3, 4, 200, false);
        addEdge(4, 6, 210, false);
        addEdge(6, 5, 180, false);
        addEdge(4, 5, 80, false);
        
        cout << "\nSample data loaded successfully!\n";
    }

    void clearGraph() {
        cities.clear();
        adj.clear();
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
    cout << "10. Clear Graph\n";
    cout << "11. Exit\n";
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
            cout << "Invalid input! Please enter a number between 1 and 11.\n";
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
                
                g.addCity(name);
                break;
            }
            case 2: {
                int u, v, w;
                char dirChoice;
                
                if (g.cities.empty()) {
                    cout << "Error: No cities available. Please add cities first!\n";
                    break;
                }
                
                cout << "Enter Source City ID: ";
                if (!(cin >> u)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
                    break;
                }
                
                cout << "Enter Destination City ID: ";
                if (!(cin >> v)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
                    break;
                }
                
                cout << "Enter Distance: ";
                if (!(cin >> w)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
                    break;
                }
                
                cout << "Is it a one-way route? (y/n): ";
                cin >> dirChoice;
                bool direction = (dirChoice == 'y' || dirChoice == 'Y');
                g.addEdge(u, v, w, direction);
                break;
            }
            case 3: {
                g.displayCities();
                break;
            }
            case 4: {
                g.displayGraph();
                break;
            }
            case 5: {
                int src, dest;
                
                if (g.cities.empty()) {
                    cout << "Error: No cities available. Please add cities first!\n";
                    break;
                }
                
                cout << "Enter Source City ID: ";
                if (!(cin >> src)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
                    break;
                }
                
                cout << "Enter Destination City ID: ";
                if (!(cin >> dest)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
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
            case 9: {
                g.loadSampleData();
                break;
            }
            case 10: {
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
            case 11: {
                cout << "Exiting program. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
            }
        }
    } while (choice != 11);

    return 0;
}