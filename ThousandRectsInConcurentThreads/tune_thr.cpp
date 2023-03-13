#include "tune_thr.h"



SendRsvObj::SendRsvObj(	QObject *parent,


			

				quint32 *g_req_send_dat,
				device_data_t *dev_data,


				quint32 *g_changed_param,
				bool *en_set_device_params,

				par_device_t *device_par,

				indicator_states_t *indic_states
				) :
		QObject(parent),
		on_tune_thr(false),

		dev_cmd(this),

		is_initing(false),

		send_par_counter(0),

		param_buffer(0),


		p_g_changed_param(g_changed_param),
		p_en_set_device_params(en_set_device_params),
		p_device_par(device_par),							// текущее

		p_g_req_send_dat(g_req_send_dat),
		p_dev_data(dev_data),


		m_changed_param(0),							// передаваемое
		m_device_par(),


		p_indic_states(indic_states),
	


		req_timer(this)
{
	memset(data_buff, 0, MAX_FRAME_LEN * sizeof(quint8));
	req_timer.setSingleShot(true);
	connect(&req_timer, SIGNAL(timeout()), this, SLOT(req_timer_timeout()));
}



void SendRsvObj::on_off_timer(bool on_off)
{
	if(on_off)
		req_timer.start(dev_cmd.GetRequestPeriod());
	else
		req_timer.stop();
}


SendRsvObj::~SendRsvObj()
{
}


bool SendRsvObj::send_param()
{
	param_buffer.set<quint16>(0);     // первые 2 байта(sizeof(quint16)) устанавливаем в 0
	param_buffer.add(m_changed_param);


	if(m_changed_param & CHNG_ALL)
		param_buffer.add(m_device_par);
	else
	{
		const par_osc_t &curr_osc_par = m_device_par.view_par.osc_par;
		const par_trk_t &trk_par = m_device_par.contr_par.trk_par;
		const par_us_t *curr_us = trk_par.takts[curr_osc_par.ntact].uss + curr_osc_par.nus;
		const par_strb_t *curr_strb = curr_us->strb + curr_osc_par.curr_strb;
		const par_vrch_t &curr_vrch = curr_us->vrch;

		if(m_changed_param&CHNG_CHAN)
			param_buffer.add(m_device_par.view_par.nchan);
		if(m_changed_param&CHNG_TACT)
			param_buffer.add(curr_osc_par.ntact);
		if(m_changed_param&CHNG_NUS)
		{
			param_buffer.add(curr_osc_par.nus);
			param_buffer.add(curr_osc_par.ninp);
		}
		if(m_changed_param&CHNG_ONUS)
			param_buffer.add(curr_us->on_us);

		if(m_changed_param&CHNG_KUS)
			param_buffer.add(curr_us->kus);

		if(m_changed_param&CHNG_BEGR)
			param_buffer.add(curr_osc_par.curr_beg_r);

		if(m_changed_param&CHNG_ENDR)
		{
			param_buffer.add(curr_osc_par.curr_end_r);
			param_buffer.add(curr_osc_par.curr_stp_r);
		}

		if(m_changed_param&CHNG_US_PAR)
		{
			param_buffer.add(curr_us->rej);
			param_buffer.add(curr_us->detpar);
			param_buffer.add(curr_us->taut);
			param_buffer.add(curr_us->tauv);
		}

		if(m_changed_param&CHNG_NSTRB)
			param_buffer.add(curr_osc_par.curr_strb);

		if(m_changed_param&CHNG_BSTRB)
			param_buffer.add(curr_strb->beg);

		if(m_changed_param&CHNG_LSTRB)
			param_buffer.add(curr_strb->len);

		if(m_changed_param&CHNG_POR)
			param_buffer.add(curr_strb->por);

		if(m_changed_param&CHNG_NUMPV)
		{
			param_buffer.add(curr_osc_par.curr_pvrch);
			param_buffer.add(curr_vrch.num_p);
		}

		if(m_changed_param&CHNG_VRON)
			param_buffer.add(curr_vrch.on_vrch);

		if(m_changed_param&CHNG_VTIME)
			param_buffer.add(curr_vrch.time[curr_osc_par.curr_pvrch]);

		if(m_changed_param&CHNG_VKUS)
			param_buffer.add(curr_vrch.kus[curr_osc_par.curr_pvrch]);


		const par_gen_t *curr_gen =  trk_par.takts[curr_osc_par.ntact].gens + m_device_par.view_par.ngen;

		if(m_changed_param&CHNG_NGEN)
			param_buffer.add(m_device_par.view_par.ngen);

		if(m_changed_param&CHNG_ONGEN)
			param_buffer.add(curr_gen->on_gen);

		if(m_changed_param&CHNG_LIMP)
			param_buffer.add(curr_gen->len_imp);

		if(m_changed_param&CHNG_NPOW)
			param_buffer.add(m_device_par.view_par.npow);

		if(m_changed_param&CHNG_PVAL)
			param_buffer.add(trk_par.pow_gen[m_device_par.view_par.npow].val);

		if(m_changed_param&CHNG_CHAN_LIST)
			param_buffer.add(trk_par.cn_list);

		if(m_changed_param&CHNG_REJ_OSC)
			param_buffer.add(curr_osc_par.rej_osc);

		if(m_changed_param&CHNG_B_SCAN)
		{
			param_buffer.add(curr_us->beg_r);
			param_buffer.add(curr_us->step_r);
			param_buffer.add(trk_par.cn_list.cn_info[m_device_par.view_par.nchan].a_step_count);
			param_buffer.add(trk_par.cn_list.cn_info[m_device_par.view_par.nchan].a_k_color);
		}

		if(m_changed_param&CHNG_VIEW_PAR)
			param_buffer.add(m_device_par.view_par);

    }
 

    quint16 snd_size = param_buffer.get_len();
    memcpy(param_buffer.get(), &snd_size, sizeof(quint16));

	m_changed_param = 0;

	return dev_cmd.dev_put_param(param_buffer.get(), param_buffer.get_len());
}







