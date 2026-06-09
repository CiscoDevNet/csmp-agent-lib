LFLAGS += -Wl,--define=SBL_ENABLE


# Config-specific post-build commands
.PHONY: postbuild
postbuild: $(NAME).hex
	$(TICLANG_ARMCOMPILER)/bin/tiarmobjcopy $(NAME).out --output-target binary $(NAME)-noheader.bin
	$(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/tools/common/mcuboot/imgtool sign --header-size 0x80 --align 4 --slot-size 0xA8000 --pad --version 2.0.0 --pad-header --key $(SIMPLELINK_CC13XX_CC26XX_SDK_INSTALL_DIR)/source/third_party/mcuboot/root-ec-p256.pem $(NAME)-noheader.bin $(NAME).bin;
