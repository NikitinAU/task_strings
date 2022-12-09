#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
//_______________________TASK 6_____________________
//create new string using only repeating symbols from the original
//use only one of each repeating
//symbols don't have to be in any specific order

//implimentation analysis:
/*let n in INTEGERS be a number of characters in string:
 *	let's find n so that O(n^2) > O(n + 256)
 *		n*n = n + 256 [-n - 256]
 *		n*n - n - 256 = 0 [quadratic equation]
 *		Positive solution: ~16.507810593582121716220544186554533161301050331552547213823181566
 *		for any number of characters > 16 this solution will be faster than O(n^2)
 *	let's find n so that O(n*log2(n)) > O(n + 256)
 *		n*log2(n) = n + 256 [-n - 256]
 *		n*log2(n) - n - 256 = 0
 *		n * (log2(n) - 1) = 256
 *		n = 2 * e^(ProductLog(128 * ln(2))) ~ 53.8767
 *		for any number of characters > 53 this solution will be faster than O(n * log2(n))
 */

//destination <- source
//speed :
//	Best case:	O(n + 256) -> O(n)
//	Worst case: O(n + 256) -> O(n)
//size : O(256) -> O(1)
char* repeatingChars(char* destination, const char* source) 
{
	const short CHAR_SIZE = 255;
	//char can hold any value from 0 to 255
	//using array[255] we can basically create a map, holding number of repeats for any symbol in string
	size_t chars[CHAR_SIZE] = { 0 };
	//counting the number of symbols appearing
	while (*source)
	{
		//since char is unsigned, at 128 we start to go into negatives
		//to prevent this, simply do 127 - char if it's < 0
		++chars[*source < 0 ? -1 * (*source) + 127 : *source];
		++source;
	}
	//adding all numbers which counts are > 1 to the new string
	for (unsigned short i = 0; i < CHAR_SIZE; ++i)
	{
		if (chars[i] > 1) {
			*destination = i;
			++destination;
		}
	}
	*destination = '\0';
	return destination;
}
void swap(char* a, char* b)
{
	const char t = *a;
	*a = *b;
	*b = t;
}
//lb - lower bound, up - upper bound
//those variables are needed so that the string could be passed by reference without calling copy constructor
void quickSort(std::string& str, int lb, int ub) {
	int low = lb, high = ub;
	int pivot = str[(low + high) / 2];
	do {
		while (str[low] < pivot) {
			low++;
		}
		while (str[high] > pivot) {
			high--;
		}
		if (low <= high) {
			swap(&str[low], &str[high]);
			low++; high--;
		}
	} while (low <= high);
	if (lb < high) {
		quickSort(str, lb, high);
	}
	if (low < ub) {
		quickSort(str, low, ub);
	}
}
//sort first, check repeating after
//speed:
//	Best case:	O(n*log2(n) + n) -> O(n * log2(n))
//	Worst case: O(n^2 + n)		 -> O(n^2)
//size: O(n)
std::string repeatingChars(std::string& s)
{
	std::string rep;
	quickSort(s, 0, s.length() - 1);
	int cur = 257; // outside of range of char to include every character
	for (size_t i = 0; i < s.length() - 1; ++i)
	{
		if (s[i] == s[i + 1] && cur != s[i])
		{
			rep.push_back(s[i]);
			cur = s[i];
		}
	}
	return rep;
}
//wrapper for repeating chars with type string
//used to enable string save functionality
//didn't want to copy paste the same code in function
std::string repeatingCharsWrap(std::string &str, bool save = false)
{
	if (save)
	{
		std::string s = str;
		return repeatingChars(s);
	}
	else {
		return repeatingChars(str);
	}
}

void main()
{
	std::ifstream in("infile.txt");
	if (!in.is_open())
	{
		std::cerr << "File cannot be opened\n";
		return;
	}
	while (!in.eof()) {
		size_t len = 0;
		in >> len;
		in.ignore(1);
		if (len > 0)
		{
			char* source = new char[len+1];
			char* dest = new char[len+1];
			in.getline(source, len+1);
			*(source + len) = '\0';
			std::cout << "original string:\n" << source << "\n\n";
			std::string str = source;

			//c type string
			auto start = std::chrono::high_resolution_clock::now();
			repeatingChars(dest, source);
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			std::cout << "with c strings (char pointers):\n" << dest << "\nexecution completed in " << duration.count() << " microseconds\n\n";

			//string type
			start = std::chrono::high_resolution_clock::now();
			std::string rep = repeatingCharsWrap(str);
			stop = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			std::cout << "with type string:\n" << rep << '\n';
			std::cout << "execution completed in " << duration.count() << " microseconds\n\n";

			delete[] source;
			delete[] dest;
		}
	}
	in.close();
}