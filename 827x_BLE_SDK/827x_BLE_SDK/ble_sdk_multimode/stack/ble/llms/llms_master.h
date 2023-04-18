/*
 * llms_master.h
 *
 *  Created on: 2019-5-24
 *      Author: Administrator
 */

#ifndef LLMS_MASTER_H_
#define LLMS_MASTER_H_



/*********************************************************************************
 *   17		  19	  23	 29		  31	 37		 39       41
 * 21.25	23.75	28.75	36.25	38.75	46.25	48.75    51.25
 *					  X				  				 X
 *					  		 V				 V				  V
 *
 *					 23		 29		 31		 37			  	  41
 *
 *					     25
 *					   31.25 mS
 ********************************************************************************/
#define 		MASTER_CONN_INTER_23							23
#define 		MASTER_CONN_INTER_25							25	// 31.25 mS
#define 		MASTER_CONN_INTER_29							29
#define 		MASTER_CONN_INTER_31							31
#define 		MASTER_CONN_INTER_37							37
#define 		MASTER_CONN_INTER_41							41







typedef struct {


} st_llms_m_conn_t;




/************************************ User Interface  ******************************************************/




/*********************************** Stack Interface, user can not use!!! **********************************/
int 	blms_m_connect (rf_packet_connect_t * pInit);
int 	blms_btx_start (void);
int 	blms_btx_post (void);



#endif /* LLMS_MASTER_H_ */
