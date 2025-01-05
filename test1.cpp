#include <iostream>
#include <stdlib.h>

using namespace std;

template <class T>
struct ChainNode
{
    T data;
    ChainNode<T> *link;

    ChainNode() {}
    ChainNode(const T &data)
    {
        this->data = data;
    }
    ChainNode(const T &data, ChainNode<T> *next)
    {
        this->data = data;
        this->link = next;
    }
};

template <class T>
class Chain
{
public:
    Chain()
    {
        first = 0;
    }

    ~Chain()
    {
        while (first)
        {
            ChainNode<T> *next = first->link;
            delete first;
            first = next;
        }
    }

    void Insert(int theIndex, const T &theElement);

    void Delete(int theIndex);

    void Output(ostream &out) const
    {
        ChainNode<T> *current = first;
        while(current != 0)
        {
            out << current->data << "->";
            current = current->link;
        }
        out << "NULL" << endl;
        
    }

    T Index(int theIndex) const;

    bool IsEmpty() const
    {
        return first == 0;
    }

private:
    ChainNode<T> *first;

};

template <class T>
void Chain<T>::Insert(int theIndex,const T& theElement)
{
    if(theIndex < 0)
    {
        cout << "The index is out of range" << endl;
        return;
    }
    if (theIndex == 0)
    {
        // insert at front
        first = new ChainNode<T>(theElement, first);
    }
    else
    {
        // find predecessor of new element
        ChainNode<T> *p = first;
        for (int i = 0; i < theIndex - 1; i++)
        {
            p = p->link;
        }

        // insert after p
        p->link = new ChainNode<T>(theElement, p->link);
    }
}

template <class T>
void Chain<T>::Delete(int theIndex)
{
    if(first == 0)
    {
        cout << "The chain is empty" << endl;
        return;
    }
    ChainNode<T> *deleteNode;
    if (theIndex == 0)
    {
        // remove first node from chain
        deleteNode = first;
        first = first->link;
    }
    else
    {
        // use p to get to predecessor of desired node
        ChainNode<T> *p = first;
        for (int i = 0; i < theIndex - 1; i++)
        {
            p = p->link;
        }

        deleteNode = p->link;
        p->link = p->link->link;
    }

    delete deleteNode;
}

template <class T>
T Chain<T>::Index(int theIndex) const
{

    ChainNode<T> *current = first;
    for (int i = 0; i < theIndex; i++)
    {
        current = current->link;
    }

    return current->data;
}

int main()
{
    Chain<int> chain;
    
    string s;
    
    while(getline(cin,s))
    {
    	string sub1 = s.substr(0, s.find(" "));
    	if(sub1 == "E")
    	{
    		if(chain.IsEmpty())
    		{
    			cout << "true" << endl;
			}
			else
			{
				cout << "false" << endl;
			}
		}
		else if(sub1 == "P")
		{
			chain.Output(cout);
		}
		else if(sub1 == "D")
		{
			string sub2 = s.substr(s.find(" ") + 1, s.length());
			chain.Delete(atoi(sub2.c_str()));
		}
		else if(sub1 == "I")
		{
			/*s = s.substr(s.find(" ") + 1, s.length());
			string idx = s.substr(0, s.find(" "));
			s = s.substr(s.find(" ") + 1, s.length());
			string val = s.substr(s.find(" ") + 1, s.length());
			//cout << idx << " " << val << endl;
			chain.Insert(atoi(idx.c_str()),atoi(val.c_str()));*/
			
			string v[3];
			for(int i = 0 ; i < 3 ; i++)
			{
				v[i] = s.substr(0, s.find(" "));
				s = s.substr(s.find(" ") + 1, s.length());
			}
			string idx = v[1];
			string val = v[2];
			chain.Insert(atoi(idx.c_str()),atoi(val.c_str()));
		} 
    }

    return 0;
}
