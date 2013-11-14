#ifndef _HashTable_h
#define _HashTable_h 1

#include "String.h"

struct HashTable {
private:
	struct Cell { // used to make linked lists
		Cell* next;
		String val;
		Cell(void) {}
		Cell(const String& s) : val(s) { 
			next = 0;
		}
	};

	static unsigned smartHash(String s) {
		unsigned h = 0;
		for (int k = 0; k < s.size(); k += 1) {
			h = (h << 5) + h + s[k];
		}
		return h;
	}


	typedef Cell* Chain;
	/* the table is an array of pointers to Cells
	 * i.e., and array of chains */
	Chain* table;
	int len; // number of keys in the table
	int cap; // number of chains in the table (i.e., array size)

	static const int initial_size = 8;

	HashTable(HashTable&) {} // I don't want to make copies of my HashTable in Project10, so I'm "hiding" the copy constructor
	HashTable& operator=(HashTable&) { return *this; } // ditto the assignment operator, no copies in Project10
public:
	HashTable(void) {
		cap = initial_size;
		len = 0;
		table = new Chain[cap];
		/* we need to initialize all the entries in the table to be null pointers (they're not null by default) */
		for (int k = 0; k < cap; k += 1) {
			table[k] = 0;
		}
	}

	~HashTable(void) { destroy(); }

	bool isMember(const String& key) {
		Cell* p = locate(key);
		while (p != 0) {
			if (p->val == key) { return true; }
			p = p->next;
		}
		return false;
	}

	void insert(const String& key) {
		if (isMember(key)) { return; }

		if (len > 0.8 * cap) { resize(); }

		Chain& p = locate(key);
		Cell* new_cell = new Cell(key);
		new_cell->next = p;
		p = new_cell;
		len += 1;
	}

	void remove(const String& key) {
		if (!isMember(key)) { return; }
		Chain& p = locate(key);
		Cell* delete_me = p;
		if (p->val == key) { 
			p = delete_me->next; 
			delete delete_me;
		} else {
			while (true) {
				Chain& p = delete_me->next;
				delete_me = delete_me->next;
				if (delete_me->val == key) {
					p = delete_me->next;
					delete delete_me;
					break;
				}
			}
		}
		len -= 1;
	}

	unsigned longestChain(void) {
		unsigned max_len = 0;
		for (int k = 0; k < cap; k += 1) {
			int chain_len = 0;
			Cell* p = table[k];
			while (p != 0) {
				chain_len += 1;
				p = p->next;
			}
			if (chain_len > max_len) { max_len = chain_len; }
		}
		return max_len;
	}

private:
	Chain& locate(const String& key) const {
		/* Step1: Ensure we know the log of the capacity so that we can use the Knuth multiplication method
		 * to convert hashcodes into array indexes */
		/* it's kinda a pain to calculate the log of the capacity
		 * so I'll recaculate the log (i.e., the shift amount for my Knuth multiplication) only when
		 * capacity has changed since the last time I calculated a shift amount */
		static int last_cap = 0; // static local variables are really globals, they're not stored on the stack
		static int shift = 32; // static local variables are not re-initialized each time the function is called
		if (cap != last_cap) {
			last_cap = cap;
			unsigned c = cap;
			shift = 32;
			while (c > 1) {
				shift = shift - 1;
				c = c / 2;
			}
		}

		/* Step 2: Select which Chain in the table this key should be stored in
		 * and return a reference to the first pointer in the Chain */

		/* use the Knuth Multiplication method to convert the HashCode into a table index */
		unsigned k = smartHash(key);
		k = (k * 0x6b43a9b5) >> shift;
		return table[k];
	}

	void destroy(void) {
		for (int k = 0; k < cap; k += 1) {
			Cell* p = table[k];
			while (p != 0) {
				Cell* q = p;
				p = p->next;
				q->next = 0;
				delete q;
			}
			table[k] = 0; // this line is important if and only if Cell has a destructor. Can you figure out why?
		}
		delete[] table;
	}

	void resize(void);
};


#endif /* _HashTable_h */