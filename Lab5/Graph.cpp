#include <iostream>  // Для ввода-вывода 
#include <vector>    // Для работы с динамическими массивами 
#include <string>    // Для работы со строками 
#include <algorithm> // Для стандартных алгоритмов 

using namespace std;


template <class V, class E>
class Vertex;

// Класс ребра графа
template <class V, class E> // объявляем шаблона класса для описания вершин и рёбер графа
class Edge  
{
public:
    Edge(const E& properties, Vertex<V, E>* vertex1, Vertex<V, E>* vertex2)
        : properties_(properties), vertex1_(vertex1), vertex2_(vertex2) {}

    // Получаем противоположную вершину относительно текущей
    Vertex<V, E>* getOtherVertex(Vertex<V, E>* current) const
     {
        return current == vertex1_ ? vertex2_ : vertex1_;
     }
    // Получаем вес ребра
    const E& getProperties() const { return properties_; }

private:
    E properties_;               // Свойства ребра
    Vertex<V, E>* vertex1_;      // Первая вершина
    Vertex<V, E>* vertex2_;      // Вторая вершина
};


template <class V, class E>
class Vertex 
{
public:
    Vertex(const V& properties) : properties_(properties) {}

    // Добавляем ребро к текущей вершине
    void addEdge(const E& properties, Vertex<V, E>* target) 
    {
        edges_.emplace_back(properties, this, target);
    }

    // Получаем свойства вершины
    const V& getProperties() const { return properties_; }

    // Получаем список рёбер, связанных с вершиной
    const vector<Edge<V, E>>& getEdges() const { return edges_; }

    // Выводим информацию о вершине и связанных рёбрах
    void display() const 
    {
        cout << "Вершина: " << properties_ << "\n";
        for (const auto& edge : edges_) 
        {
            Vertex<V, E>* target = edge.getOtherVertex(const_cast<Vertex<V, E>*>(this));
            cout << "  Ребро к вершине " << target->getProperties()
                 << ", вес: " << edge.getProperties() << "\n";
        }
    }

private:
    V properties_;             
    vector<Edge<V, E>> edges_;  // Список рёбер, связанных с вершиной
};

// Рекурсивная функция поиска пути
template <class V, class E>
bool search(Vertex<V, E>* vertex, const V& target, vector<Vertex<V, E>*>& visited, E& cost, string& path) 
{
    if (find(visited.begin(), visited.end(), vertex) != visited.end())
        return false;

    visited.push_back(vertex);
    if (vertex->getProperties() == target)
        return true;

    for (const auto& edge : vertex->getEdges())
    {
        Vertex<V, E>* next = edge.getOtherVertex(vertex);
        if (search(next, target, visited, cost, path)) 
        {
            cost += edge.getProperties();
            path = vertex->getProperties() + " -" + to_string(edge.getProperties()) + "> " + path;
            return true;
        }
    }

    visited.pop_back();
    return false;
}

int main()
{

    Vertex<string, int> a1("A1"), b1("B1"), c1("C1"), b2("B2"), e1("E1"), f1("F1"), b3("B3");
    a1.addEdge(10, &b1);
    b1.addEdge(20, &c1);
    a1.addEdge(15, &b2);
    c1.addEdge(30, &e1);
    e1.addEdge(25, &f1);
    b2.addEdge(40, &f1);
    a1.addEdge(10, &b3);
    b3.addEdge(12, &c1);

    // Выводим все вершины и их рёбра
    cout << "Граф:\n";
    a1.display();
    b1.display();
    c1.display();
    b2.display();
    e1.display();
    f1.display();
    b3.display();

    // Ищем кратчайший путь от вершины A1 до вершины F1
    vector<Vertex<string, int>*> visited;
    int cost = 0;
    string path;
    if (search(&a1, string("F1"), visited, cost, path)) {
        cout << "\nПуть найден: " << path << "\n";
        cout << "Общая стоимость: " << cost << "\n";
    } else {
        cout << "\nПуть не найден.\n";
    }

    return 0;
}
