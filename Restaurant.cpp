#include "main.h"
class imp_res : public Restaurant
{
private:
	class TimeLine
	{
	public:
		customer *data;
		TimeLine *next;
		TimeLine *prev;
		bool onTable;

	public:
		TimeLine(customer *data, bool onTable, TimeLine *next = nullptr, TimeLine *prev = nullptr)
			: data(data), next(next), prev(prev), onTable(onTable) {}
		~TimeLine()
		{
			delete data;
		}
	};

private:
	customer *queueHead;
	customer *customerX;
	TimeLine *orderTimeHead;
	TimeLine *orderTimeTail;
	int tableSize;
	int queueSize;
	bool is_Exist(customer *t, string name, int energy);
	//! Thằng này nó tháo cục Time
	TimeLine *findCustomerDelete();
	//! Thằng này là nó xóa cái cục Time và nối lại cái queue và cái bàn
	void deleteCustomerRefTime(TimeLine *TimeLineDelete);
	void queueToTable();
	void reverse_print(TimeLine *head);
	void swapNode(customer *head, customer *tail);
	customer *getCusAt(customer *x, int index);
	int insertSort(customer *head, int n, int gap);
	int getIdxQueue(customer *t);

public:
	// imp_res()
	// {
	// 	customerX = queueHead = nullptr;
	// 	queueSize = tableSize = 0;
	// 	orderTimeHead = orderTimeTail = nullptr;
	// }
	imp_res() : customerX(nullptr), queueHead(nullptr), queueSize(0), tableSize(0),
				orderTimeHead(nullptr), orderTimeTail(nullptr) {}
	~imp_res()
	{
		while (orderTimeHead)
		{
			TimeLine *temp = orderTimeHead;
			orderTimeHead = orderTimeHead->next;
			delete temp;
		}
	}
	void RED(string name, int energy);
	void BLUE(int num);
	void PURPLE();
	void REVERSAL();
	void DOMAIN_EXPANSION();
	void UNLIMITED_VOID();
	void LIGHT(int num);
};

void imp_res::RED(string name, int energy)
{
	if (energy == 0 || tableSize + queueSize >= MAXSIZE * 2)
		return;
	else
	{
		customer *temp = customerX;
		if (is_Exist(temp, name, energy))
			return;
	}
	if (tableSize == MAXSIZE)
	{
		customer *newCus = new customer(name, energy, nullptr, nullptr);

		if (queueSize == 0)
		{
			queueHead = newCus;
			queueHead->prev = newCus;
			queueHead->next = newCus;
		}
		else
		{
			newCus->prev = queueHead->prev;
			newCus->next = queueHead;
			queueHead->prev->next = newCus;
			queueHead->prev = newCus;
		}
		queueSize++;
		TimeLine *newOrderTime = new TimeLine(newCus, false);

		orderTimeTail->next = newOrderTime;
		newOrderTime->prev = orderTimeTail;
		orderTimeTail = orderTimeTail->next;
		return;
	}
	else if (tableSize == 0)
	{
		customer *cus = new customer(name, energy, nullptr, nullptr);

		customerX = cus;
		customerX->prev = cus;
		customerX->next = cus;
		tableSize++;
		orderTimeTail = orderTimeHead = new TimeLine(customerX, true);
		return;
	}

	if (tableSize >= MAXSIZE / 2)
	{
		customer *temp = customerX;
		// biến lưu giá trị lệch Energy lớn nhất;
		int maxDiff = abs(temp->energy - energy);
		// khách hàng X mới
		customer *newX = customerX;
		for (int i = 0; i < tableSize; i++)
		{
			if (maxDiff < abs(temp->energy - energy))
			{
				maxDiff = abs(temp->energy - energy);
				newX = temp;
			}
			temp = temp->next;
		}
		customerX = newX;
	}

	customer *newCus = new customer(name, energy, nullptr, nullptr);

	if (energy >= customerX->energy)
	{
		customer *tmp = customerX->next;
		customerX->next = newCus;

		newCus->prev = customerX;
		newCus->next = tmp;
		tmp->prev = newCus;
	}
	else
	{
		customer *tmp = customerX->prev;
		newCus->next = customerX;
		tmp->next = newCus;
		customerX->prev = newCus;
		newCus->prev = tmp;
	}

	customerX = newCus;
	tableSize++;

	TimeLine *newOrderTime = new TimeLine(newCus, true);

	orderTimeTail->next = newOrderTime;
	newOrderTime->prev = orderTimeTail;
	orderTimeTail = orderTimeTail->next;
	return;
}

