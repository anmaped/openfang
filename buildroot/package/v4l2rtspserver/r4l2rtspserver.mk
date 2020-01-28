################################################################################
#
# r4l2rtspserver
#
################################################################################

V4L2RTSPSERVER_VERSION = v0.0.8
V4L2RTSPSERVER_SOURCE = v4l2rtspserver-$(V4L2RTSPSERVER_VERSION).tar.gz
V4L2RTSPSERVER_SITE = $(PWD)/package/v4l2rtspserver
V4L2RTSPSERVER_SITE_METHOD = file

$(eval $(cmake-package))
