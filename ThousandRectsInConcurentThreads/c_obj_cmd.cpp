#include "c_obj_cmd.h"

c_obj_cmd::c_obj_cmd(QObject *parent)
	: QObject(parent),
	curr_par_device(),
	_parent(parent)
{

}

c_obj_cmd::~c_obj_cmd()
{

}
