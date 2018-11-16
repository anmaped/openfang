################################################################################
#
# r4l2rtspserver
#
################################################################################

V4L2RTSPSERVER_VERSION = v0.0.8
V4L2RTSPSERVER_SOURCE = v4l2rtspserver-$(V4L2RTSPSERVER_VERSION).tar.gz
V4L2RTSPSERVER_SITE = https://raw.githubusercontent.com/anmaped/openfang/master
#V4L2RTSPSERVER_GIT_SUBMODULES = YES
V4L2RTSPSERVER_DEPENDENCIES =
V4L2RTSPSERVER_CONF_OPTS =

$(eval $(cmake-package))
