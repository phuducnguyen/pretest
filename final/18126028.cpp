#include "func.cpp"
#include <set>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <bitset>

using namespace std;

struct Graph {
	map<string, int> name2id;
	vector<string> id2name;
	vector<vector<int> > adj;
	vector<Flight> base;
};

struct Node {
	vector<char> A;
	Node* left; 
	Node* right;
};

Flight parseFlightData(string buff) {
	stringstream ss(buff);
	Flight newFlight;
	getline(ss, buff, '\"');
	getline(ss, newFlight.src, ',');
	getline(ss, newFlight.des, '\"');

	getline(ss, buff, '\"');
	getline(ss, newFlight.airliner, '\"');

	getline(ss, buff, '\"');
	getline(ss, buff, ' ');
	newFlight.bus = stoi(buff);

	getline(ss, buff, ',');
	getline(ss, buff, ' ');
	getline(ss, buff, ' ');
	newFlight.eco = stoi(buff);

	getline(ss, buff, '\"');
	getline(ss, buff, '\"');

	getline(ss, buff, ' ');
	newFlight.hours = stoi(buff);
	getline(ss, buff, ' ');
	getline(ss, buff, ' ');

	if (buff == "hours\"]}" || buff == "hour\"]}")
		newFlight.mins = 0;
	else if (buff == "minutes\"]}" || buff == "minute\"]}") {
		newFlight.mins = newFlight.hours;
		newFlight.hours = 0;
	}
	else
		newFlight.mins = stoi(buff);

	return newFlight;
}

Flight getFlight(string buff) {
	stringstream ss(buff);
	Flight newFlight;
	getline(ss, buff, '\"');
	getline(ss, newFlight.src, ',');
	getline(ss, newFlight.des, '\"');

	getline(ss, buff, '\"');
	getline(ss, newFlight.airliner, '\"');

	getline(ss, buff, '\"');
	getline(ss, buff, ' ');
	newFlight.bus = stoi(buff);

	getline(ss, buff, ',');
	getline(ss, buff, ' ');
	getline(ss, buff, ' ');
	newFlight.eco = stoi(buff);

	getline(ss, buff, '\"');
	getline(ss, buff, '\"');

	getline(ss, buff, ' ');
	newFlight.hours = stoi(buff);
	getline(ss, buff, ' ');
	getline(ss, buff, ' ');

	if (buff == "hours\"]}" || buff == "hour\"]}")
		newFlight.mins = 0;
	else if (buff == "minutes\"]}" || buff == "minute\"]}") {
		newFlight.mins = newFlight.hours;
		newFlight.hours = 0;
	}
	else
		newFlight.mins = stoi(buff);

	return newFlight;
}

vector<Flight> getAllFlights(const string& flightdata_filename) {
	vector<Flight> fList;
	ifstream is(flightdata_filename);

	if (!is.is_open()) {
        cerr << "Failed to open file: " << flightdata_filename << endl;
        return fList;
    }

	string buff;
	while (getline(is, buff) && !buff.empty()) {
		fList.push_back(parseFlightData(buff));
	}

	is.close();
	return fList;
}

Graph createGraph(const vector<Flight>& fList) {
    Graph g;

    g.base = fList;

    // Create a set of nodes (separated locations), sorted lexicographically
    set<string> nodes_set;
    for (const Flight& flight : g.base) {
        nodes_set.insert(flight.src);
        nodes_set.insert(flight.des);
    }

    int total_nodes = nodes_set.size();

    // Create id2name vector
    g.id2name.assign(nodes_set.begin(), nodes_set.end());

    // Create name2id map directly using range-based for loop
    for (int i = 0; i < total_nodes; i++) {
        g.name2id[g.id2name[i]] = i;
    }

    // Create adjacency matrix
    g.adj.assign(total_nodes, vector<int>(total_nodes, -1)); // non-existing edges

    for (int i = 0; i < g.base.size(); i++) {
        int src = g.name2id[g.base[i].src];
        int des = g.name2id[g.base[i].des];

        g.adj[src][des] = 1;
        g.adj[des][src] = 1;
    }

    return g;
}

vector<Flight> readFlightDataFromFile(const string& filename) {
    vector<Flight> flights;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return flights;
    }

    string line;
    while (getline(file, line)) {
        flights.push_back(parseFlightData(line));
    }

    file.close();
    return flights;
}

