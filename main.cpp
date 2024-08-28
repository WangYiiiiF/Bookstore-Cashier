#include "Sales_data.h"

int main()
{
	cout << "-----Bookstore Cashier Program-----" << endl;
	Sales_data total;
	//Init();		//Only Run Once.
	//hello
	//PrintData();  Print ALL data
	if (read(cin, total))
	{
		Sales_data trans;
		while (read(cin, trans))
		{
			if (total.isbn() == trans.isbn())
			{
				total.combine(trans);
			}
			else
			{
				print(cout, total) << endl;
				total = trans;
			}
		}
	print(cout, total) << endl;
	}
	else 
	{
		cerr << "No data!?" << endl;
	}

	return 0;
}