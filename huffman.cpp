#include<iostream>
using namespace std;
#include<map>
#include<string>
// 以下为堆
template<typename E, typename Comp>
class heap
{
private:
	E*Heap;
	int maxsize;
	int n;
	void siftdown(int pos)
	{
		while (!isLeaf(pos))
		{
			int j = leftchild(pos); int rc = rightchild(pos);
			if (rc < n&&Comp::prior(Heap[rc], Heap[j]))
				j = rc;
			if (Comp::prior(Heap[pos], Heap[j])) return;
			swap(Heap, j, pos);
			pos = j;
		}
	}
public:
	heap(E* h, int n1, int maxsize1)
	{
		Heap = h;
		n = n1;
		maxsize = maxsize1;
		buildHeap();
	}
	int size()const
	{
		return n;
	}
	bool isLeaf(int pos)
	{
		return ((pos < n) && (pos >= n / 2));
	}
	int leftchild(int pos)
	{
		return 2 * pos + 1;
	}
	int rightchild(int pos)
	{
		return 2 * pos + 2;
	}
	int parent(int pos)
	{
		return (pos - 1) / 2;
	}
	void buildHeap()
	{
		for (int i = n / 2 - 1; i >= 0; --i) siftdown(i);
	}
	void insert(const E&it)
	{
		if (n > maxsize) {
			cout << "Heap is full"; exit(-1);
		}
		int curr = n++;
		Heap[curr] = it;
		while ((curr != 0) && (Comp::prior(Heap[curr], Heap[parent(curr)])))
		{
			swap(Heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
	E removefirst()
	{
		if (n == 0) { cout << "Heap is empty"; exit(-1); }
		swap(Heap, 0, --n);
		if (n != 0) siftdown(0);
		return Heap[n];
	}
	E remove(int pos)
	{
		if (pos >= n || pos < 0) { cout << "Bad position"; exit(-1); }
		if (pos == n - 1) n--;
		else
		{
			swap(Heap, pos, --n);
			while ((pos != 0) && (Comp::prior(Heap[pos], Heap[parent(pos)])))
			{
				swap(Heap, pos, parent(pos));
				pos = parent(pos);
			}
			if (n != 0) siftdown(pos);
		}
		return Heap[n];
	}
	void print()const
	{
		for (int i = 0; i<n; ++i)
			cout << Heap[i] << " ";
	}
};
//以下为huffman树
template<typename E>
class Huffnode
{
public:
	virtual ~Huffnode() {}
	virtual int weight() = 0; //纯虚函数
	virtual bool isLeaf() = 0; //纯虚函数
};
template<typename E>
class Leafnode :public Huffnode<E>  //叶子结点
{
private:
	E it;
	int wgt;
public:
	Leafnode(const E&val, int freq)  //构造函数
	{
		it = val;
		wgt = freq;
	}
	int weight() { return wgt; }
	E val() { return it; }
	bool isLeaf() { return true; }
};
template<typename E>
class Intlnode :public Huffnode<E>  //内部节点
{
private:
	Huffnode<E>*lc;
	Huffnode<E>*rc;
	int wgt;
public:
	Intlnode(Huffnode<E>*l, Huffnode<E>*r) //构造函数
	{
		lc = l;
		rc = r;
		wgt = l->weight() + r->weight();
	}
	int weight() { return wgt; }
	bool isLeaf() { return false; }
	Huffnode<E>* left()const
	{
		return lc;
	}
	Huffnode<E>* right()const
	{
		return rc;
	}
	void setLeft(Huffnode<E>* b)
	{
		lc = static_cast<Huffnode<E>*>(b);
	}
	void setRight(Huffnode<E>*b)
	{
		rc = static_cast<Huffnode<E>*>(b);
	}
};
template<typename E>
class Hufftree {
private:
	Huffnode<E>*Root;
public:
	Hufftree(const E&val, int freq)  //叶结点相关的构造函数
	{
		Root = new Leafnode<E>(val, freq);
	}
	Hufftree(Hufftree<E>*l, Hufftree<E>*r)  //内部节点相关的构造函数
	{
		Root = new Intlnode<E>(l->root(), r->root());
	}
	~Hufftree() {}
	Huffnode<E>*root() { return Root; }
	int weight() { return Root->weight(); }
};
template<typename E>
void swap(E* Heap, int a, int b)  //交换函数
{
	E temp = Heap[a];
	Heap[a] = Heap[b];
	Heap[b] = temp;
}
template<typename E>
class minTreeComp  //最小堆的比较类
{
public:
	static bool prior(Hufftree<E> *a, Hufftree<E> *b)
	{
		return a->weight()<b->weight();
	}
};
//建树函数
template<typename E>
Hufftree<E>* buildHuff(Hufftree<E>** treearray, int count)
{

	heap<Hufftree<E>*, minTreeComp<char> >* forest = new heap<Hufftree<E>*, minTreeComp<char> >(treearray, count, count);

	Hufftree<E>*temp1, *temp2, *temp3 = NULL;

	while (forest->size() > 1)
	{

		temp1 = forest->removefirst();

		temp2 = forest->removefirst();

		temp3 = new Hufftree<E>(temp1, temp2);

		forest->insert(temp3);

		delete temp1;
		delete temp2;
	}
	return temp3;
}

template<typename E>
void getcode(Huffnode<E>*node,map<char,string>&huffmancode,string a)
{
	if (node == NULL) return;   //如果结点为空 则返回
	if (node->isLeaf()) //如果结点为叶子结点，则令huffmancode的map中的值进行赋值，之后返回
	{
		huffmancode[((Leafnode<E>*)(node))->val()] = a; return;
	}
	getcode(((Intlnode<E>*)(node))->left(), huffmancode, a + "0");  //对左子树递归
	getcode(((Intlnode<E>*)(node))->right(), huffmancode, a + "1"); //对右子树递归

}
int main()
{
	Hufftree<char>*arr[6];
	arr[0] = new Hufftree<char>('a', 7);
	arr[1] = new Hufftree<char>('b', 9);
	arr[2] = new Hufftree<char>('c', 2);
	arr[3] = new Hufftree<char>('d', 6);
	arr[4] = new Hufftree<char>('e', 32);
	arr[5] = new Hufftree<char>('f', 3);
	auto L1 = buildHuff(arr, 6);
	map<char, int>freq;
	freq['a'] = 7;
	freq['b'] = 9;
	freq['c'] = 2;
	freq['d'] = 6;
	freq['e'] = 32;
	freq['f'] = 3;
	map<char, string>huffmancode;  //建立一个map
	getcode<char>(L1->root(), huffmancode, "");
	int totallength = 0;
	for (char i = 'a'; i <= 'f'; ++i)
	{
		totallength += freq[i]*huffmancode[i].length();
		cout << i << " " << huffmancode[i] << endl;
	}
	string temp;
	cout << "通过键盘用abcdef六个字母给出一段字母串：" << endl;
	cin >> temp;
	string temp2;
	for (int i = 0; i < temp.length(); ++i)
	{
		temp2 += huffmancode[temp[i]];
	}
	cout << "字母串的huffman编码结果：" << endl;
	cout << temp2 << endl;
	cout << "The average code length is:" << totallength / 59 << endl;
	system("pause");
}

