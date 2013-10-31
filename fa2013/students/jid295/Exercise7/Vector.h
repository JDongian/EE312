#include <stdint.h>
#include <assert.h>

struct Vector {
private:
	uint32_t length;
	uint32_t capacity;
	int32_t* data;
public:
	Vector(uint32_t initial_size) {
		length = initial_size;
		capacity = initial_size;
		data = new int32_t[initial_size];
	}

	Vector(Vector& that) {
		copy(that);
	}

	~Vector(void) {
		destroy();
	}

	Vector& operator=(Vector& that) {
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}

	int32_t& operator[](uint32_t k) {
		assert(k < length);
		return data[k];
	}

	uint32_t size() { 
		return this->length; 
	}

	void push_back(int32_t v) {
		if (length == capacity) {
			capacity = capacity * 2;
			int32_t* p = new int32_t[capacity];

			for (uint32_t k = 0; k < length; k += 1) {
				p[k] = data[k];
			}

			delete[] data;
			data = p;
		}

		data[length] = v;
		length += 1;
	}

	void pop_back(void) {
		length -= 1;
	}

private:
	void copy(Vector& that) {
		this->length = that.length;
		this->capacity = this->length;
		this->data = new int32_t[this->capacity];
		for (uint32_t k = 0; k < that.length; k += 1) {
			this->data[k] = that.data[k];
		}
	}

	void destroy(void) {
		delete[] data;
	}
};

