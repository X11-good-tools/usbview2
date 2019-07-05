## Process this file with automake to produce Makefile.in

# -- @FIXME: We need to move the header files for the XML parsing to a correct place
# -- and update the include directories (-I)
INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-I$(PWD)

bin_PROGRAMS = usbview2

usbview2_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	usbparse.c usbparse.h \
	usbtree.c usbtree.h \
	lsusb.c lsusb.h \
	xml/parser.c xml/parser.h

usbview2_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

AM_CFLAGS = -DUSBXML_DIR='"$(pkgdatadir)"' @PACKAGE_CFLAGS@

# -- Install the XML devices file
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	if test -d $(DESTDIR)$(pkgdatadir); then \
	  $(INSTALL_DATA) $(srcdir)/xml/usb.xml $(srcdir)/xml/usb.ids \
		$(DESTDIR)$(pkgdatadir)/; \
	fi
