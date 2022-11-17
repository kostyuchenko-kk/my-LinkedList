#include "DoubleList.h"
#include <iostream>

template<class T>
void printList(DoubleList<T>& list)
{
	for (auto &el:list)
	{
		std::cout << el << std::endl;
	}
}

int main()
{
	DoubleList<int> list = { 3,6,9,12,15,20 };
	list.push_back(30);
	list.push_back(30);
	list.pop_back();
	list.push_front(1);
	list.push_front(1);
	list.pop_front();
	list.insert(++list.begin(), 2);
	list.erase(list.find(9));
	printList(list);

	std::cout << std::endl;

	DoubleList<int> list1(10, 20);
	DoubleList<int> list2(list1);
	DoubleList<int> list3(std::move(list2));
	list3 = list1;
	list1 = std::move(list3);

	printList(list1);

	return 0;
}