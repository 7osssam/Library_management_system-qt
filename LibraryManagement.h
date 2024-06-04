// LibraryManagement.h
#ifndef LIBRARYMANAGEMENT_H
#define LIBRARYMANAGEMENT_H

#include "Book.h"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class LibraryManagement
{
private:
	std::vector<Book> books;
	void			  loadBooks();
	void			  saveBooks();

public:
	LibraryManagement();
	void			  addBook(const Book& book);
	void			  removeBook(const std::string& barcode);
	std::vector<Book> searchBooks(const std::string& title);
	std::vector<Book> filterBooks(const std::string& category);

	std::vector<Book> getBooks() const;

	bool openBook(const std::string& barcode);
};

#endif // LIBRARYMANAGEMENT_H
