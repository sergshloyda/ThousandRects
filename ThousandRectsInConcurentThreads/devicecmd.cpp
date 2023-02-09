#include "devicecmd.h"

DevCmdObr::DevCmdObr(QObject *parent)
	: QObject(parent),_pBaseCmd(nullptr),_baseDevCmd(parent)
{

}

DevCmdObr::~DevCmdObr()
{

}
bool DevCmdObr::dev_get_cur_rej(quint8 *btmp)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_REJ, btmp);

}
bool DevCmdObr::dev_set_cur_rej(quint8 rej)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SET_REJ, &rej, 1);
}
bool DevCmdObr::dev_get_osc(quint8 *btmp)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_OSC, btmp);
}
bool DevCmdObr::dev_get_amps(quint8 *btmp, quint16 *len)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_AMPS, nullptr, 0, btmp, len);
}
bool DevCmdObr::dev_put_clear_buff()
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_REQ_CLEAR_BUFF, nullptr, 0);
}
bool DevCmdObr::dev_put_param(const char *iparam, quint16 num_bytes)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_WR_N_AUTO_PAR_CMD, reinterpret_cast<const quint8 *>(iparam), num_bytes);
}
bool DevCmdObr::dev_put_req_alt(alt_dat_req_t *ireq)

{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SEND_ALT_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));;
}

bool DevCmdObr::dev_get_alt(alt_dat_req_t *odat)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_ALT_DAT, reinterpret_cast<quint8 *>(odat));
}
bool DevCmdObr::dev_put_alt(alt_dat_req_t *ireq)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SEND_ALT_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}
bool DevCmdObr::dev_put_req_act(alt_dat_req_t *ireq)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SEND_ACT_REQ, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}
bool DevCmdObr::dev_get_act(alt_dat_req_t *odat)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_ACT_DAT, reinterpret_cast<quint8 *>(odat));
}
bool DevCmdObr::dev_put_act(alt_dat_req_t *ireq)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SEND_ACT_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}
bool DevCmdObr::dev_put_pspi(alt_dat_req_t *ireq)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_SEND_PSPI_DAT, reinterpret_cast<quint8 *>(ireq), sizeof(alt_dat_req_t));
}
bool DevCmdObr::dev_put_contr(alt_dat_req_t *ireq)
{
	quint8 send_dat[3];
	send_dat[0]=ireq->nbytes;
	send_dat[1]=ireq->data[0]&0xff;
	send_dat[2]=(ireq->data[0]>>8)&0xff;

	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, ireq->addr&0xff, send_dat, 3);
}
bool DevCmdObr::dev_rd_par_dev(quint8 *btmp)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_RD_AUTO_PAR_DEV_CMD, nullptr, 0, btmp);
}
bool DevCmdObr::dev_wr_par_dev(const quint16 par)
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_WR_AUTO_PAR_DEV_CMD, reinterpret_cast<const quint8*>(&par), sizeof(quint16));
}
bool DevCmdObr::dev_wr_regenerate_auto_param_set()
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_WR_AUTO_REGENERATE_SET_CMD, nullptr, 0);
}
bool DevCmdObr::dev_get_version_str(quint8 *btmp)
{
	return _baseDevCmd.send_and_wait(CMD_REQ_DAT, NUM_REQ_GET_VERSION, btmp);
}
bool DevCmdObr::dev_put_clr_coord()
{
	return _baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_CLR_COORD, nullptr, 0);
}
bool DevCmdObr::dev_put_vdp_speed(quint16 speed)
{
	//	return send_and_wait(CMD_PUT_DAT, CMD_CONTR_VDP, reinterpret_cast<quint8 *>(&speed), 2);
	return true;
}
bool DevCmdObr::dev_put_mab_ind_states(bool red_ind, bool green_ind)
{
	quint8 state_flags = (green_ind ? 0 : (1<<0)) | (red_ind ? 0 : (1<<1));
	_baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_CONTR_SET_LED_MP, &state_flags, 1);
	return true;
}
bool DevCmdObr::dev_put_front_ind(quint8 ind_flags)
{
	_baseDevCmd.send_and_wait(CMD_PUT_DAT, NUM_CONTR_SET_LED, &ind_flags, 1);
	return true;
}

void DevCmdObr::AttachToCMD(BaseConn *base_cmd)
{
	_baseDevCmd.AttachToCMD(base_cmd);
}
void DevCmdObr::DetachFromCMD()
{
	_baseDevCmd.DetachFromCMD();
}
void DevCmdObr::ResetConnCount()
{
	_baseDevCmd.ResetConnCount();
}

int DevCmdObr::GetUnSuccesfullCount() const
{
	return _baseDevCmd.GetUnSuccesfullCount();
}
bool DevCmdObr::IsAttached() const
{
	return _baseDevCmd.IsAttached();
}
int DevCmdObr::getEstTimeCount()
{
	return _baseDevCmd.time_count;
}
int DevCmdObr::GetRequestPeriod() const
{
	return _baseDevCmd.GetRequestPeriod();
}