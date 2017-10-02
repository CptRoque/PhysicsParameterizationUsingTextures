#pragma once

template <typename T>
class List
{
public:
	List()
	{
		_startPointer = nullptr;
		_length = 0;
		_capacity = 0;
	}

	List(int length) : List()
	{
		IncreaseCapacity(length);
	}

	List(const List<T> &list) : List(list.GetLength())
	{
		for (int i = 0; i < list.GetLength(); i++)
		{
			_startPointer[i] = list[i];
		}
	}

	~List()
	{
		if (_startPointer != nullptr) 
		{ 
			free(_startPointer); 
		}
	}

	int Add(T obj)
	{
		if (_startPointer == nullptr || _length == _capacity)
		{
			IncreaseCapacity(_incrementSize);
		}
		_startPointer[_length] = obj;
		return _length++;
	}

	void Concat(const List<T> &list)
	{
		if (_startPointer == nullptr) return;

		for (int i = 0; i < list.GetLength(); ++i)
		{
			if (!Contains(list[i]))
			{
				Add(list[i]);
			}
		}
	}

	bool Contains(const T& obj) const
	{
		if (_startPointer == nullptr) return false;

		for (int i = 0; i < _length; i++)
		{
			if (_startPointer[i] == obj)
			{
				return true;
			}
		}
		return false;
	}

	int GetIndexOf(const T& obj) const
	{
		if (_startPointer == nullptr) return -1;

		for (int i = 0; i < _length; i++)
		{
			if (_startPointer[i] == obj)
			{
				return i;
			}
		}
		return -1;
	}

	void Remove(int index)
	{
		if (_startPointer == nullptr || index < 0 || index >= _length) return;

		for (int i = index; i < _length - 1; i++)
		{
			_startPointer[i] = _startPointer[i + 1];
		}
		--_length;
	}

	void Remove(const T &obj)
	{
		Remove(GetIndexOf(obj));
	}

	void Remove(const List<T> &list)
	{
		if (list.GetLength() == 0) return;

		List<int> indices;
		for (int i = 0; i < list.GetLength(); ++i)
		{
			int index = GetIndexOf(list[i]);
			if (index >= 0) indices.Add(index);
		}
		indices.Sort();

		int indexCount = 1;
		for (int i = indices[0]; i < _length - indices.GetLength() && i + indexCount < _length; i++)
		{
			if (indexCount < indices.GetLength() && i + indexCount == indices[indexCount]) indexCount++;
			_startPointer[i] = _startPointer[i + indexCount];
		}
		_length -= indices.GetLength();
	}

	//Does not deallocate memory
	void RemoveAll()
	{
		_length = 0;
	}

	void Clear()
	{
		RemoveAll();
		TrimToLength();
	}

	void TrimToLength()
	{
		if (_startPointer == nullptr) return;

		if (_length == 0)
		{
			free(_startPointer);
			_startPointer = nullptr;
			_capacity = 0;
		}
		else
		{
			SetCapacity(_length);
		}
	}

	//Sorts from smallest to largest, in case of numbers
	void Sort()
	{
		for (int i = 0; i < _length; ++i)
		{
			for (int j = i + 1; j < _length; j++)
			{
				if (_startPointer[j] < _startPointer[i])
				{
					int temp = _startPointer[i];
					_startPointer[i] = _startPointer[j];
					_startPointer[j] = temp;
				}
			}
		}
	}

	int GetLength() const
	{
		return _length;
	}

	int GetCapacity() const
	{
		return _capacity;
	}

	bool IncreaseCapacity(int increment)
	{
		if (increment < 1) return false;

		return SetCapacity(_capacity + increment);
	}

	const T& operator[](int i) const
	{
		return _startPointer[i];
	}

	T&       operator[](int i)
	{
		return _startPointer[i];
	}

private:
	T* _startPointer = nullptr;
	int _length = 0;
	int _capacity = 0;

	static const int _incrementSize = 10;

	bool SetCapacity(int capacity)
	{
		if (capacity < _length)
		{
			return false;
		}

		int size = sizeof(T);
		int newSize = (capacity * size);
		T* tempPointer = (T*)realloc(_startPointer, newSize);
		if (tempPointer != nullptr)
		{
			_startPointer = tempPointer;
			
			if (capacity > _capacity)
			{
				for (int i = _capacity; i < capacity; ++i)
				{
					_startPointer[i] = T();
				}
			}

			_capacity = capacity;
			return true;
		}
		return false;
	}
};