vector<Flight> filterFlightsByDuration(const vector<Flight>& allFlights, int maxDuration) {
    vector<Flight> filteredFlights;
    for (const Flight& flight : allFlights) {
        if (flight.hours < maxDuration) {
            filteredFlights.push_back(flight);
        }
    }
    return filteredFlights;
}

void printDirectFlightsToCountry(const vector<Flight>& flights, const string& country, int maxCount = 10) {
    int count = 0;
    for (const Flight& flight : flights) {
        string otherLocation = (flight.src == country) ? flight.des : flight.src;
        cout << otherLocation;
        count++;

        // Remove semicolon in the end
        if (count < maxCount) {
            cout << ", ";
        } else {
            break;
        }
    }
    cout << endl;
}

void sol_1(string flightdata_filename, const int maxFlightDuration, const string& country1) {
	vector<Flight> allFlights = readFlightDataFromFile(flightdata_filename);
    vector<Flight> filteredFlights = filterFlightsByDuration(allFlights, maxFlightDuration);
    Graph flightGraph = createGraph(filteredFlights);

    cout << country1 << ": ";
    printDirectFlightsToCountry(filteredFlights, country1);
}

void sol_2(Graph& graph) {
	// The total number of nodes (countries)
    int total_nodes = graph.id2name.size();
    vector<int> degree(total_nodes, 0);	// Store the degree of each node

    // Calculate the degree of each node by iterating through flights
    for (const Flight& flight : graph.base) {
        int src = graph.name2id[flight.src];
        int des = graph.name2id[flight.des];
        degree[src]++;
        degree[des]++;
    }

    // Count the number of nodes with even degree,
    // then find the maximum even degree
    int num_nodes_even_degree = 0;
    int max_even_degree = 0;
    for (int i = 0; i < total_nodes; i++) {
        if (degree[i] % 2 == 0) {
            num_nodes_even_degree++;
            max_even_degree = max(max_even_degree, degree[i]);
        }
    }

    // Build a comma-separated list of countries
    // with the highest even degree
    string even_degree_list = "";
    for (int i = 0; i < total_nodes; i++) {
        if (degree[i] == max_even_degree) {
            if (!even_degree_list.empty()) {
                even_degree_list += ", ";
            }
            even_degree_list += graph.id2name[i];
        }
    }

    // Result
    cout << num_nodes_even_degree << " even vertices, the highest: " << even_degree_list << endl;
}

// Graph Theory
// Function to check if two countries are directly connected by a flight
// bool isConnected(Graph& graph, const string& country1, const string& country2) {
//     int country1_idx = graph.name2id[country1];
//     int country2_idx = graph.name2id[country2];

//     return graph.adj[country1_idx][country2_idx] != -1;
// }

// void ex3(Graph& graph, const string& country1, const string& country2) {
//     vector<vector<string>> developed_areas;

//     int country1_idx = graph.name2id[country1];
//     int country2_idx = graph.name2id[country2];

//     if (!isConnected(graph, country1_idx, country2_idx)) {
//         cout << "0 quad";
//         return;
//     }

//     for (int i = 0; i < graph.id2name.size(); i++) {
//         string curr_country_i = graph.id2name[i];
//         int curr_country_i_idx = graph.name2id[curr_country_i];

//         if (isConnected(graph, country1_idx, curr_country_i_idx) && 
//             isConnected(graph, country2_idx, curr_country_i_idx)) {

//             for (int j = i + 1; j < graph.id2name.size(); j++) {
//                 string curr_country_j = graph.id2name[j];
//                 int curr_country_j_idx = graph.name2id[curr_country_j];

//                 if (isConnected(graph, country1_idx, curr_country_j_idx) && 
//                     isConnected(graph, country2_idx, curr_country_j_idx) &&
//                     isConnected(graph, curr_country_i_idx, curr_country_j_idx)) {

//                     developed_areas.push_back({ country1, country2, curr_country_i, curr_country_j });
//                 }
//             }
//         }
//     }

//     sort(developed_areas.begin(), developed_areas.end());

//     cout << developed_areas.size() << " quads\n";
//     for (int i = 0; i < min(3, int(developed_areas.size())); i++) {
//         for (int j = 0; j < 3; j++) {
//             cout << developed_areas[i][j];
//             if (j < 2) {
//                 cout << ", ";
//             }
//         }
//         cout << ", " << developed_areas[i][3] << endl;
//     }
// }

