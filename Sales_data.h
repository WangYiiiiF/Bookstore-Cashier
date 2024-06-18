#include "Data_base.h"

using namespace std;

class Sales_data
{
	//ΪSales_data�ķǳ�Ա������������Ԫ����
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend istream& read(istream&, Sales_data&);
	friend ostream& print(ostream&, const Sales_data&);

	//˽�г�Ա����
private:
	string bookNO;
	unsigned units_sold = 0;
	double revenue = 0.0;

	//���г�Ա����
public:
	//���캯��
	Sales_data() = default;
	Sales_data(const string& s, unsigned n, double p) :bookNO(s), units_sold(n), revenue(p* n) {}
	Sales_data(const string& s) :bookNO(s) {}
	Sales_data(istream&);

	string isbn() const;			//const��Ա��������ʾ�ú��������޸�������ݳ�Ա
	Sales_data& combine(const Sales_data&);
	double avg_price() const;		//const��Ա���� 
};


//�ǳ�Ա����

Sales_data add(const Sales_data& lhs, const Sales_data& rhs)
{
	Sales_data sum;
	sum = lhs;
	sum.combine(rhs);
	return sum;
}

istream& read(istream& is, Sales_data& item)
{
	double price = 0;
	is >> item.bookNO >> item.units_sold >> price;
	Instert(item.bookNO, item.units_sold, price);
	item.revenue = price * item.units_sold;
	return is;
}

ostream& print(ostream& os, const Sales_data& item)
{
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}

//���캯��
Sales_data::Sales_data(istream& is)
{
	read(is, *this);
}

//��Ա����
string Sales_data::isbn() const
{
	return this->bookNO;
}

Sales_data& Sales_data::combine(const Sales_data& item)
{
	this->units_sold += item.units_sold;
	this->revenue += item.revenue;
	return *this;			//���ص��øú����Ķ���
}

double Sales_data::avg_price() const
{
	if (units_sold)
	{
		return this->revenue / this->units_sold;
	}
	else
	{
		return 0;
	}
}
