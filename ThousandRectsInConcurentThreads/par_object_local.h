#ifndef PAR_OBJECT_LOCAL_H
#define PAR_OBJECT_LOCAL_H

#include <QString>



struct par_setting_local
{
	QString ParamDefinition;

	QString sizing;						// размеры (генерируются на основе параметров контроля) (? !)

	QString object_length;
	QString material;


	QString acus_block;
	QString document;
	QString reflector;
	QString SOP;

	
};


struct defect_count_t
{
	int defect_long_outside;
	int defect_long_inside;

	int defect_travers_outside;
	int defect_travers_inside;

	int defect_lamination;

	int defect_low_thick;
	int defect_high_thick;

	int defect_no_ak;
};


struct par_object_local
{
	QString month;					// текущий месяц
	QString date;
	QString shift;						// смена
	QString control_time;


	QString defectoskopist;
	QString tabel_num;						// привязан к дефектоскописту ?


	QString consignment;
	QString sertificate;


	int obj_num;
	QString control_area;						// участок контроля


	int obj_code;
	defect_count_t defects;


	float L_obj;			// длина трубы
	float V_obj;			// скорость контроля ?

	float H_min;


	QString controlling_time;			// не учитывается в базах данных
	QString local_file_path;			// ???


	enum ObjectStateCodes
	{
		Unknown = 0,
		Controlling = 1,
		Stopped = 2,
		OK = 3,
		Broken = 4
	};

	int number_miss_coord;
};




#endif // PAR_OBJECT_LOCAL_H