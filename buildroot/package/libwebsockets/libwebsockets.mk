################################################################################
#
# libwebsockets
#
################################################################################

LIBWEBSOCKETS_VERSION = v3.0-stable
LIBWEBSOCKETS_SOURCE = libwebsockets-$(LIBWEBSOCKETS_VERSION).tar.gz
LIBWEBSOCKETS_SITE = https://libwebsockets.org/repo/libwebsockets
LIBWEBSOCKETS_SITE_METHOD = git
LIBWEBSOCKETS_LICENSE = LGPLv2.1 with exceptions
LIBWEBSOCKETS_LICENSE_FILES = LICENSE
LIBWEBSOCKETS_DEPENDENCIES = zlib
LIBWEBSOCKETS_INSTALL_STAGING = YES

LIBWEBSOCKETS_CONF_OPT += -DWITHOUT_TESTAPPS=ON

ifeq ($(BR2_PACKAGE_OPENSSL),y)
LIBWEBSOCKETS_DEPENDENCIES += openssl
else
LIBWEBSOCKETS_CONF_OPT += -DWITH_SSL=OFF
endif

$(eval $(cmake-package))
