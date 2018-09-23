/*
 * Copyright 2012 Cisco Systems, Inc. and/or its affiliates. All rights reserved.
 * Author: Konke Radlow <koradlow@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335  USA
 */

#ifndef __LIBV4L2RDS
#define __LIBV4L2RDS


#include <stdbool.h>
#include <stdint.h>

#include <linux/videodev2.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if HAVE_VISIBILITY
#define LIBV4L_PUBLIC __attribute__ ((visibility("default")))
#else
#define LIBV4L_PUBLIC
#endif

/* used to define the current version (version field) of the v4l2_rds struct */
#define V4L2_RDS_VERSION (2)

/* Constants used to define the size of arrays used to store RDS information */
#define MAX_ODA_CNT 18 	/* there are 16 groups each with type a or b. Of these
			 * 32 distinct groups, 18 can be used for ODA purposes */
#define MAX_AF_CNT 25	/* AF Method A allows a maximum of 25 AFs to be defined
			 * AF Method B does not impose a limit on the number of AFs
			 * but it is not fully supported at the moment and will
			 * not receive more than 25 AFs */
#define MAX_TMC_ADDITIONAL 28	/* 28 is the maximal possible number of fields.
			* Additional data is limited to 112 bit, and the smallest
			* optional tuple has a size of 4 bit (4 bit identifier +
			* 0 bits of data) */
#define MAX_TMC_ALT_STATIONS 32 /* defined by ISO 14819-1:2003, 7.5.3.3  */
#define MAX_TMC_AF_CNT 4	/* limit for the numbers of AFs stored per alternative TMC
			* station. This value is not defined by the standard, but based on observation
			* of real-world RDS-TMC streams. The maximum encountered number of AFs per
			* station during testing was 2 */
#define MAX_EON_CNT 20	/* Maximal number of entries in the EON table (for storing
			* information about other radio stations, broadcasted by the current station). 
			* This value is not defined by the standard, but based on observation
			* of real-world RDS-TMC streams. EON doesn't seem to be a widely used feature
			* and the maximum number of EON encountered during testing was 8 */

/* Define Constants for the possible types of RDS information
 * used to address the relevant bit in the valid_fields bitmask */
#define V4L2_RDS_PI 		0x01	/* Program Identification */
#define V4L2_RDS_PTY		0x02	/* Program Type */
#define V4L2_RDS_TP		0x04	/* Traffic Program */
#define V4L2_RDS_PS		0x08	/* Program Service Name */
#define V4L2_RDS_TA		0x10	/* Traffic Announcement */
#define V4L2_RDS_DI		0x20	/* Decoder Information */
#define V4L2_RDS_MS		0x40	/* Music / Speech flag */
#define V4L2_RDS_PTYN		0x80	/* Program Type Name */
#define V4L2_RDS_RT		0x100 	/* Radio-Text */
#define V4L2_RDS_TIME		0x200	/* Date and Time information */
#define V4L2_RDS_TMC		0x400	/* TMC availability */
#define V4L2_RDS_AF		0x800	/* AF (alternative freq) available */
#define V4L2_RDS_ECC		0x1000	/* Extended County Code */
#define V4L2_RDS_LC		0x2000	/* Language Code */
#define V4L2_RDS_TMC_SG		0x4000	/* RDS-TMC single group */
#define V4L2_RDS_TMC_MG		0x8000	/* RDS-TMC multi group */
#define V4L2_RDS_TMC_SYS	0x10000	/* RDS-TMC system information */
#define V4L2_RDS_EON		0x20000	/* Enhanced Other Network Info */
#define V4L2_RDS_LSF		0x40000	/* Linkage information */
#define V4L2_RDS_TMC_TUNING	0x80000	/* RDS-TMC tuning information */

/* Define Constants for the state of the RDS decoding process
 * used to address the relevant bit in the decode_information bitmask */
#define V4L2_RDS_GROUP_NEW 	0x01	/* New group received */
#define V4L2_RDS_ODA		0x02	/* Open Data Group announced */

