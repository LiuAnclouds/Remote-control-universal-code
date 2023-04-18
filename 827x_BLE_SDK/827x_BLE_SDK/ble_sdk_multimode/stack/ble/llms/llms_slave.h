/*
 * llms_slave.h
 *
 *  Created on: 2019-5-24
 *      Author: Administrator
 */

#ifndef LLMS_SLAVE_H_
#define LLMS_SLAVE_H_



#define			SLAVE_SLOT_FINE_TUNE_EN							1   // enable:  make sure slave sync master timing
																	// disable: slave will disconnect for long time; can use this to see BRX early set timing



#define			SLAVE_SYNC_CONN_CREATE							BIT(0)
#define			SLAVE_SYNC_CONN_UPDATE							BIT(1)
#define			SLAVE_SYNC_HIGH_DUTY							BIT(7)


#define			SLAVE_SYNC_HIGHDUTY_SUPPORT_EN					0


#define			BRX_EARLY_SET_SYNC								200
#define			BRX_EARLY_SET_COMMON							150



typedef struct {

	u32		connExpectTime;
	u32		conn_start_time;
	u32		conn_duration;

	u32		tick_1st_rx;

	u32		tick_fine_tune;

	u8	    sync_timing;
	u8		rsvd1;
	u8		rsvd2;
	u8		rsvd3;

} st_lls_conn_t;

extern	_attribute_aligned_(4)	st_lls_conn_t	blmsSlave[];
extern	st_lls_conn_t	*bls_pconn;
extern	int				bls_conn_sel;



/************************************ User Interface  ******************************************************/




/*********************************** Stack Interface, user can not use!!! **********************************/
int 	blms_s_connect (rf_packet_connect_t * pInit);
int 	blms_brx_start(void);
int 	blms_brx_post(void);




#endif /* LLMS_SLAVE_H_ */
