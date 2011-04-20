
#ifndef  __OBJCOUNTER__HPP__
#define  __OBJCOUNTER__HPP__

///A counter which will count the number of objects allocated at any
///given time for classes that inherit it. Good for detecting memory
///leaks. It is taken directly from the wikipedia page on the
///Curiously Recurring Template Patten
///
/// This patten is also covered by Dr. Dobb's at
/// http://drdobbs.com/cpp/184403484

template <typename Derived>
struct Counter {
    Counter() {
        count++;
    }
	Counter(const Counter&) {
		count++;
	}
	static long objCount() {return count;}
protected:
    ~Counter() {
        count--;
    }
    static long count;
};
template <typename Derived> long Counter<Derived>::count( 0 );



#endif
