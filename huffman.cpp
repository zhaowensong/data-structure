#include<iostream>
using namespace std;
#include<map>
#include<string>
// ����Ϊ��
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
//����Ϊhuffman��
template<typename E>
class Huffnode
{
public:
	virtual ~Huffnode() {}
	virtual int weight() = 0; //���麯��
	virtual bool isLeaf() = 0; //���麯��
};
template<typename E>
class Leafnode :public Huffnode<E>  //Ҷ�ӽ��
{
private:
	E it;
	int wgt;
public:
	Leafnode(const E&val, int freq)  //���캯��
	{
		it = val;
		wgt = freq;
	}
	int weight() { return wgt; }
	E val() { return it; }
	bool isLeaf() { return true; }
};
template<typename E>
class Intlnode :public Huffnode<E>  //�ڲ��ڵ�
{
private:
	Huffnode<E>*lc;
	Huffnode<E>*rc;
	int wgt;
public:
	Intlnode(Huffnode<E>*l, Huffnode<E>*r) //���캯��
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
	Hufftree(const E&val, int freq)  //Ҷ�����صĹ��캯��
	{
		Root = new Leafnode<E>(val, freq);
	}
	Hufftree(Hufftree<E>*l, Hufftree<E>*r)  //�ڲ��ڵ���صĹ��캯��
	{
		Root = new Intlnode<E>(l->root(), r->root());
	}
	~Hufftree() {}
	Huffnode<E>*root() { return Root; }
	int weight() { return Root->weight(); }
};
template<typename E>
void swap(E* Heap, int a, int b)  //��������
{
	E temp = Heap[a];
	Heap[a] = Heap[b];
	Heap[b] = temp;
}
template<typename E>
class minTreeComp  //��С�ѵıȽ���
{
public:
	static bool prior(Hufftree<E> *a, Hufftree<E> *b)
	{
		return a->weight()<b->weight();
	}
};
//��������
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
	if (node == NULL) return;   //������Ϊ�� �򷵻�
	if (node->isLeaf()) //������ΪҶ�ӽ�㣬����huffmancode��map�е�ֵ���и�ֵ��֮�󷵻�
	{
		huffmancode[((Leafnode<E>*)(node))->val()] = a; return;
	}
	getcode(((Intlnode<E>*)(node))->left(), huffmancode, a + "0");  //���������ݹ�
	getcode(((Intlnode<E>*)(node))->right(), huffmancode, a + "1"); //���������ݹ�

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
	map<char, string>huffmancode;  //����һ��map
	getcode<char>(L1->root(), huffmancode, "");
	int totallength = 0;
	for (char i = 'a'; i <= 'f'; ++i)
	{
		totallength += freq[i]*huffmancode[i].length();
		cout << i << " " << huffmancode[i] << endl;
	}
	string temp;
	cout << "ͨ��������abcdef������ĸ����һ����ĸ����" << endl;
	cin >> temp;
	string temp2;
	for (int i = 0; i < temp.length(); ++i)
	{
		temp2 += huffmancode[temp[i]];
	}
	cout << "��ĸ����huffman��������" << endl;
	cout << temp2 << endl;
	cout << "The average code length is:" << totallength / 59 << endl;
	system("pause");
}

