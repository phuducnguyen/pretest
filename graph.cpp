#include "func.cpp"
#include <set>
// ====================== STRUCTS =======================

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

// ======================== GRAPH ============================

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


	// 92. {"St. Barth,Anguilla": ["Airbus A300", "20 business, 355 economy", "34 minutes"]}
	// 123. {"Ghana,Vietnam": ["Yakovlev Yak-42-90", "51 business, 45 economy", "15 hours"]}

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

vector<Flight> get_all_flights_list(string flightdata_filename) {
	vector<Flight> resList;
	ifstream is(flightdata_filename);
	string buff;

	while (getline(is, buff) && buff != "")
		resList.push_back(getFlight(buff));

	return resList;
}

Graph createGraph(const vector<Flight>& flights_list) {
	Graph res_graph;

	res_graph.base = flights_list;

	// create list of nodes (seperated locations), sort them lexicographically
	set<string> nodes_set;
	for (int i = 0; i < res_graph.base.size(); i++) {
		nodes_set.insert(res_graph.base[i].src);
		nodes_set.insert(res_graph.base[i].des);
	}
	int total_nodes = nodes_set.size(); // total numbers of nodes

	// create id2name vector
	res_graph.id2name = vector<string>(total_nodes);
	copy(nodes_set.begin(), nodes_set.end(), res_graph.id2name.begin());

	// create name2id map
	for (int i = 0; i < total_nodes; i++)
		res_graph.name2id[res_graph.id2name[i]] = i;

	// create adj_mat
	// undirected graph, weight of an edge of 2 nodes is the index of the flights between that 2 nodes in flights_vector
	res_graph.adj.resize(total_nodes, vector<int>(total_nodes, -1)); // -1 indicates edge[u,v] doesn't exist
	for (int i = 0; i < res_graph.base.size(); i++) {
		int src = res_graph.name2id[res_graph.base[i].src];
		int des = res_graph.name2id[res_graph.base[i].des];

		res_graph.adj[src][des] = i;
		res_graph.adj[des][src] = i;
	}

	return res_graph;
}

// CAU 1
vector<Flight> get_lessThan_10hours_flights(const vector<Flight>& all_flights_list) {
	vector<Flight> res;
	for (auto flight : all_flights_list)
		if (flight.hours < 10)
			res.push_back(flight);

	return res;
}

Graph ex1(string flightdata_filename, string country1) {
	vector<Flight> lessThan_10hours_flights =
		get_lessThan_10hours_flights(
			get_all_flights_list(flightdata_filename));

	Graph res = createGraph(lessThan_10hours_flights);

	cout << country1 << ": ";
	int count = 0;
	for (int i = 0; i < lessThan_10hours_flights.size(); i++) {
		if (lessThan_10hours_flights[i].src == country1)
			count++, cout << lessThan_10hours_flights[i].des;
		else
			count++, cout << lessThan_10hours_flights[i].src;

		if (count < 10)
			cout << ", ";
		if (count == 10)
			break;
	}

	return res;
}

// cau 2
void ex2(Graph& graph) {
	int total_edges = graph.base.size(); // total flights
	int total_nodes = graph.id2name.size();


	vector<int> deg(graph.id2name.size(), 0);
	for (int i = 0; i < total_edges; i++) {
		int src = graph.name2id[graph.base[i].src];
		int des = graph.name2id[graph.base[i].des];

		deg[src]++;
		deg[des]++;
	}

	int num_nodes_even_deg = 0;
	int max_even_deg = 0;
	for (int i = 0; i < total_nodes; i++)
		if (deg[i] % 2 == 0) {
			num_nodes_even_deg++;
			max_even_deg = max(max_even_deg, deg[i]);
		}

	string even_deg_list = "";
	for (int i = 0; i < total_nodes; i++) {
		if (deg[i] == max_even_deg) {
			if (even_deg_list != "")
				even_deg_list += ", ";
			even_deg_list += graph.id2name[i];
		}
	}
	cout << num_nodes_even_deg << " even vertices, the highest: " << even_deg_list;
}


// cau 3
bool isConnected(Graph& graph, const string& country1, const  string& country2) {
	int country1_idx = graph.name2id[country1];
	int country2_idx = graph.name2id[country2];

	return graph.adj[country1_idx][country2_idx] != -1;
}

void ex3(Graph& graph, const string& country1, const string& country2) {
	vector<vector<string>> developed_areas;

	if (!isConnected(graph, country1, country2)) {
		cout << "0 quad";
		return;
	}

	int country1_idx = graph.name2id[country1];
	int country2_idx = graph.name2id[country2];

	for (int i = 0; i < graph.id2name.size() /*number of nodes*/; i++) {
		string curr_country_i = graph.id2name[i];
		if (isConnected(graph, country1, curr_country_i) &&
			isConnected(graph, country2, curr_country_i)) {
			for (int j = i + 1; j < graph.id2name.size(); j++) {
				string curr_country_j = graph.id2name[j];
				if (isConnected(graph, country1, curr_country_j) &&
					isConnected(graph, country2, curr_country_j) &&
					isConnected(graph, curr_country_i, curr_country_j)) {
					developed_areas.push_back({ country1, country2, curr_country_i, curr_country_j });
				}
			}
		}
	}

	sort(developed_areas.begin(), developed_areas.end());

	cout << developed_areas.size() << " quads\n";
	for (int i = 0; i < 3; i++) { // PRINT 3 RANDOM AREAS
		for (int j = 0; j < 3; j++)
			cout << developed_areas[i][j] << ", ";
		cout << developed_areas[i][3] << endl;
	}
}

// ======================== AVL TREE =============================

bool isVowel(const char& x) {
	return x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u';
}

int vowel_count(const vector<char>& arr) {
	int res = 0;
	for (const char& x : arr) {
		if (isVowel(x))
			res++;
	}
	return res;
}

int max_appear(const vector<char>& arr) {
	map<char, int> freq;
	for (const char& x : arr) {
		freq[x]++;
	}

	int max_appear = 0;
	for (auto x : freq)
		max_appear = max(max_appear, x.second);

	return max_appear;
}

// cau 4


















void todo(string flightdata_filename, string data_filename) {
	// TODO
	string country1, country2;
	ifstream is(data_filename);
	string buff; is >> buff;
	stringstream ss(buff);
	getline(ss, country1, ',');
	getline(ss, country2);
	// =================== GRAPH =====================

	// cau 1
	Graph G = ex1(flightdata_filename, country1);
	cout << "\n=====\n";

	// cau 2
	ex2(G);
	cout << "\n=====\n";

	// cau 3
	ex3(G, country1, country2);

	// =================== AVL TREE =====================


}
void main_debug() {
	todo("g1.v2.jl", "data.txt");

	/*vector<char> arr = { 'a','b','a', 'a', 'a', 'c' };
	cout << max_appear(arr);*/
}
