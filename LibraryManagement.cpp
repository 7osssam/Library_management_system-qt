// LibraryManagement.cpp
#include "LibraryManagement.h"
#include <fstream>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include "logger.h"

#define LIBRARY_FILE "../data/books.json"

using json = nlohmann::json;

LibraryManagement::LibraryManagement()
{
	loadBooks();
}

void LibraryManagement::loadBooks()
{
	// Define and initialize libraryFilePath and libraryFile inside the loadBooks function
	QString libraryFilePath = QDir(QCoreApplication::applicationDirPath()).filePath(LIBRARY_FILE);
	QFile libraryFile(libraryFilePath);

	std::ifstream inFile(libraryFile.fileName().toStdString());
	if (inFile)
	{
		json j;
		inFile >> j;
		for (const auto &item : j)
		{
			books.emplace_back(item["title"], item["author"], item["category"], item["barcode"]);
			qDebug() << "Loaded book: " << item["title"].get<std::string>().c_str();
		}
	}
}

void LibraryManagement::saveBooks()
{
	// Define and initialize libraryFilePath and libraryFile inside the saveBooks function
	QString libraryFilePath = QDir(QCoreApplication::applicationDirPath()).filePath(LIBRARY_FILE);
	QFile libraryFile(libraryFilePath);

	json j = json::array();
	for (const auto &book : books)
	{
		j.push_back({{"title", book.title},
					 {"author", book.author},
					 {"category", book.category},
					 {"barcode", book.barcode}});
		qDebug() << "Saved book: " << book.title.c_str();
	}
	std::ofstream outFile(libraryFile.fileName().toStdString());
	outFile << j.dump(4);
}

void LibraryManagement::addBook(const Book &book)
{
	books.push_back(book);
	saveBooks();
}

void LibraryManagement::removeBook(const std::string &barcode)
{
	books.erase(std::remove_if(books.begin(), books.end(),
							   [&barcode](const Book &book)
							   {
								   return book.barcode == barcode;
							   }),
				books.end());

	qDebug() << "Removed book: " << barcode.c_str();
	saveBooks();
}

std::vector<Book> LibraryManagement::searchBooks(const std::string &title)
{
	std::vector<Book> result;
	for (const auto &book : books)
	{
		if (book.title.find(title) != std::string::npos)
		{
			result.push_back(book);
			qDebug() << "Search: Found book: " << book.title.c_str();
		}
	}
	return result;
}

std::vector<Book> LibraryManagement::filterBooks(const std::string &category)
{
	std::vector<Book> result;
	for (const auto &book : books)
	{
		if (book.category == category)
		{
			result.push_back(book);
			qDebug() << "Filder: Found book: " << book.title.c_str();
		}
	}
	return result;
}

std::vector<Book> LibraryManagement::getBooks() const
{
	return books;
}

bool LibraryManagement::openBook(const std::string &barcode)
{
	// open the book (.txt file) with the barcode as the filename using process control
	QString bookFilePath = QDir(QCoreApplication::applicationDirPath())
							   .filePath("../data/" + QString::fromStdString(barcode) + ".txt");
	QFile bookFile(bookFilePath);

	if (bookFile.open(QIODevice::ReadOnly))
	{
		QProcess::startDetached("notepad.exe", QStringList() << bookFile.fileName());
		qDebug() << "Opened book: " << barcode.c_str();
		return true;
	}

	return false;
}
