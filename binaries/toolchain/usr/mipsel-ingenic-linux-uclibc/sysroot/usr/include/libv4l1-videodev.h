/* libv4l1 linux/videodev.h replacement file */
#ifndef __LINUX_VIDEODEV_H
#define __LINUX_VIDEODEV_H

#ifdef linux
#include <linux/ioctl.h>
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#include <sys/ioctl.h>
#endif

#include <stdint.h>

#define VID_TYPE_CAPTURE	1	/* Can capture */
#define VID_TYPE_TUNER		2	/* Can tune */
#define VID_TYPE_TELETEXT	4	/* Does teletext */
#define VID_TYPE_OVERLAY	8	/* Overlay onto frame buffer */
#define VID_TYPE_CHROMAKEY	16	/* Overlay by chromakey */
#define VID_TYPE_CLIPPING	32	/* Can clip */
#define VID_TYPE_FRAMERAM	64	/* Uses the frame buffer memory */
#define VID_TYPE_SCALES		128	/* Scalable */
#define VID_TYPE_MONOCHROME	256	/* Monochrome only */
#define VID_TYPE_SUBCAPTURE	512	/* Can capture subareas of the image */
#define VID_TYPE_MPEG_DECODER	1024	/* Can decode MPEG streams */
#define VID_TYPE_MPEG_ENCODER	2048	/* Can encode MPEG streams */
#define VID_TYPE_MJPEG_DECODER	4096	/* Can decode MJPEG streams */
#define VID_TYPE_MJPEG_ENCODER	8192	/* Can encode MJPEG streams */

struct video_capability
{
	char name[32];
	int type;
	int channels;	/* Num channels */
	int audios;	/* Num audio devices */
	int maxwidth;	/* Supported width */
	int maxheight;	/* And height */
	int minwidth;	/* Supported width */
	int minheight;	/* And height */
};


struct video_channel
{
	int channel;
	char name[32];
	int tuners;
	uint32_t flags;
#define VIDEO_VC_TUNER		1	/* Channel has a tuner */
#define VIDEO_VC_AUDIO		2	/* Channel has audio */
	uint16_t type;
#define VIDEO_TYPE_TV		1
#define VIDEO_TYPE_CAMERA	2
	uint16_t norm;			/* Norm set by channel */
};

struct video_tuner
{
	int tuner;
	char name[32];
	unsigned long rangelow, rangehigh;	/* Tuner range */
	uint32_t flags;
#define VIDEO_TUNER_PAL		1
#define VIDEO_TUNER_NTSC	2
#define VIDEO_TUNER_SECAM	4
#define VIDEO_TUNER_LOW		8	/* Uses KHz not MHz */
#define VIDEO_TUNER_NORM	16	/* Tuner can set norm */
#define VIDEO_TUNER_STEREO_ON	128	/* Tuner is seeing stereo */
#define VIDEO_TUNER_RDS_ON      256     /* Tuner is seeing an RDS datastream */
#define VIDEO_TUNER_MBS_ON      512     /* Tuner is seeing an MBS datastream */
	uint16_t mode;			/* PAL/NTSC/SECAM/OTHER */
#define VIDEO_MODE_PAL		0
#define VIDEO_MODE_NTSC		1
#define VIDEO_MODE_SECAM	2
#define VIDEO_MODE_AUTO		3
	uint16_t signal;			/* Signal strength 16bit scale */
};

struct video_picture
{
	uint16_t	brightness;
	uint16_t	hue;
	uint16_t	colour;
	uint16_t	contrast;
	uint16_t	whiteness;	/* Black and white only */
	uint16_t	depth;		/* Capture depth */
	uint16_t	palette;	/* Palette in use */
#define VIDEO_PALETTE_GREY	1	/* Linear greyscale */
#define VIDEO_PALETTE_HI240	2	/* High 240 cube (BT848) */
#define VIDEO_PALETTE_RGB565	3	/* 565 16 bit RGB */
#define VIDEO_PALETTE_RGB24	4	/* 24bit RGB */
#define VIDEO_PALETTE_RGB32	5	/* 32bit RGB */
#define VIDEO_PALETTE_RGB555	6	/* 555 15bit RGB */
#define VIDEO_PALETTE_YUV422	7	/* YUV422 capture */
#define VIDEO_PALETTE_YUYV	8
#define VIDEO_PALETTE_UYVY	9	/* The great thing about standards is ... */
#define VIDEO_PALETTE_YUV420	10
#define VIDEO_PALETTE_YUV411	11	/* YUV411 capture */
#define VIDEO_PALETTE_RAW	12	/* RAW capture (BT848) */
#define VIDEO_PALETTE_YUV422P	13	/* YUV 4:2:2 Planar */
#define VIDEO_PALETTE_YUV411P	14	/* YUV 4:1:1 Planar */
#define VIDEO_PALETTE_YUV420P	15	/* YUV 4:2:0 Planar */
#define VIDEO_PALETTE_YUV410P	16	/* YUV 4:1:0 Planar */
#define VIDEO_PALETTE_PLANAR	13	/* start of planar entries */
#define VIDEO_PALETTE_COMPONENT 7	/* start of component entries */
};