void imp_res::BLUE(int num)
{
	if (tableSize == 0)
		return;

	for (int i = 0; i < num && tableSize != 0; i++)
	{
		TimeLine *TimeLineDelete = this->findCustomerDelete();
		this->deleteCustomerRefTime(TimeLineDelete);
	}
	this->queueToTable();
}
void imp_res::PURPLE()
{
	// solution<<"PURPLE"<<endl;
	int numOfSwap = 0;
	int n = 0; //! kích thước danh sách cần sort
	customer *temp = queueHead;
	customer *max = queueHead; // khách hàng có energy cao nhất
	for (int i = 0; i < queueSize; i++)
	{
		if (abs(temp->energy) >= abs(max->energy))
		{
			max = temp;
			n = i + 1;
		}
		temp = temp->next;
	}

	for (int i = n / 2; i > 2; i /= 2)
	{
		for (int j = 0; j < i; j++)
		{
			numOfSwap += insertSort(this->getCusAt(queueHead, j), n - j, i);
		}
	}
	numOfSwap += insertSort(queueHead, n, 1);
	// solution<<"The number of swaps: "<<numOfSwap<<endl;
	this->BLUE(numOfSwap % MAXSIZE);
}

void imp_res::DOMAIN_EXPANSION()
{
	if (tableSize == 0)
		return;

	int total_magic = 0;//! chu thuat su
	int total_ghost = 0;//! chu thuat su va oan linh
	TimeLine *temp = orderTimeHead;
	for (int i = 0; i < tableSize + queueSize; i++)
	{
		temp->data->energy > 0 ? total_magic += temp->data->energy : total_ghost += temp->data->energy;
		temp = temp->next;
	}
	TimeLine *magicTail = nullptr;
	TimeLine *ghostTail = nullptr;
	TimeLine *magicHead = nullptr;
	TimeLine *ghostHead = nullptr;

	TimeLine *tempTime = orderTimeHead;
	for (int i = 0; i < tableSize + queueSize; i++)
	{

		if (tempTime->data->energy > 0)
		{

			if (magicHead == nullptr)
			{
				magicHead = magicTail = tempTime;
			}
			else
			{
				magicTail->next = tempTime;
				tempTime->prev = magicTail;
				magicTail = magicTail->next;
			}
		}
		else
		{
			if (ghostHead == nullptr)
			{
				ghostHead = ghostTail = tempTime;
			}
			else
			{
				ghostTail->next = tempTime;
				tempTime->prev = ghostTail;
				ghostTail = ghostTail->next;
			}
		}

		tempTime = tempTime->next;
	}

	if (ghostHead != nullptr)
		ghostHead->prev = ghostTail->next = nullptr;
	if (magicHead != nullptr)
		magicHead->prev = magicTail->next = nullptr;

	//! khúc này là đuổi từ thằng vô trễ nhất -> vô sớm nhất
	//! nếu mà có thay đổi sẽ thay ....Tail->....Head
	if (abs(total_ghost) <= total_magic)
	{

		reverse_print(ghostHead);
		while (ghostHead != nullptr)
		{
			TimeLine *TimeLineDelete = ghostHead;
			ghostHead = ghostHead->next;
			if (ghostHead)
				ghostHead->prev = nullptr;
			this->deleteCustomerRefTime(TimeLineDelete);
		}

		orderTimeHead = magicHead;
		orderTimeTail = magicTail;
	}
	else
	{
		reverse_print(magicHead);
		while (magicHead != nullptr)
		{
			TimeLine *TimeLineDelete = magicHead;
			magicHead = magicHead->next;
			if (magicHead)
				magicHead->prev = nullptr;
			this->deleteCustomerRefTime(TimeLineDelete);
		}

		orderTimeHead = ghostHead;
		orderTimeTail = ghostTail;
	}

	this->queueToTable();
}

