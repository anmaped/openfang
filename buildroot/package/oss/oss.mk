################################################################################
#
# oss cmd
#
################################################################################

OSS_VERSION = v4.2-build2017
OSS_SOURCE = oss-$(OSS_VERSION)-src-gpl.tar.bz2
OSS_SITE = http://www.4front-tech.com/developer/sources/stable/gpl/
#OSS_SITE_METHOD = git
#OSS_GIT_SUBMODULES = YES
OSS_DEPENDENCIES =
OSS_CONF_OPTS =

OSS_CONFIGURE_CMDS = mkdir $(@D)/build && cd $(@D)/build && ../configure --no-regparm --only-drv=

define OSS_BUILD_CMDS
	$(MAKE) CPLUSPLUS=$(TARGET_CXX) CC=$(TARGET_CC) LD=$(TARGET_LD) -C $(@D)/build/lib
	$(MAKE) CPLUSPLUS=$(TARGET_CXX) CC=$(TARGET_CC) LD=$(TARGET_LD) -C $(@D)/build/cmd
endef

define OSS_INSTALL_TARGET_CMDS
	ln -s ossplay $(@D)/build/target/bin/ossrecord
	cp -a $(@D)/build/target/bin/* $(TARGET_DIR)/usr/bin
	cp $(@D)/build/target/lib/* $(TARGET_DIR)/usr/lib
	cp $(@D)/build/target/sbin/* $(TARGET_DIR)/usr/sbin
endef

$(eval $(generic-package))
