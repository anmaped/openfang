################################################################################
#
# r4l2rtspserver
#
################################################################################

V4L2RTSPSERVER_VERSION = v0.0.8
V4L2RTSPSERVER_SITE = https://github.com/mpromonet/v4l2rtspserver.git
V4L2RTSPSERVER_SITE_METHOD = git
#V4L2RTSPSERVER_GIT_SUBMODULES = YES
V4L2RTSPSERVER_DEPENDENCIES =
V4L2RTSPSERVER_CONF_OPTS = -DLOCALE_NOT_USED

$(eval $(cmake-package))