void imp_res::REVERSAL()
{
	if (tableSize <= 1)
		return;

	customer *head = nullptr;
	customer *tail = nullptr;
	customer *virX = nullptr;
	head = virX = customerX;
	// TÌM OÁN LINH
	for (int i = 0; i < tableSize; i++)
	{
		if (head->energy < 0)
			break;
		head = head->prev;
	}

	tail = customerX->next;
	for (int i = 0; i < tableSize; i++)
	{
		if (tail->energy < 0)
		{
			break;
		}
		tail = tail->next;
	}

	if (head->energy < 0 && head != tail)
	{

		for (int i = 0; i < tableSize / 2; i++)
		{
			this->swapNode(head, tail);

			customer *temp = head;
			head = tail;
			tail = temp;
			if (tail == customerX)
				virX = head;
			head = head->prev;
			for (int i = 0; i < tableSize; i++)
			{
				if (head->energy < 0)
					break;
				head = head->prev;
			}

			if (head == tail)
				break;

			tail = tail->next;
			for (int i = 0; i < tableSize; i++)
			{
				if (tail->energy < 0)
					break;
				tail = tail->next;
			}
			if (head == tail)
				break;
		}
	}

	head = virX;
	for (int i = 0; i < tableSize; i++)
	{
		if (head->energy > 0)
		{
			break;
		}
		head = head->prev;
	}

	tail = virX->next;
	for (int i = 0; i < tableSize; i++)
	{
		if (tail->energy > 0)
		{
			break;
		}
		tail = tail->next;
	}

	if (head->energy > 0 && head != tail)
	{
		for (int i = 0; i < tableSize / 2; i++)
		{
			this->swapNode(head, tail);

			customer *temp = head;
			head = tail;
			tail = temp;

			head = head->prev;
			for (int i = 0; i < tableSize; i++)
			{
				if (head->energy > 0)
					break;
				head = head->prev;
			}

			if (head == tail)
				break;

			tail = tail->next;
			for (int i = 0; i < tableSize; i++)
			{
				if (tail->energy > 0)
					break;
				tail = tail->next;
			}
			if (head == tail)
				break;
		}
	}
}
void imp_res::UNLIMITED_VOID()
{
	if (tableSize <= 3)
		return;

	int MIN_TOTAL = 2147483647;
	customer *head = nullptr, *tail = nullptr;

	customer *tempi = customerX;
	int lenOfSequence = -1;
	for (int i = 0; i < tableSize; i++)
	{
		int TOTAL = 0;

		customer *tempj = tempi;
		for (int j = i; j < tableSize + i; j++)
		{
			TOTAL += tempj->energy;
			if (j - i > 2)
			{
				if (MIN_TOTAL > TOTAL || (MIN_TOTAL == TOTAL && j - i >= lenOfSequence))
				{
					MIN_TOTAL = TOTAL;
					lenOfSequence = j - i;

					head = tempi;
					tail = tempj;
				}
			}

			tempj = tempj->next;
		}
		tempi = tempi->next;
	}

	customer *minElement = head;
	customer *tmp = head->next;
	while (tmp != tail->next)
	{
		if (tmp->energy < minElement->energy)
		{
			minElement = tmp;
		}
		tmp = tmp->next;
	}
	tmp = minElement->next;
	minElement->print();
	while (tmp != minElement)
	{
		if (tmp == tail->next)
		{
			tmp = head;
			if (head == minElement)
				break;
		}
		tmp->print();
		tmp = tmp->next;
	}
}

void imp_res::LIGHT(int num)
{
	if (num == 0)
	{
		customer *temp = queueHead;
		for (int i = 0; i < queueSize; i++)
		{

			temp->print();
			temp = temp->next;
		}
	}
	else if (num > 0)
	{
		customer *temp = customerX;
		for (int i = 0; i < tableSize; i++)
		{
			temp->print();
			temp = temp->next;
		}
	}
	else if (num < 0)
	{
		customer *temp = customerX;
		for (int i = 0; i < tableSize; i++)
		{
			temp->print();
			temp = temp->prev;
		}
	}
}

bool imp_res::is_Exist(customer *t, string name, int energy)
{
	for (int i = 0; i < tableSize; i++)
	{
		if (t->name == name)
		{
			return true;
		}
		t = t->next;
	}
	t = queueHead;
	for (int i = 0; i < queueSize; i++)
	{
		if (t->name == name)
		{
			return true;
		}
		t = t->next;
	}
	return false;
}

imp_res::TimeLine *imp_res::findCustomerDelete()
{
	TimeLine *TimeLineDelete = orderTimeHead;
	for (int i = 0; i < queueSize + tableSize; i++)
	{
		if (TimeLineDelete->onTable)
			break;
		TimeLineDelete = TimeLineDelete->next;
	}

	if (queueSize + tableSize == 1)
	{
		orderTimeHead = orderTimeTail = nullptr;
	}
	else if (TimeLineDelete == orderTimeHead)
	{
		orderTimeHead = orderTimeHead->next;
		orderTimeHead->prev = nullptr;
	}
	else if (TimeLineDelete == orderTimeTail)
	{
		orderTimeTail = orderTimeTail->prev;
		orderTimeTail->next = nullptr;
	}
	else
	{
		TimeLineDelete->prev->next = TimeLineDelete->next;
		TimeLineDelete->next->prev = TimeLineDelete->prev;
	}

	return TimeLineDelete;
}