bool isConnected(const Graph& graph, int country1_idx, int country2_idx, int curr_country_idx) {
    vector<bool> visited(graph.id2name.size(), false);

    function<bool(int, int)> dfsIsConnected = [&](int source, int target) {
        if (source == target) {
            return true;
        }

        visited[source] = true;

        for (int neighbor : graph.adj[source]) {
            if (!visited[neighbor]) {
                if (dfsIsConnected(neighbor, target)) {
                    return true;
                }
            }
        }

        return false;
    };

    return dfsIsConnected(country1_idx, curr_country_idx) &&
           dfsIsConnected(country2_idx, curr_country_idx);
}

vector<vector<string>> find_developed_areas(Graph& graph, const string& country1, const string& country2) {
    vector<vector<string>> developed_areas;

    const int MAX_N = 100; // Adjust this based on your graph size
    bitset<MAX_N> isConnected[MAX_N];

    // Precompute connectivity matrix
    for (int i = 0; i < graph.id2name.size(); i++) {
        for (int j = i + 1; j < graph.id2name.size(); j++) {
            isConnected[i][j] = isConnected[j][i] = isConnected(graph, i, j);
        }
    }

    int country1_idx = graph.name2id[country1];
    int country2_idx = graph.name2id[country2];

    for (int i = 0; i < graph.id2name.size(); i++) {
        if (isConnected[country1_idx][i] && isConnected[country2_idx][i]) {
            string curr_country_i = graph.id2name[i];

            for (int j = i + 1; j < graph.id2name.size(); j++) {
                if (isConnected[country1_idx][j] && isConnected[country2_idx][j] && isConnected[i][j]) {
                    vector<string> area = {country1, country2, curr_country_i, graph.id2name[j]};
                    sort(area.begin(), area.end());
                    developed_areas.push_back(area);
                }
            }
        }
    }

    sort(developed_areas.begin(), developed_areas.end());

    return developed_areas;
}
/*
vector<vector<string>> find_developed_areas(Graph& graph, const string& country1, const string& country2) {
	vector<vector<string>> developed_areas;


	queue<string> queue;
	queue.push(country1);
	queue.push(country2);

	// Check visited
	unordered_set<int> visited;
	visited.insert(graph.name2id[country1]);
	visited.insert(graph.name2id[country2]);

	while (!queue.empty()) {
		// Lấy quốc gia đầu tiên trong hàng đợi.
		string country = queue.front();
		queue.pop();

		// Tìm tất cả các quốc gia có chuyến bay trực tiếp đến country.
		int country_id = graph.name2id[country];
		vector<int> neighbors = graph.adj[country_id];
		for (int neighbor : neighbors) {
	  		if (visited.count(neighbor) == 0) {
	    	visited.insert(neighbor);
	    	queue.push(graph.id2name[neighbor]);
	  		}
		}

		// Nếu country có chuyến bay trực tiếp đến cả country1 và country2:
		if (has_direct_flight_to_both(graph, country_id, graph.name2id[country1], graph.name2id[country2])) {
	        vector<string> area;
		    area.push_back(graph.id2name[graph.name2id[country1]]);
		    area.push_back(graph.id2name[graph.name2id[country2]]);
		    area.push_back(graph.id2name[country_id]);
		    
		    developed_areas.push_back(area);
		    sort(developed_areas.back().begin(), developed_areas.back().end());
      	}
	}
	return developed_areas;
}
*/
void sol_3(Graph& graph, const string& country1, const string& country2) {
	vector<vector<string>> developed_areas = find_developed_areas(graph, country1, country2);

  	// In ra tổng số khu vực phát triển và 3 khu vực phát triển bất kỳ trong tổng số các khu vực nói trên.
  	cout << developed_areas.size() << " quads\n";
  		for (int i = 0; i < min(3, int(developed_areas.size())); i++) {
    		for (int j = 0; j < 3; j++) {
      			cout << developed_areas[i][j];
      			if (j < 2) {
        			cout << ", ";
      			}
    		}
    	cout << ", " << developed_areas[i][3] << endl;
  	}
}


