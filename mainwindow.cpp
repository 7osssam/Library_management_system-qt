#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include "logger.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
	connect(ui->addBookButton, &QPushButton::clicked, this, &MainWindow::onAddBookButtonClicked);

	// ===================== User buttons =====================
	connect(ui->searchButtonUser, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
	connect(ui->filterButtonUser, &QPushButton::clicked, this, &MainWindow::onFilterButtonClicked);
	// ===================== Admin buttons =====================
	connect(ui->searchButtonAdmin, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);
	connect(ui->filterButtonAdmin, &QPushButton::clicked, this, &MainWindow::onFilterButtonClicked);
	connect(ui->removeBookButton, &QPushButton::clicked, this, &MainWindow::onRemoveBookButtonClicked);

	// ===================== User Management buttons =====================
	connect(ui->saveUserButton, &QPushButton::clicked, this, &MainWindow::onSaveUserButtonClicked);
	connect(ui->restoreUserButton, &QPushButton::clicked, this, &MainWindow::onRestoreUserButtonClicked);
	connect(ui->signupButton, &QPushButton::clicked, this, &MainWindow::onSignupButtonClicked);

	updateUI();

	// Add the back-to-login button
	addBackToLoginButton();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onLoginButtonClicked()
{
	QString username = ui->usernameInput->text();
	QString password = ui->passwordInput->text();

	qInfo() << "Login attempt: " << username.toStdString() << ", " << password.toStdString();

	if (userManagement.authenticate(username.toStdString(), password.toStdString()))
	{
		currentUser = username.toStdString();
		isAdmin = userManagement.isAdmin(username.toStdString());
		updateUI();
	}
	else
	{
		QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
	}
}

void MainWindow::onAddBookButtonClicked()
{
	if (!isAdmin)
	{
		qDebug() << "User is not an admin";
		return;
	}
	else
	{
		qDebug() << "User is an admin";
	}

	QString title = QInputDialog::getText(this, "Add Book", "Enter book title:");
	QString author = QInputDialog::getText(this, "Add Book", "Enter book author:");
	QString category = QInputDialog::getText(this, "Add Book", "Enter book category:");
	QString barcode = QInputDialog::getText(this, "Add Book", "Enter book barcode:");

	if (!title.isEmpty() && !author.isEmpty() && !category.isEmpty() && !barcode.isEmpty())
	{
		library.addBook(
			Book(title.toStdString(), author.toStdString(), category.toStdString(), barcode.toStdString()));
	}
	updateDatabaseView();
}

void MainWindow::onSearchButtonClicked()
{
	QString title = QInputDialog::getText(this, "Search Book", "Enter book title:");

	if (title.isEmpty())
		return;

	auto books = library.searchBooks(title.toStdString());

	QString result;
	for (const auto& book: books)
	{
		result +=
			QString::fromStdString("Title: " + book.title + "\nAuthor: " + book.author +
								   "\nCategory: " + book.category + "\nBarcode: " + book.barcode + "\n");
	}

	if (result.isEmpty())
	{
		result = "No books found.";
	}

	QMessageBox::information(this, "Search Results", result);
}

void MainWindow::onFilterButtonClicked()
{
	QString category = QInputDialog::getText(this, "Filter Books", "Enter book category:");
	if (category.isEmpty())
		return;

	auto books = library.filterBooks(category.toStdString());

	QString result;
	for (const auto& book: books)
	{
		result +=
			QString::fromStdString("Title: " + book.title + "\nAuthor: " + book.author +
								   "\nCategory: " + book.category + "\nBarcode: " + book.barcode + "\n");
	}

	if (result.isEmpty())
	{
		result = "No books found.";
	}

	QMessageBox::information(this, "Filter Results", result);
	updateDatabaseView();
}

void MainWindow::onRemoveBookButtonClicked()
{
	if (!isAdmin)
		return;

	QString barcode = QInputDialog::getText(this, "Remove Book", "Enter book barcode:");
	if (barcode.isEmpty())
		return;

	library.removeBook(barcode.toStdString());

	updateDatabaseView();
}

void MainWindow::onSaveUserButtonClicked()
{
	QString username = ui->usernameInput->text();
	QString password = ui->passwordInput->text();
	bool	isAdminChecked = ui->isAdminCheckbox->isChecked();
	userManagement.addUser(username.toStdString(), password.toStdString(), isAdminChecked ? "admin" : "user");
	memento.save(username.toStdString(), password.toStdString());
}

void MainWindow::onRestoreUserButtonClicked()
{
	auto [username, password] = memento.restore();
	ui->usernameInput->setText(QString::fromStdString(username));
	ui->passwordInput->setText(QString::fromStdString(password));
}

void MainWindow::onSignupButtonClicked()
{
	QString username = ui->usernameInput->text();
	QString password = ui->passwordInput->text();
	bool	isAdminChecked = ui->isAdminCheckbox->isChecked();

	QMessageBox::StandardButton confirmButton =
		QMessageBox::question(this, "Confirm Sign Up",
							  "Do you want to sign up with the following credentials?\n"
							  "Username: " +
								  username +
								  "\n"
								  "Password: " +
								  password +
								  "\n"
								  "Admin: " +
								  (isAdminChecked ? "Yes" : "No"),
							  QMessageBox::Yes | QMessageBox::No);
	if (confirmButton == QMessageBox::Yes)
	{
		userManagement.addUser(username.toStdString(), password.toStdString(),
							   isAdminChecked ? "admin" : "user");
		memento.save(username.toStdString(), password.toStdString());
		QMessageBox::information(this, "Sign Up Successful", "You have been signed up successfully!");
	}
}

void MainWindow::updateDatabaseView()
{
	ui->databaseView->clear(); // Clear the list widget before updating

	// Iterate over the database and add items to the list widget
	for (const auto& book: library.getBooks())
	{
		QString itemText =
			QString::fromStdString("Title: " + book.title + "\nAuthor: " + book.author +
								   "\nCategory: " + book.category + "\nBarcode: " + book.barcode);
		ui->databaseView->addItem(itemText);
	}
}

void MainWindow::updateUI()
{
	if (currentUser.empty())
	{
		ui->stackedWidget->setCurrentIndex(0); // Show login screen
	}
	else if (isAdmin)
	{
		showAdminScreen();
	}
	else
	{
		showUserScreen();
	}
}

void MainWindow::showAdminScreen()
{
	ui->stackedWidget->setCurrentIndex(1); // Show admin screen
	ui->addBookButton->setEnabled(true);
	ui->removeBookButton->setEnabled(true);
	updateDatabaseView();
}

void MainWindow::showUserScreen()
{
	ui->stackedWidget->setCurrentIndex(2); // Show user screen
	ui->addBookButton->setEnabled(false);
	ui->removeBookButton->setEnabled(false);

	// open book button to open the book (.txt file) with the barcode as the filename
	QPushButton* open = new QPushButton("Open Book", this);
	connect(open, &QPushButton::clicked, this, &MainWindow::onOpenBookButtonClicked);

	ui->userLayout->addWidget(open);
}

void MainWindow::addBackToLoginButton()
{
	QPushButton* backToLoginButtonAdmin = new QPushButton("Back to Login", this);
	// change color of the button
	backToLoginButtonAdmin->setStyleSheet(" background-color: #FF6347; color: white;");

	connect(backToLoginButtonAdmin, &QPushButton::clicked, this, &MainWindow::onBackToLoginButtonClicked);
	ui->adminLayout->addWidget(backToLoginButtonAdmin);

	QPushButton* backToLoginButtonUser = new QPushButton("Back to Login", this);
	backToLoginButtonUser->setStyleSheet(" background-color: #FF6347; color: white;");
	connect(backToLoginButtonUser, &QPushButton::clicked, this, &MainWindow::onBackToLoginButtonClicked);
	ui->userLayout->addWidget(backToLoginButtonUser);
}

void MainWindow::onBackToLoginButtonClicked()
{
	ui->stackedWidget->setCurrentIndex(0); // Switch to loginPage
}

void MainWindow::onOpenBookButtonClicked()
{
	QString barcode = QInputDialog::getText(this, "Open Book", "Enter book barcode:");
	library.openBook(barcode.toStdString());
}