#ifndef C_OBJ_CMD_H
#define C_OBJ_CMD_H

#include <QObject>
#include "dev_struct.h"
class c_obj_cmd : public QObject
{
	Q_OBJECT

public:
	c_obj_cmd(QObject *parent);
	~c_obj_cmd();
	par_device_t curr_par_device;		// параметры дефектоскопа (все)

private:
	QObject* _parent;
};

#endif // C_OBJ_CMD_H
