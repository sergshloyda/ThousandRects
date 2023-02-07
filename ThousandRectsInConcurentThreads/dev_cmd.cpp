#include "dev_cmd.h"


c_dev_cmd::c_dev_cmd(QObject *parent) : 
		c_base_dev_cmd(parent)
{
}

c_dev_cmd::~c_dev_cmd()
{
}


bool c_dev_cmd::dev_get_cur_rej(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_REJ, btmp);
}

bool c_dev_cmd::dev_set_cur_rej(quint8 rej)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SET_REJ, &rej, 1);
}






bool c_dev_cmd::dev_get_osc(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_OSC, btmp);
}

/*bool c_dev_cmd::dev_get_spectr(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_SPECTR, btmp);
}*/


bool c_dev_cmd::dev_get_amps(quint8 *btmp, quint16 *len)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_AMPS, nullptr, 0, btmp, len);
}

bool c_dev_cmd::dev_get_version_str(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_VERSION, btmp);
}









bool c_dev_cmd::dev_put_param(const char *iparam, quint16 num_bytes)
{
	return send_and_wait(CMD_PUT_DAT, NUM_WR_N_AUTO_PAR_CMD, reinterpret_cast<const quint8 *>(iparam), num_bytes);
}

bool c_dev_cmd::dev_rd_par_dev(quint8 *btmp)
{
	return send_and_wait(CMD_REQ_DAT, NUM_RD_AUTO_PAR_DEV_CMD, nullptr, 0, btmp);
}

bool c_dev_cmd::dev_wr_par_dev(quint16 par)				// запись настройки под новым номером
{
	return send_and_wait(CMD_PUT_DAT, NUM_WR_AUTO_PAR_DEV_CMD, reinterpret_cast<const quint8*>(&par), sizeof(quint16));
}


bool c_dev_cmd::dev_wr_regenerate_auto_param_set()
{
	return send_and_wait(CMD_PUT_DAT, NUM_WR_AUTO_REGENERATE_SET_CMD, nullptr, 0);
}







bool c_dev_cmd::dev_put_clear_buff()
{
	return send_and_wait(CMD_PUT_DAT, NUM_REQ_CLEAR_BUFF, nullptr, 0);
}

bool c_dev_cmd::dev_put_clr_coord()
{
	return send_and_wait(CMD_PUT_DAT, NUM_CLR_COORD, nullptr, 0);
}





bool c_dev_cmd::dev_put_req_alt(alt_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_ALT_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}

bool c_dev_cmd::dev_get_alt(alt_dat_req_t *odat)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_ALT_DAT, reinterpret_cast<quint8 *>(odat));
}

bool c_dev_cmd::dev_put_alt(alt_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_ALT_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}

bool c_dev_cmd::dev_put_req_act(alt_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_ACT_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}

bool c_dev_cmd::dev_get_act(alt_dat_req_t *odat)
{
	return send_and_wait(CMD_REQ_DAT, NUM_REQ_ACT_DAT, reinterpret_cast<quint8 *>(odat));
}

bool c_dev_cmd::dev_put_act(alt_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_ACT_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}

bool c_dev_cmd::dev_put_pspi(alt_dat_req_t *ireq)
{
	return send_and_wait(CMD_PUT_DAT, NUM_SEND_PSPI_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}

bool c_dev_cmd::dev_put_contr(alt_dat_req_t *ireq)
{
	quint8 send_dat[3];
	send_dat[0]=ireq->nbytes;
	send_dat[1]=ireq->data[0]&0xff;
	send_dat[2]=(ireq->data[0]>>8)&0xff;

	return send_and_wait(CMD_PUT_DAT, ireq->addr&0xff, send_dat, 3);
}

bool c_dev_cmd::dev_put_vdp_speed(quint16 speed)
{
//	return send_and_wait(CMD_PUT_DAT, CMD_CONTR_VDP, reinterpret_cast<quint8 *>(&speed), 2);
	return true;
}


bool c_dev_cmd::dev_put_mab_ind_states(bool red_ind, bool green_ind)
{
	quint8 state_flags = (green_ind ? 0 : (1<<0)) | (red_ind ? 0 : (1<<1));
	return send_and_wait(CMD_PUT_DAT, NUM_CONTR_SET_LED_MP, &state_flags, 1);
	return true;
}

bool c_dev_cmd::dev_put_front_ind(quint8 ind_flags)
{
	return send_and_wait(CMD_PUT_DAT, NUM_CONTR_SET_LED, &ind_flags, 1);
	return true;
}