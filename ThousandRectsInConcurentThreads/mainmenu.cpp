#include "mainmenu.h"
#include <QApplication>


MainMenu::MainMenu(QWidget *parent, QObject *reseiver) :QMenuBar(parent),
		ConnectAction(nullptr),
		mab_setup(nullptr),
		chanel_setup(nullptr),
		algor_setup(nullptr),
		selection_setup(nullptr),
		action_text_size(10),
		main_text_size(12),
		conn_state(false),

		subMenuFile( tr("\320\244\320\260\320\271\320\273") ),//Файл
		subMenuOpt( tr("\320\236\320\277\321\206\320\270\320\270") )//Опции
{

	ConnectAction = this->addAction( tr("\320\237\321\203\321\201\320\272 F3"), reseiver, SLOT(StartStopAction()) );//Пуск F3


	subMenuFile.addAction( tr("\320\237\320\265\321\200\320\265\320\271\321\202\320\270 \320\262 \321\200\320\260\320\261\320\276\321\207\320\270\320\271 \321\200\320\265\320\266\320\270\320\274"),
		//Перейти в рабочий режим
		reseiver, SLOT(SwitchToViewMode()), QKeySequence(Qt::Key_F11) );

	subMenuFile.addSeparator();

	subMenuFile.addAction( tr("\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\321\203"), 
		//Загрузить настройку
		reseiver, SLOT(butt_load_par()), QKeySequence(Qt::CTRL + Qt::Key_L) );
	subMenuFile.addAction( tr("\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\321\203"), 
		//Сохранить настройку
		reseiver, SLOT(butt_save_par()), QKeySequence(Qt::CTRL + Qt::Key_S) );

	subMenuFile.addSeparator();


	subMenuFile.addAction( tr("\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262 \320\277\321\200\320\270\320\261\320\276\321\200\320\265"),
		//Сохранить в приборе
		reseiver, SLOT(save_par_in_dev()) );
	subMenuFile.addAction( tr("\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\270\320\267 \320\277\321\200\320\270\320\261\320\276\321\200\320\260"),
		//Загрузить из прибора
		reseiver, SLOT(load_par_from_dev()) );

	subMenuFile.addSeparator();

	subMenuFile.addAction( tr("\320\237\320\265\321\200\320\265\321\201\320\276\320\267\320\264\320\260\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272"),
		//Пересоздать список настроек
		reseiver, SLOT(regenerate_auto_par_set()) );



//	subMenuOpt.addSeparator();

	subMenuOpt.addAction( tr("\320\224\320\270\321\200\320\265\320\272\321\202\320\276\321\200\320\270\320\270..."),
		//Директории...
		reseiver, SLOT(change_directories()), QKeySequence(Qt::CTRL + Qt::Key_D) );
	subMenuOpt.addAction( tr("\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213"), 
		//Параметры
		reseiver, SLOT(setup_params()), QKeySequence(Qt::CTRL + Qt::Key_P) );
	


	mab_setup = subMenuOpt.addAction( tr("\320\241\321\205\320\265\320\274\320\260 \320\234\320\220\320\221"),
		//Схема МАБ
		reseiver, SLOT(show_mab_setup()) );
	chanel_setup = subMenuOpt.addAction( tr("\320\241\321\205\320\265\320\274\320\260 \320\272\320\260\320\275\320\260\320\273\320\276\320\262"),
		//Схема каналов
		reseiver, SLOT(show_channel_setup()) );
	algor_setup = subMenuOpt.addAction( tr("\320\220\320\273\320\263\320\276\321\200\320\270\321\202\320\274\321\213"), reseiver, SLOT(show_algor_setup()) );
	//Алгоритмы
	selection_setup = subMenuOpt.addAction( tr("\320\237\321\200\320\270\320\274\320\265\320\275\320\265\320\275\320\270\320\265"), reseiver, SLOT(show_selection_setup()) );
	//Применение


	QFont font;
    font.setPointSize(action_text_size);
	subMenuFile.setFont(font);
	subMenuOpt.setFont(font);
		

	addMenu(&subMenuFile);
	addMenu(&subMenuOpt);
    font.setPointSize(main_text_size);
	setFont(font);


}


MainMenu::~MainMenu()
{
}


void MainMenu::SetConnectionState(const bool state)
{
	if(conn_state != state)
	{
		conn_state = state;
		ConnectAction->setText( state ? tr("\320\241\321\202\320\276\320\277 F3") : tr("\320\237\321\203\321\201\320\272 F3") );//Пуск F3 Стоп F3
	}
}

bool MainMenu::GetMenuConnState() const
{
	return conn_state;
}

void MainMenu::SetExtraEditingEnabled(const bool enabled)
{
	mab_setup->setEnabled(enabled);
	chanel_setup->setEnabled(enabled);
	algor_setup->setEnabled(enabled);
	selection_setup->setEnabled(enabled);
}