void imp_res::deleteCustomerRefTime(TimeLine *TimeLineDelete)
{
	if (TimeLineDelete->onTable == true)
	{
		if (tableSize == 1)
		{
			tableSize = 0;
			customerX = nullptr;
		}
		else
		{
			customer *customerDelete = TimeLineDelete->data;
			customer *beforeDeleted = customerDelete->prev;
			customer *afterDeleted = customerDelete->next;
			beforeDeleted->next = customerDelete->next;
			afterDeleted->prev = customerDelete->prev;
			if (customerDelete->energy > 0)
			{
				customerX = afterDeleted;
			}
			else
			{
				customerX = beforeDeleted;
			}
			tableSize--;
		}
	}
	else
	{
		if (queueSize == 1)
		{
			queueSize = 0;
			queueHead = nullptr;
		}
		else
		{
			customer *customerDelete = TimeLineDelete->data;
			customer *beforeDeleted = customerDelete->prev;
			customer *afterDeleted = customerDelete->next;
			beforeDeleted->next = customerDelete->next;
			afterDeleted->prev = customerDelete->prev;

			if (customerDelete == queueHead)
				queueHead = queueHead->next;
			queueSize--;
		}
	}

	delete TimeLineDelete;
}

void imp_res::queueToTable()
{
	while (queueSize != 0 && tableSize != MAXSIZE)
	{
		customer *newCus = queueHead;
		TimeLine *temp = orderTimeHead;
		for (int i = 0; i < queueSize + tableSize; i++)
		{
			if (temp->onTable == false && temp->data == newCus)
			{
				temp->onTable = true;
				break;
			}
			temp = temp->next;
		}
		if (queueSize == 1)
			queueHead = nullptr;
		else
		{
			queueHead->prev->next = queueHead->next;
			queueHead->next->prev = queueHead->prev;
			queueHead = queueHead->next;
		}
		queueSize--;
		if (tableSize == 0)
		{
			customerX = newCus;
			newCus->next = newCus->prev = newCus;
		}
		else
		{
			if (tableSize >= MAXSIZE / 2)
			{
				customer *temp = customerX;
				// biến lưu giá trị lệch Energy lớn nhất;
				int maxDiff = abs(temp->energy - newCus->energy);
				// khách hàng X mới
				customer *newX = customerX;
				for (int i = 0; i < tableSize; i++)
				{
					if (maxDiff < abs(temp->energy - newCus->energy))
					{
						maxDiff = abs(temp->energy - newCus->energy);
						newX = temp;
					}
					temp = temp->next;
				}
				customerX = newX;
			}
			if (newCus->energy >= customerX->energy)
			{
				customer *tmp = customerX->next;
				customerX->next = newCus;
				newCus->prev = customerX;
				newCus->next = tmp;
				tmp->prev = newCus;
			}
			else
			{
				customer *tmp = customerX->prev;
				newCus->next = customerX;
				tmp->next = newCus;
				customerX->prev = newCus;
				newCus->prev = tmp;
			}
		}
		customerX = newCus;
		tableSize++;
	}
}

void imp_res::reverse_print(TimeLine *head)
{
	if (head != nullptr)
	{
		reverse_print(head->next);
		head->data->print();
	}
}
imp_res::customer *imp_res::getCusAt(customer *x, int index)
{
	for (int i = 0; i < index; i++)
		x = x->next;
	return x;
}
int imp_res::insertSort(customer *head, int n, int gap)
{
	int cnt = 0;
	for (int i = gap; i < n; i += gap)
	{
		for (int j = i; j >= gap; j -= gap)
		{
			customer *a = this->getCusAt(head, j);
			customer *b = this->getCusAt(head, j - gap);

			if (abs(a->energy) < abs(b->energy))
				break;
			else if (abs(a->energy) > abs(b->energy))
			{
				// solution<<j<<a->name<<"   "<< j - gap<<b->name<<endl;
				swapNode(a, b);
				cnt++;
			}
			else if (abs(a->energy) == abs(b->energy))
			{
				if (getIdxQueue(a) < getIdxQueue(b))
				{
					// solution<<j<<a->name<<"   "<< j - gap<<b->name<<endl;
					swapNode(a, b);
					cnt++;
				}
				else
					continue;
			}

			if (b == queueHead && b == head)
				queueHead = head = a;
			else if (b == head)
				head = a;
			// solution<<"STEP:" <<cnt <<endl;
			// LIGHT(0);
			// solution<<"-----------------------------------"<<endl;
		}
	}
	return cnt;
}

int imp_res::getIdxQueue(customer *t)
{
	TimeLine *temp = orderTimeTail;
	for (int i = queueSize + tableSize - 1; i >= 0; i--)
	{
		if (temp->data == t)
			return i;
		temp = temp->prev;
	}
	return -1;	
}

void imp_res::swapNode(customer *head, customer *tail)
{
	customer *temp = new customer();

	head->prev->next = temp;
	head->next->prev = temp;
	temp->next = head->next;
	temp->prev = head->prev;
	head->next = head->prev = nullptr;

	tail->prev->next = head;
	tail->next->prev = head;
	head->prev = tail->prev;
	head->next = tail->next;
	tail->next = tail->prev = nullptr;

	temp->next->prev = tail;
	temp->prev->next = tail;
	tail->prev = temp->prev;
	tail->next = temp->next;

	delete temp;
}