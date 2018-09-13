################################################################################
#
# liblivemedia
#
################################################################################

LIBLIVEMEDIA_VERSION = latest
LIBLIVEMEDIA_SOURCE = live555-latest.tar.gz
LIBLIVEMEDIA_SITE = http://www.live555.com/liveMedia/public
LIBLIVEMEDIA_DEPENDENCIES =
LIBLIVEMEDIA_CONF_OPTS =

LIBLIVEMEDIA_CONFIGURE_CMDS = echo $(@D) && cd $(@D) && ./genMakefiles linux

define LIBLIVEMEDIA_BUILD_CMDS
	$(MAKE) CPLUSPLUS_COMPILER=$(TARGET_CXX) C_COMPILER=$(TARGET_CC) LINK='$(TARGET_CXX) -o' CPPFLAGS=-DALLOW_RTSP_SERVER_PORT_REUSE=1 INCLUDES='-Iinclude -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include -I../BasicUsageEnvironment/include -DLOCALE_NOT_USED' -C $(@D) all
endef

define LIBLIVEMEDIA_INSTALL_TARGET_CMDS
	DESTDIR=$(TARGET_DIR) $(MAKE) -C $(@D) install
endef

$(eval $(generic-package))