void SendRsvObj::req_timer_timeout()
{
	if(!on_tune_thr)
		return;

	if(is_initing)
	{
		if(apply_params())
		{
			thread()->msleep(100);

			is_initing = false;

			if(on_tune_thr)
				emit signal_connect_setted();
		}
		else if(on_tune_thr)
			emit signal_unsuccesfull();
	}
	else
	{
		bool get_result = dev_cmd.dev_get_osc( data_buff );

		if(get_result)
		{
			QByteArray osc_array(reinterpret_cast<const char *>(data_buff), sizeof(osc_struct_t));
			emit signal_draw_osc(osc_array);
		}

		if(!on_tune_thr)
			return;





		if(!on_tune_thr)
			return;


		quint16 res_len = 0;

		if(get_result)
			get_result = dev_cmd.dev_get_amps( data_buff , &res_len);			// разбор данных идёт в другом потоке...

		if(!on_tune_thr)
			return;


		if(get_result && (res_len > 0))
		{
			QByteArray ampl_array(reinterpret_cast<const char *>(data_buff), res_len);
			emit signal_collect_amps(ampl_array);										// передаются без обработки (?)
		}


		if(get_result && on_tune_thr && p_indic_states->gr_rd_changed)
		{
			get_result =  dev_cmd.dev_put_mab_ind_states(p_indic_states->red_ind, p_indic_states->green_ind);
			if(get_result)
				p_indic_states->gr_rd_changed = false;
		}

		if(get_result && on_tune_thr && p_indic_states->fr_changed)
		{
			get_result =  dev_cmd.dev_put_front_ind(p_indic_states->front_ind);
			if(get_result)
				p_indic_states->fr_changed = false;
		}



		++send_par_counter;

		if((send_par_counter % 5) == 0)
		{
			send_par_counter = 0;

			if(*p_g_changed_param && get_result && on_tune_thr)
			{
				if(*p_en_set_device_params)
					m_changed_param |=*p_g_changed_param;
				else
					m_changed_param |=*p_g_changed_param & ( CHNG_CHAN|CHNG_TACT|CHNG_NUS|CHNG_BEGR|CHNG_ENDR
															|CHNG_NSTRB|CHNG_NUMPV|CHNG_NGEN
															|CHNG_NPOW |CHNG_REJ_OSC|CHNG_ALL);
				memcpy(&m_device_par, p_device_par, sizeof(par_device_t));

				*p_g_changed_param=0;
			}

			if(m_changed_param && get_result && on_tune_thr)
			{
				get_result = send_param();
				if(get_result)
					m_changed_param=0;
			}
		}


		if(on_tune_thr && !get_result)
			emit signal_unsuccesfull();
		
	}

	if(on_tune_thr)
		req_timer.start(dev_cmd.GetRequestPeriod());
}


bool SendRsvObj::apply_params()
{
	quint8 curr_mode = 0;
	bool result = dev_cmd.dev_get_cur_rej(&curr_mode);
	quint8 setup_view_mode=((REJ_AUTO_FLAW << REJ_MAIN_OFFSET) | REJ_MENU_SETUP);	//0x31
	if(result && on_tune_thr)
	{
		if(curr_mode != setup_view_mode)
		{
			result = dev_cmd.dev_set_cur_rej(setup_view_mode);			// REJ_AUTO_FLAW  | REJ_MENU_SETUP
			if(!result && on_tune_thr)
				emit signal_wrong_mode();
		}
	}

	if(result && on_tune_thr)
	{
		memcpy(&m_device_par, p_device_par, sizeof(par_device_t));
		m_changed_param=CHNG_ALL;
		result = send_param();
		if(result && on_tune_thr)
		{
			*p_g_changed_param=0;
			m_changed_param=0;
		}
	}


	if(result && on_tune_thr)
	{
		result =  dev_cmd.dev_put_mab_ind_states(p_indic_states->red_ind, p_indic_states->green_ind);
		if(result)
			p_indic_states->gr_rd_changed = false;
	}

	if(result && on_tune_thr)
	{
		result =  dev_cmd.dev_put_front_ind(p_indic_states->front_ind);
		if(result)
			p_indic_states->fr_changed = false;
	}

	return result;
}