/* Decoder Information (DI) codes
 * used to decode the DI information according to the RDS standard */
#define V4L2_RDS_FLAG_STEREO 		0x01
#define V4L2_RDS_FLAG_ARTIFICIAL_HEAD	0x02
#define V4L2_RDS_FLAG_COMPRESSED	0x04
#define V4L2_RDS_FLAG_STATIC_PTY	0x08

/* TMC related codes
 * used to extract TMC fields from RDS-TMC groups
 * see ISO 14819-1:2003, Figure 2 - RDS-TMC single-grp full message structure */
#define V4L2_TMC_TUNING_INFO	0x10	/* Bit 4 indicates Tuning Info / User msg */
#define V4L2_TMC_SINGLE_GROUP	0x08	/* Bit 3 indicates Single / Multi-group msg */

/* struct to encapsulate one complete RDS group */
/* This structure is used internally to store data until a complete RDS
 * group was received and group id dependent decoding can be done.
 * It is also used to provide external access to uninterpreted RDS groups
 * when manual decoding is required (e.g. special ODA types) */
struct v4l2_rds_group {
	uint16_t pi;		/* Program Identification */
	char group_version;	/* group version ('A' / 'B') */
	uint8_t group_id;	/* group number (0..16) */

	/* uninterpreted data blocks for decoding (e.g. ODA) */
	uint8_t data_b_lsb;
	uint8_t data_c_msb;
	uint8_t data_c_lsb;
	uint8_t data_d_msb;
	uint8_t data_d_lsb;
};

/* struct to encapsulate some statistical information about the decoding process */
struct v4l2_rds_statistics {
	uint32_t block_cnt;		/* total amount of received blocks */
	uint32_t group_cnt;		/* total amount of successfully
					 * decoded groups */
	uint32_t block_error_cnt;	/* blocks that were marked as erroneous
					 * and had to be dropped */
	uint32_t group_error_cnt;	/* group decoding processes that had to be
					 * aborted because of erroneous blocks
					 * or wrong order of blocks */
	uint32_t block_corrected_cnt;	/* blocks that contained 1-bit errors
					 * which were corrected */
	uint32_t group_type_cnt[16];	/* number of occurrence for each
					 * defined RDS group */
};

/* struct to encapsulate the definition of one ODA (Open Data Application) type */
struct v4l2_rds_oda {
	uint8_t group_id;	/* RDS group used to broadcast this ODA */
	char group_version;	/* group version (A / B) for this ODA */
	uint16_t aid;		/* Application Identification for this ODA,
				 * AIDs are centrally administered by the
				 * RDS Registration Office (rds.org.uk) */
};

/* struct to encapsulate an array of all defined ODA types for a channel */
/* This structure will grow with ODA announcements broadcasted in type 3A
 * groups, that were verified not to be no duplicates or redefinitions */
struct v4l2_rds_oda_set {
	uint8_t size;		/* number of ODAs defined by this channel */
	struct v4l2_rds_oda oda[MAX_ODA_CNT];
};

/* struct to encapsulate an array of Alternative Frequencies for a channel */
/* Every channel can send out AFs for his program. The number of AFs that
 * will be broadcasted is announced by the channel */
struct v4l2_rds_af_set {
	uint8_t size;			/* size of the set (might be smaller
					 * than the announced size) */
	uint8_t announced_af;		/* number of announced AF */
	uint32_t af[MAX_AF_CNT];	/* AFs defined in Hz */
};

/* struct to encapsulate one entry in the EON table (Enhanced Other Network) */
struct v4l2_rds_eon {
	uint32_t valid_fields;
	uint16_t pi;
	uint8_t ps[9];
	uint8_t pty;
	bool ta;
	bool tp;
	uint16_t lsf;		/* Linkage Set Number */
	struct v4l2_rds_af_set af;
};

