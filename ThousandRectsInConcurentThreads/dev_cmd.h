#ifndef DEV_CMD_H
#define DEV_CMD_H

#include <base_dev_cmd.h>
#include "dev_interf.h"

class c_dev_cmd : public c_base_dev_cmd
{
public:

	c_dev_cmd(QObject *parent);
	virtual ~c_dev_cmd();


	bool dev_get_cur_rej(quint8 *btmp);
	bool dev_set_cur_rej(quint8 rej);

	bool dev_get_osc(quint8 *btmp);
//	bool dev_get_spectr(quint8 *btmp);

	bool dev_get_amps(quint8 *btmp, quint16 *len);
	bool dev_put_clear_buff();

	bool dev_put_param(const char *iparam, quint16 num_bytes);

	bool dev_put_req_alt(alt_dat_req_t *ireq);
	bool dev_get_alt(alt_dat_req_t *odat);
	bool dev_put_alt(alt_dat_req_t *ireq);
	bool dev_put_req_act(alt_dat_req_t *ireq);
	bool dev_get_act(alt_dat_req_t *odat);
	bool dev_put_act(alt_dat_req_t *ireq);
	bool dev_put_pspi(alt_dat_req_t *ireq);
	bool dev_put_contr(alt_dat_req_t *ireq);

	bool dev_rd_par_dev(quint8 *btmp);
	bool dev_wr_par_dev(const quint16 par);

	bool dev_wr_regenerate_auto_param_set();


	bool dev_get_version_str(quint8 *btmp);


	bool dev_put_clr_coord();
	bool dev_put_vdp_speed(quint16 speed);


	bool dev_put_mab_ind_states(bool red_ind, bool green_ind);

	bool dev_put_front_ind(quint8 ind_flags);


};

#endif // DEV_CMD_H
