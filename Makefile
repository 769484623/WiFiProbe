include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=Scaner
PKG_VERSION:=1.1
PKG_RELEASE:=1
PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Scaner
	DEPENDS:=+libpcap +libpthread
	MAINTAINER:=LNStar
endef

define Package/Scaner/description
	Scaner
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/Scaner/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Scaner $(1)/bin/
endef
$(eval $(call BuildPackage,Scaner))  
