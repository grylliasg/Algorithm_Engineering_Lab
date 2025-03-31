#include <iostream>
#include <LEDA/core/list.h>
using namespace std;
using namespace leda;

void printLists(leda::list<int>& l1, leda::list<int>& l2)
{
    
    if (!l1.empty())
    {
        // Print all the elements of the list 1
        cout << "List 1: " << endl;
        int i;
        forall(i, l1)
        {
            cout << i << " ";
        }
        cout << endl;
        cout << "1st element of l1: " << l1.contents(l1.first()) << ", Last element of l1: " << l1.contents(l1.last()) << endl;
        cout << "Max element of l1: " << l1.contents(l1.max()) << ", Min element of l1: " << l1.contents(l1.min()) << endl;
        cout << "List size: " << l1.size() << endl;
        cout << endl << endl;
    }
    else cout << "List 1 is empty..." << endl;
    
    if (!l2.empty())
    {
        // Print all the elements of the list 2
        cout << "List 2: " << endl;
        int y;
        forall(y, l2)
        {
            cout << y << " ";
        }
        cout << endl;
        cout << "1st element of l2: " << l2.contents(l2.first()) << ", Last element of l2: " << l2.contents(l2.last()) << endl;
        cout << "Max element of l2: " << l2.contents(l2.max()) << ", Min element of l2: " << l2.contents(l2.min()) << endl;
        cout << "List size: " << l2.size() << endl << endl;
    }
    else cout << "List 2 is empty..." << endl;
    
}

int main()
{
    leda::list<int> list1; // Initialize list 1 
    leda::list<int> list2; // Initialize list 2
    for (int i = 0; i < 21; i++)
    {
        list1.append(i); // Insert the element from behind
        list2.push_front(i); // Insertion from front
    }

    printLists(list1, list2);

    // 1.3 Move items from list1 to list2 before element "10"

    list_item it, li;

    for (it = list2.first(); it != nil; it = list2.succ(it)) // Diatrexoume ta stoixeia ths listas 2
    {
        if (list2.contents(it) == 10) break; // An vreis to 10, break. Ara to it twra deixnei sto 10
    }

    for (li = list1.first(); li != nil; li = list1.succ(li)) // Diatrexoume ta stoixeia ths listas 2
    {
        list2.insert(list2.contents(li), it, before); // Edw prepei na valoume "before" giati to default einai "behind"
    }
    
    li = nil;   // CLear
    list1.clear();

    printLists(list1, list2);

    // 1.4 Metafora 1st element to the end

    it = list2.first();     // Point to the 1st element and send it to the back
    list2.move_to_back(it);

    printLists(list1, list2);

    // 1.5 Diagrafh stoixeiou theshs 10

    li = list2.first();     // "li" points to the 1st element
    for (int z = 1; z < 10; z++) li = list2.succ(li);   // Metakinish 9 theseis (now "li" points to the 10th element)
    list2.del_item(li);
    li = nil;   // Clear the pointer

    printLists(list1, list2);

    // 1.6 Taksinomisi kai anathesi timhs sthn list1, kai afairesh diplotypwn apo thn 2

    list2.sort();
    list1 = list2;
    list2.unique();
    printLists(list1, list2);

    // 1.7 Sygxwneysh sthn 1h lista

    list1.merge(list2);
    printLists(list1, list2);

    return 0;
}