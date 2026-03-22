#include <vector>
#include <iostream>
using namespace std;
#define nl '\n'

class Minheap{
    private:
        vector<pair<double,int>> heap;

        int parent(int i){ return (i-1)/2; }
        int left(int i){ return (2*i) + 1; }
        int right(int i){ return (2*i) + 2; }

        void heapifyUp(int i){
            while(i>0 && heap[parent(i)].first > heap[i].first){
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void heapifyDown(int i){
            int smallest = i;
            int l = left(i);
            int r = right(i);
            int val = heap[smallest].first;

            if(l<heap.size() && val > heap[l].first){
                smallest = l;
                val = heap[smallest].first;
            }

            if(r<heap.size() && val > heap[r].first){
                smallest = r;
                val = heap[smallest].first;
            }

            if(smallest != i){
                swap(heap[i],heap[smallest]);
                heapifyDown(smallest); // FIX
            }
        }

    public:
        void add(pair<double,int> val){
            heap.push_back(val);
            heapifyUp(heap.size() - 1);
        }

        void del(){
            swap(heap[0],heap[heap.size() - 1]);
            heap.pop_back();
            heapifyDown(0);
        }

        pair<double,int> top(){ 
            return heap[0];
        }

        vector<pair<double,int>> disp(){
            return heap;
        }

        int sz(){
            return heap.size();
        }
};

vector<pair<double,int>> heapOptmization(vector<pair<double,int>> &sc){
    Minheap h;

    for(auto &x:sc){
        if(h.sz() < 3){
            h.add(x);
        }
        else if(x.first > h.top().first){ 
            h.del();
            h.add(x);
        }
    }

    return h.disp();
}

/*
 * 
 *   5
 * 12  20
 * 
 * add 3
 * swap 3
 * 12 20
 *      5
 * 
 * 5 7 9
 * 
 * first swap(a[root],a[n-1])
 * then pop_back then heapify down 
 * after that add the elems in the end then heapify up
 * 
 * 
 */