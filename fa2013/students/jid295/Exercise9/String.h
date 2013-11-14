#ifndef _String_h
#define _String_h
#include <iostream>

struct String {
private:
	char* ptr;
	int len;
public:

	String(const char* src) { construct(src); }

	String(void) { construct(""); }

	String(const String& that) { copy(that); }
	~String(void) { destroy(); }
	String& operator=(const String& that) {
		if (this != &that) { 
			destroy();
			copy(that);
		}
		return *this;
	}

	String& operator=(const char* src) {
		destroy();
		construct(src);
		return *this;
	}

	char operator[](int k) const { return ptr[k]; }
	int size(void) const { return len; }

	void print(std::ostream& out) const {
		out << ptr;
	}

	bool operator==(const String& that) const {
		if (this->len != that.len) { return false; }
		int k = 0;
		while (k < len) {
			if (this->ptr[k] != that.ptr[k]) { return false; }
			k += 1;
		}
		return true;
	}

	bool operator<(const String& that) const {
		int k = 0;
		while (this->ptr[k] == that.ptr[k]) {
			if (this->ptr[k] == 0) { return false; } // strings are equal
			k += 1;
		}
		return this->ptr[k] < that.ptr[k];
	}

	const char* c_str(void) { return ptr; }

private:
	/* for internal use only, clients really shouldn't be calling these functions
	 * in future versions of this String object type, I might remove these functions entirely or
	 * dramatically change their behavior. */


	void construct(const char* src) {
		len = 0;
		while (src[len] != 0) {
			len += 1;
		}
		ptr = new char[len + 1];
		copy(src);
	}

	/* copy(char*) assumes that ptr has been allocated and holds sufficient capacity for src */
	void copy(const char* src) {
		int k = len;
		while (k >= 0) {
			ptr[k] = src[k];
			k -= 1;
		}
	}

	/* copy(String&) is my "real" copy constructor. I always name this function "copy" and its purpose
	 * is to construct a "deep copy" (i.e., an independent object that shares no pointers with the original object) */
	void copy(const String& that) {
		len = that.len;
		ptr = new char[len + 1];
		copy(that.ptr);
	}

	/* destroy(void) is my "real" destructor. I always name this function destroy and my official destructor is always
	 * just a simple one-line call to this function */
	void destroy(void) {
		delete[] ptr;
	}
};

/* some operator overloads for string */

inline std::ostream& operator<<(std::ostream& out, const String& s) {
	s.print(out);
	return out;
}


inline bool operator!=(const String& s1, const String& s2) { return ! (s1 == s2); }
inline bool operator>(const String& s1, const String& s2) { return s2 < s1; }
inline bool operator<=(const String& s1, const String& s2) { return ! (s2 < s1); }
inline bool operator>=(const String& s1, const String& s2) { return ! (s1 < s2); }

	

#endif /* _String_h */