/* struct to encapsulate a table of EON information */
struct v4l2_rds_eon_set {
	uint8_t size;		/* size of the table */
	uint8_t index;		/* current position in the table */
	struct v4l2_rds_eon eon[MAX_EON_CNT];	/* Information about other
						 * radio channels */
};

/* struct to encapsulate alternative frequencies (AFs) for RDS-TMC stations.
 * AFs listed in af[] can be used unconditionally. 
 * AFs listed in mapped_af[n] should only be used if the current 
 * tuner frequency matches the value in mapped_af_tuning[n] */
struct v4l2_tmc_alt_freq {
	uint8_t af_size;		/* number of known AFs */
	uint8_t af_index;
	uint8_t mapped_af_size;		/* number of mapped AFs */
	uint8_t mapped_af_index;
	uint32_t af[MAX_TMC_AF_CNT];		/* AFs defined in Hz */
	uint32_t mapped_af[MAX_TMC_AF_CNT];		/* mapped AFs defined in Hz */
	uint32_t mapped_af_tuning[MAX_TMC_AF_CNT];	/* mapped AFs defined in Hz */
};

/* struct to encapsulate information about stations carrying RDS-TMC services */
struct v4l2_tmc_station {
	uint16_t pi;
	uint8_t ltn;	/* database-ID of ON */
	uint8_t msg;	/* msg parameters of ON */
	uint8_t sid;	/* service-ID of ON */
	struct v4l2_tmc_alt_freq afi;
};

/* struct to encapsulate tuning information for TMC */
struct v4l2_tmc_tuning {
	uint8_t station_cnt;	/* number of announced alternative stations */
	uint8_t index;
	struct v4l2_tmc_station station[MAX_TMC_ALT_STATIONS];	/* information
							* about other stations carrying the same RDS-TMC service */
};

/* struct to encapsulate an additional data field in a TMC message */
struct v4l2_tmc_additional {
	uint8_t label;
	uint16_t data;
};

/* struct to encapsulate an arbitrary number of additional data fields
 * belonging to one TMC message */
struct v4l2_tmc_additional_set {
	uint8_t size;
	struct v4l2_tmc_additional fields[MAX_TMC_ADDITIONAL];
};

/* struct to encapsulate a decoded TMC message with optional additional
 * data field (in case of a multi-group TMC message) */
struct v4l2_rds_tmc_msg {
	uint8_t length;	/* length of multi-group message (0..4) */
	uint8_t sid;		/* service identifier at time of reception */
	uint8_t extent;
	uint8_t dp;		/* duration and persistence */
	uint16_t event;		/* TMC event code */
	uint16_t location;	/* TMC event location */
	bool follow_diversion;	/* indicates if the driver is adviced to
				 * follow the diversion */
	bool neg_direction;	/* indicates negative / positive direction */

	/* decoded additional information (only available in multi-group
	 * messages) */
	struct v4l2_tmc_additional_set additional;
};

/* struct to encapsulate all TMC related information, including TMC System
 * Information, TMC Tuning information and a buffer for the last decoded
 * TMC messages */
struct v4l2_rds_tmc {
	uint8_t ltn;		/* location_table_number */
	bool afi;		/* alternative frequency indicator */
	bool enhanced_mode;	/* mode of transmission,
				 * if false -> basic => gaps between tmc groups
				 * gap defines timing behavior
				 * if true -> enhanced => t_a, t_w and t_d
				 * define timing behavior of tmc groups */
	uint8_t mgs;		/* message geographical scope */
	uint8_t sid;		/* service identifier (unique ID on national level) */
	uint8_t gap;		/* Gap parameters */
	uint8_t t_a;		/* activity time (only if mode = enhanced) */
	uint8_t t_w;		/* window time (only if mode = enhanced */
	uint8_t t_d;		/* delay time (only if mode = enhanced */
	uint8_t spn[9];		/* service provider name */
	struct v4l2_rds_tmc_msg tmc_msg;

	/* tuning information for alternative service providers */
	struct v4l2_tmc_tuning tuning;
};

