#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LibraryManagement.h"
#include "UserManagement.h"
#include "Memento.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onLoginButtonClicked();
	void onAddBookButtonClicked();
	void onSearchButtonClicked();
	void onFilterButtonClicked();
	void onRemoveBookButtonClicked();
	void onSaveUserButtonClicked();
	void onRestoreUserButtonClicked();
	void onSignupButtonClicked(); // New slot for sign-up button
	void updateDatabaseView();
	void onBackToLoginButtonClicked();
	void onOpenBookButtonClicked();

private:
	Ui::MainWindow*	  ui;
	LibraryManagement library;
	UserManagement	  userManagement;
	Memento			  memento;
	std::string		  currentUser;
	bool			  isAdmin;

	void updateUI();
	void showAdminScreen();
	void showUserScreen();
	void addBackToLoginButton();
};

#endif // MAINWINDOW_H
