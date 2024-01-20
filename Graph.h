#include <iostream>
#include <vector>
#include <map>
#include <cfloat>
#include <set>

struct Item{
	unsigned int id;
	double weight = 1;
	struct Item * next = nullptr;
	Item(int id): id(id){}
	Item(int id, double weight): id(id), weight(weight){}
};

class List{
	public:
		struct Item * Head;
		
		List():Head(nullptr){}
		int Insert(struct Item * x); 		// Вставка с соблюдением порядка id
		int Remove(int id);          		// Удаление 1 элемента по id
		void Delete(void);            		// Удаление всего списка
		void Print(void);		   	  		// Печать списка
		int Change(int id, double weight); // Изменение веса
		~List(){this->Delete();}
};

class Graph{
	public:
		std::map<unsigned int, class List *> adjacency_list; 	// список вершин
		unsigned int Size = 0;									// количество вершин
		unsigned int Id = 0;									// идентификатор следующей вершины
		
		void PrintGraph();										// печать графа
		void AddVertex();										// добавление вершины
		int DeleteVertex(int id);								// удаление вершины
		int AddOrEdge(int id_from, int id_to);					// добавление ребра 
		int AddOrEdge(int id_from, int id_to, double weight);	// добавление ребра с весом
		int AddEdge(int id_from, int id_to);					// добавление ребра 
		int AddEdge(int id_from, int id_to, double weight);	    // добавление ребра с весом
		int DeleteEdge(int id_from, int id_to);				    // удаление ребра
		int InsertVertex(int id);								// вставка вершины
		bool SearchEdge(int id_from, int id_to);				// проверка ребра между вершинами
		double Dijkstra(int id_from, int id_to); 				// алгоритм Дейкстры
};

void List::Print(){
	struct Item * n = this->Head;
	while(n != nullptr){
		std::cout << n->id << "(" << n->weight << ")" << " ";
		n = n->next;
	}
}

int List::Insert(struct Item * x){
	if(this->Head == nullptr || this->Head->id > x->id){
		x->next = this->Head;
		this->Head = x;
	}
	else{
		struct Item * n = this->Head;
		while(n->next != nullptr && n->next->id < x->id){
			n = n->next;
		}
		if(n->next != nullptr && n->next->id == x->id){
			return 1;
		}else{
			x->next = n->next;
			n->next = x;
		}
	}
	return 0;
}

int List::Remove(int id){
	struct Item * n = this->Head;
	struct Item * p = nullptr;
	while(n != nullptr && n->id < id){
		p = n;
		n = n->next;
	}
	if (n != nullptr && n->id == id){
		if(p == nullptr)
			this->Head = n->next;
		else
			p->next = n->next;
		delete n;
	}else return 1;
	return 0;	
}

void List::Delete(){
	struct Item *temp;
	while(this->Head != nullptr){
		temp = this->Head;
		this->Head = this->Head->next;
		delete temp;
	}
}

int List::Change(int id, double weight){
	struct Item * n = this->Head;
	while(n != nullptr && n->id != id){
		n = n->next;
	}
	if (n != nullptr && n->id == id){
		n->weight = weight;
	}else return 1;
	return 0;
}

void Graph::PrintGraph(){
	if(this->Size > 0){
		for(const auto& element : this->adjacency_list){
			std::cout << element.first << ": ";
			element.second->Print();
			std::cout << std::endl;
		}
	}
	else{
		std::cout<< "The graph is empty" << std::endl;
	}
}

void Graph::AddVertex(){
	this->adjacency_list[this->Id++] = new List;
	Size++;
}

int Graph::InsertVertex(int id){
	if(this->adjacency_list.find(id) == this->adjacency_list.end() && id <= this->Id){
		this->adjacency_list[id] = new List;
		Size++;
	}else return 1;
	return 0;
}

int Graph::AddOrEdge(int id_from, int id_to){ 
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if((it != this->adjacency_list.end()) && (it2 != this->adjacency_list.end()) && (id_from != id_to)){
		return it->second->Insert(new Item(id_to));
	}else return 1;
}

int Graph::AddEdge(int id_from, int id_to){
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if(it == this->adjacency_list.end() || it2 == this->adjacency_list.end()) return 1;
	if(this->SearchEdge(id_from, id_to) || this->SearchEdge(id_to, id_from)) return 1;
	this->AddOrEdge(id_from, id_to);
	this->AddOrEdge(id_to, id_from);
	return 0;
}

int Graph::AddOrEdge(int id_from, int id_to, double weight){ 
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if((it != this->adjacency_list.end()) && (it2 != this->adjacency_list.end()) && (id_from != id_to)){
		return it->second->Insert(new Item(id_to, weight));
	}else return 1;
}

int Graph::AddEdge(int id_from, int id_to, double weight){
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if(it == this->adjacency_list.end() || it2 == this->adjacency_list.end()) return 1;
	if(this->SearchEdge(id_from, id_to) || this->SearchEdge(id_to, id_from)) return 1;
	this->AddOrEdge(id_from, id_to, weight);
	this->AddOrEdge(id_to, id_from, weight);
	return 0;
}

int Graph::DeleteEdge(int id_from, int id_to){
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if((it != this->adjacency_list.end()) && (it2 != this->adjacency_list.end())){
		return it->second->Remove(id_to);
	}else return 1;
	return 0;
}

int Graph::DeleteVertex(int id){
	auto it = this->adjacency_list.find(id);
	if(it != this->adjacency_list.end()){
		for(const auto& element : this->adjacency_list){
			this->DeleteEdge(element.first, id);
		}
		it->second->Delete();
		this->adjacency_list.erase(id);
		this->Size--;
	}else return 1;
	return 0;
}

bool Graph::SearchEdge(int id_from, int id_to){
	auto it = this->adjacency_list.find(id_from);
	auto it2 = this->adjacency_list.find(id_to);
	if(it != this->adjacency_list.end() && it2 != this->adjacency_list.end() && id_from != id_to){
		Item * p = this->adjacency_list[id_from]->Head;
		while(p != nullptr){
			if(p->id == id_to) return true;
			p = p->next;
		}
	}
	return false;
}

double Graph::Dijkstra(int id_from, int id_to){ 
	if(this->adjacency_list.find(id_from) == this->adjacency_list.end() ||
	   this->adjacency_list.find(id_to) == this->adjacency_list.end())
	   return DBL_MAX;
	if(id_from == id_to) return DBL_MAX;
	std::vector<int>distance(this->Id, DBL_MAX);
	distance[id_from] = 0;
	std::set<std::pair<int, int>> q;
	q.insert({distance[id_from], id_from});
	while(!q.empty()){
		auto from = q.begin()->second;
		struct Item * p = this->adjacency_list[from]->Head;
		q.erase(q.begin());
		while(p != nullptr){
			if(distance[p->id] > distance[from] + p->weight){
				q.erase({distance[p->id], p->id});
				distance[p->id] = distance[from] + p->weight;
				q.insert({distance[p->id], p->id});
			}
			p = p->next;	
		}		
	}
	return distance[id_to];
}