/* struct to encapsulate state and RDS information for current decoding process */
/* This is the structure that will be used by external applications, to
 * communicate with the library and get access to RDS data */
struct v4l2_rds {
	/** state information **/
	uint32_t decode_information;	/* state of decoding process */
	uint32_t valid_fields;		/* currently valid info fields
					 * of this structure */

	/** RDS info fields **/
	bool is_rbds; 		/* use RBDS standard version of LUTs */
	uint16_t pi;		/* Program Identification */
	uint8_t ps[9];		/* Program Service Name, UTF-8 encoding,
				 * '\0' terminated */
	uint8_t pty;		/* Program Type */
	uint8_t ptyn[9];	/* Program Type Name, UTF-8 encoding,
				 * '\0' terminated */
	bool ptyn_ab_flag;	/* PTYN A/B flag (toggled), to signal
				 * change of PTYN */
	uint8_t rt_length;	/* length of RT string */
	uint8_t rt[65];		/* Radio-Text string, UTF-8 encoding,
				 * '\0' terminated */
	bool rt_ab_flag;	/* RT A/B flag (toggled), to signal
				 * transmission of new RT */
	bool ta;		/* Traffic Announcement */
	bool tp;		/* Traffic Program */
	bool ms;		/* Music / Speech flag */
	uint8_t di;		/* Decoder Information */
	uint8_t ecc;		/* Extended Country Code */
	uint8_t lc;		/* Language Code */
	time_t time;		/* local time and date of transmission */

	struct v4l2_rds_statistics rds_statistics;
	struct v4l2_rds_oda_set rds_oda;	/* Open Data Services */
	struct v4l2_rds_af_set rds_af; 		/* Alternative Frequencies */
	struct v4l2_rds_eon_set rds_eon;	/* EON information */
	struct v4l2_rds_tmc tmc;		/* TMC information */
};

/* v4l2_rds_init() - initializes a new decoding process
 * @is_rbds:	defines which standard is used: true=RBDS, false=RDS
 *
 * initialize a new instance of the RDS-decoding struct and return
 * a handle containing state and RDS information, used to interact
 * with the library functions */
LIBV4L_PUBLIC struct v4l2_rds *v4l2_rds_create(bool is_rbds);

/* frees all memory allocated for the RDS-decoding struct */
LIBV4L_PUBLIC void v4l2_rds_destroy(struct v4l2_rds *handle);

/* resets the RDS information in the handle to initial values
 * e.g. can be used when radio channel is changed
 * @reset_statistics:	true = set all statistic values to 0, false = keep them untouched */
LIBV4L_PUBLIC void v4l2_rds_reset(struct v4l2_rds *handle, bool reset_statistics);

/* adds a raw RDS block to decode it into RDS groups
 * @return:	bitmask with with updated fields set to 1
 * @rds_data: 	3 bytes of raw RDS data, obtained by calling read()
 * 				on RDS capable V4L2 devices */
LIBV4L_PUBLIC uint32_t v4l2_rds_add(struct v4l2_rds *handle, struct v4l2_rds_data *rds_data);

/*
 * group of functions to translate numerical RDS data into strings
 *
 * return program description string defined in the RDS/RBDS Standard
 * ! return value depends on selected Standard !*/
LIBV4L_PUBLIC const char *v4l2_rds_get_pty_str(const struct v4l2_rds *handle);
LIBV4L_PUBLIC const char *v4l2_rds_get_language_str(const struct v4l2_rds *handle);
LIBV4L_PUBLIC const char *v4l2_rds_get_country_str(const struct v4l2_rds *handle);
LIBV4L_PUBLIC const char *v4l2_rds_get_coverage_str(const struct v4l2_rds *handle);

/* returns a pointer to the last decoded RDS group, in order to give raw
 * access to RDS data if it is required (e.g. ODA decoding) */
LIBV4L_PUBLIC const struct v4l2_rds_group *v4l2_rds_get_group
	(const struct v4l2_rds *handle);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
