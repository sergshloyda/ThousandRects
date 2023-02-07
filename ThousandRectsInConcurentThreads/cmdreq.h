#ifndef CMDREQ_H
#define CMDREQ_H



#define MAX_FRAME_LEN			10240


#define CMD_PUT_DAT  				0x3
#define CMD_REQ_DAT  				0x4


#define CMD_SUBM_PUT				0x85
#define CMD_SUBM_REQ	  			0x5




struct sent_dat_t
{
	quint8 type;
	quint8 cmd;
	quint16 len;
	quint8 buff[MAX_FRAME_LEN - 2*sizeof(quint8) - sizeof(quint16)];

	quint16 GetFullLength() const
	{
		if(type == CMD_REQ_DAT)
			return 2*sizeof(quint8) + sizeof(quint16) + sizeof(quint16);

		return 2*sizeof(quint8) + sizeof(quint16) + len + sizeof(quint16);
	}
};


struct resv_dat_t
{
	quint8 type;
	quint8 cmd;
	quint16 len;
	quint8 buff[MAX_FRAME_LEN];
};



#endif // CMDREQ_H
