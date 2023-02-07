#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenuBar>

class MainMenu : public QMenuBar
{
	Q_OBJECT

protected:


	QAction *ConnectAction;

	QAction *mab_setup;
	QAction *chanel_setup;
	QAction *algor_setup;
	QAction *selection_setup;


	const int action_text_size;
	const int main_text_size;

	bool conn_state;

public:
	MainMenu(QWidget *parent, QObject *reseiver);
	virtual ~MainMenu();

	QMenu subMenuFile;
	QMenu subMenuOpt;

	void SetConnectionState(const bool state);

	bool GetMenuConnState() const;


	void SetExtraEditingEnabled(const bool enabled);

};

#endif // MAINMENU_H