struct video_audio
{
	int		audio;		/* Audio channel */
	uint16_t	volume;		/* If settable */
	uint16_t	bass, treble;
	uint32_t	flags;
#define VIDEO_AUDIO_MUTE	1
#define VIDEO_AUDIO_MUTABLE	2
#define VIDEO_AUDIO_VOLUME	4
#define VIDEO_AUDIO_BASS	8
#define VIDEO_AUDIO_TREBLE	16
#define VIDEO_AUDIO_BALANCE	32
	char		name[16];
#define VIDEO_SOUND_MONO	1
#define VIDEO_SOUND_STEREO	2
#define VIDEO_SOUND_LANG1	4
#define VIDEO_SOUND_LANG2	8
	uint16_t	mode;
	uint16_t	balance;	/* Stereo balance */
	uint16_t	step;		/* Step actual volume uses */
};

struct video_clip
{
	int32_t	x,y;
	int32_t	width, height;
	struct video_clip *next;	/* For user use/driver use only */
};

struct video_window
{
	uint32_t	x,y;			/* Position of window */
	uint32_t	width,height;		/* Its size */
	uint32_t	chromakey;
	uint32_t	flags;
	struct		video_clip *clips;	/* Set only */
	int		clipcount;
#define VIDEO_WINDOW_INTERLACE	1
#define VIDEO_WINDOW_CHROMAKEY	16		/* Overlay by chromakey */
#define VIDEO_CLIP_BITMAP	-1
/* bitmap is 1024x625, a '1' bit represents a clipped pixel */
#define VIDEO_CLIPMAP_SIZE	(128 * 625)
};

struct video_buffer
{
	void	*base;
	int	height,width;
	int	depth;
	int	bytesperline;
};

struct video_mmap
{
	unsigned	int frame;	/* Frame (0 - n) for double buffer */
	int		height,width;
	unsigned	int format;	/* should be VIDEO_PALETTE_* */
};

struct video_mbuf
{
	int	size;		/* Total memory to map */
	int	frames;		/* Frames */
	int	offsets[32];
};

struct vbi_format {
	uint32_t	sampling_rate;	/* in Hz */
	uint32_t	samples_per_line;
	uint32_t	sample_format;	/* VIDEO_PALETTE_RAW only (1 byte) */
	int32_t		start[2];	/* starting line for each frame */
	uint32_t	count[2];	/* count of lines for each frame */
	uint32_t	flags;
#define	VBI_UNSYNC	1	/* can distingues between top/bottom field */
#define	VBI_INTERLACED	2	/* lines are interlaced */
};

#define VIDIOCGCAP		_IOR('v',1,struct video_capability)	/* Get capabilities */
#define VIDIOCGCHAN		_IOWR('v',2,struct video_channel)	/* Get channel info (sources) */
#define VIDIOCSCHAN		_IOW('v',3,struct video_channel)	/* Set channel 	*/
#define VIDIOCGTUNER		_IOWR('v',4,struct video_tuner)		/* Get tuner abilities */
#define VIDIOCSTUNER		_IOW('v',5,struct video_tuner)		/* Tune the tuner for the current channel */
#define VIDIOCGPICT		_IOR('v',6,struct video_picture)	/* Get picture properties */
#define VIDIOCSPICT		_IOW('v',7,struct video_picture)	/* Set picture properties */
#define VIDIOCCAPTURE		_IOW('v',8,int)				/* Start, end capture */
#define VIDIOCGWIN		_IOR('v',9, struct video_window)	/* Get the video overlay window */
#define VIDIOCSWIN		_IOW('v',10, struct video_window)	/* Set the video overlay window - passes clip list for hardware smarts , chromakey etc */
#define VIDIOCGFBUF		_IOR('v',11, struct video_buffer)	/* Get frame buffer */
#define VIDIOCSFBUF		_IOW('v',12, struct video_buffer)	/* Set frame buffer - root only */
#define VIDIOCGFREQ		_IOR('v',14, unsigned long)		/* Set tuner */
#define VIDIOCSFREQ		_IOW('v',15, unsigned long)		/* Set tuner */
#define VIDIOCGAUDIO		_IOR('v',16, struct video_audio)	/* Get audio info */
#define VIDIOCSAUDIO		_IOW('v',17, struct video_audio)	/* Audio source, mute etc */
#define VIDIOCSYNC		_IOW('v',18, int)			/* Sync with mmap grabbing */
#define VIDIOCMCAPTURE		_IOW('v',19, struct video_mmap)		/* Grab frames */
#define VIDIOCGMBUF		_IOR('v',20, struct video_mbuf)		/* Memory map buffer info */
#define	VIDIOCGVBIFMT		_IOR('v',28, struct vbi_format)		/* Get VBI information */
#define	VIDIOCSVBIFMT		_IOW('v',29, struct vbi_format)		/* Set VBI information */

#endif
