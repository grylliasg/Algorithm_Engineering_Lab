#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
using namespace std;

void printLists(list<int> a, list<int> b)
{
    cout << endl << "My List: " << endl;
    cout << " Front:" << a.front() << " Back:" << a.back() << " Size:" << a.size() << endl;
    cout << " ";

    list<int>::iterator it;  // Arxikopoihsh Iterator

    for (it = a.begin(); it != a.end(); it++)
    {
        cout << *it << " ";
    }

    cout << endl << endl;

    cout << "Your List: " << endl;
    cout << " Front:" << b.front() << " Back:" << b.back() << " Size:" << b.size() << endl;
    cout << " ";
    for (it = b.begin(); it != b.end(); it++)
    {
        cout << *it << " ";
    }

    cout << endl << endl;
}


int main()
{
    list<int> myList;
    list<int> yourList;
    list<int>::iterator iter;

    for (int i=0; i<6; i++)
    {
        myList.push_back(i);
        yourList.push_front(i);
    }
    
    printLists(myList, yourList);

    // 3. Metakinish stoixeiwn
    
    iter = find(yourList.begin(), yourList.end(), 3); // Pointing to item "3" in "yourList" if exists

    yourList.splice(iter, myList); // Transfer
    printLists(myList, yourList);

    // 4. Metafora tou 1ou stoixeiou sto telos 

    iter = yourList.begin();

    yourList.splice(yourList.end(), yourList, iter);
    printLists(myList, yourList);

    // 5. Diagrafh 6ou stoixeiou

    iter = yourList.begin();
    advance(iter, 5);
    
    iter = yourList.erase(iter); // Erasing...
    printLists(myList, yourList);

    // 6. Taksinomish kai afairesh diplotypwn

    yourList.sort();
    myList = yourList;
    yourList.unique(); // Afairesh diplotypwn
    
    printLists(myList, yourList);

    // 7. Sygxwneysh sthn 1h lista

    myList.merge(yourList);

    printLists(myList, yourList);

    return 0;
}