void todo(string flightdata_filename, string data_filename) {
	// Process data for Graph
	string country1, country2;
	ifstream is(data_filename);

	string buff; is >> buff;
	stringstream ss(buff);
	
	getline(ss, country1, ',');
	getline(ss, country2);
    ifstream inputFile(flightdata_filename);

    int maxFlightDuration = 10;

    
    // string flightDataFilename = "g1.v2.jl";
    // string country1 = "Vietnam";
    
    /* Q1: Hãy tìm các chuyến bay có thời gian bay ít hơn 10 tiếng, 
    	và xây dựng 1 đồ thị G vô hướng biểu diễn các chuyến bay vừa tìm được. 
    	In ra 10 nước bất kì có đường bay trực tiếp đến nước country1 được cho trước.
    */
   	// sol_1(flightdata_filename, maxFlightDuration, country1);
    vector<Flight> allFlights = readFlightDataFromFile(flightdata_filename);
    vector<Flight> filteredFlights = filterFlightsByDuration(allFlights, maxFlightDuration);
    Graph flightGraph = createGraph(filteredFlights);

    cout << country1 << ": ";
    printDirectFlightsToCountry(filteredFlights, country1);
    cout << "=====\n";

    /* Q2: Viết hàm đếm số nước có bậc chẵn và in ra nước có bậc chẵn cao nhất. 
    	Nếu có nhiều nước có cùng bậc chẵn cao nhất, in ra tất cả các nước này. 
    */
    sol_2(flightGraph);
    cout << "=====\n";

    /* Q3: In ra tổng số khu vực phát triển và 
    	3 khu vực phát triển bất kì trong tổng số các khu vực nói trên. 
    */
    sol_3(flightGraph, country1, country2);
    cout << "=====\n";

    // inputFile.close();
}

void main_debug() {
    todo("g1.v2.jl", "data.txt");
    
	// Comment after debugging
	cout << "[DEBUG MODE]" << endl;


}

/*
Graph graph; // Assuming you have created an instance of the Graph structure

// Accessing name2id (a map)
int countryId = graph.name2id["CountryName"];

// Accessing id2name (a vector)
string countryName = graph.id2name[countryId];

// Accessing adj (a vector of vectors)
vector<int>& neighbors = graph.adj[countryId];

// Accessing base (a vector)
Flight flight = graph.base[index];
 */

// BFS
// vector<vector<string>> find_developed_areas(Graph& graph, const string& country1, const string& country2) {
//  vector<vector<string>> developed_areas;
//  developed_areas.reserve(10); // Reserve enough memory for 10 elements.

//  queue<string> queue;
//  queue.push(country1);
//  queue.push(country2);

//  // Check visited
//  unordered_set<int> visited;
//  visited.insert(graph.name2id[country1]);
//  visited.insert(graph.name2id[country2]);

//  while (!queue.empty()) {
//      // Lấy quốc gia đầu tiên trong hàng đợi.
//      string country = queue.front();
//      queue.pop();

//      // Tìm tất cả các quốc gia có chuyến bay trực tiếp đến country.
//      int country_id = graph.name2id[country];
//      vector<int> neighbors = graph.adj[country_id];
//      for (int neighbor : neighbors) {
//          if (visited.count(neighbor) == 0) {
//          visited.insert(neighbor);
//          queue.push(graph.id2name[neighbor]);
//          }
//      }

//      // Nếu country có chuyến bay trực tiếp đến cả country1 và country2:
//      if (has_direct_flight_to_both(graph, country_id, graph.name2id[country1], graph.name2id[country2])) {
//          vector<string> area;
//          area.push_back(graph.id2name[graph.name2id[country1]]);
//          area.push_back(graph.id2name[graph.name2id[country2]]);
//          area.push_back(graph.id2name[country_id]);
            
//          developed_areas.push_back(area);
//          sort(developed_areas.back().begin(), developed_areas.back().end());
//          }
//  }
//  return developed_areas;
// }
// 

    // // Process data for Graph
    // string country1, country2;
    // ifstream is("data.txt");
    // string buff; is >> buff;
    // stringstream ss(buff);
    // getline(ss, country1, ',');
    // getline(ss, country2);
    // ifstream inputFile("g1.v2.jl");
    
    // if (!inputFile.is_open()) {
    //     cerr << "Failed to open file." << endl;
    //     return;
    // }

    // string line;
    // while (getline(inputFile, line)) {
    //  // Parse the flight data
    //  Flight flight = getFlight(line);

    //  // Print the parsed flight information
    //  cout << flight.src << " " << flight.des << " " << flight.airliner << " "  << flight.bus  << " "
    //      << flight.eco << " " << flight.hours << " " << flight.mins;
    //  cout << endl;
    // }