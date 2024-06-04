// Book.h
#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book
{
public:
	std::string title;
	std::string author;
	std::string category;
	std::string barcode;

	Book(const std::string& title, const std::string& author, const std::string& category,
		 const std::string& barcode) : title(title), author(author), category(category), barcode(barcode)
	{
	}
};

#endif // BOOK_H
