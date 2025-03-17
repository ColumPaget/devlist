AC_INIT([devlist],[1.0])

AC_PROG_CC
AC_LANG([C])
AC_PROG_MAKE_SET
AC_PROG_EGREP

AC_SYS_LARGEFILE

AC_ARG_WITH(libuseful, [  --with-libuseful		use system-wide libUseful (default=auto) ], cf_system_libuseful=$withval )

dnl These libraries are required by libuseful. We only really need them if static linking, but
dnl let's be lazy and just include them all anyway
AC_CHECK_LIB(z,deflate,,)
AC_CHECK_LIB(cap,cap_init,,)

dnl New versions of openssl seem to lack EVP_CIPHER_CTX_init, so let's look for another function. I've no idea what's going on with openssl, this is why I use libressl instead.
if test "$cf_have_libcrypto" != "yes"
then
  AC_CHECK_LIB(crypto,EVP_CIPHER_CTX_new,,)
  cf_have_libcrypto=$ac_cv_lib_crypto_EVP_CIPHER_CTX_new
fi


AC_CHECK_LIB(ssl,SSL_library_init,,)
cf_have_libssl=$ac_cv_lib_ssl_SSL_library_init

dnl New versions of openssl seem to lack SSL_library_init, so let's look for another function. I've no idea what's going on with openssl, this is why I use libressl instead.
if test "$cf_have_libssl" != "yes"
then
  AC_CHECK_LIB(ssl,OPENSSL_init_ssl,,)
  cf_have_libssl=$ac_cv_lib_ssl_OPENSSL_init_ssl
fi


if test "$cf_system_libuseful" != "no"
then
	AC_CHECK_LIB([Useful-5],GetToken,[cf_have_libUseful5=yes],[cf_have_libUseful5=no])
	AC_CHECK_LIB([Useful-5],TerminalReadPromptWithHistory,AC_DEFINE([HAVE_PROMPT_HISTORY],"Y"))
	AC_CHECK_HEADERS([libUseful-5/libUseful.h])
fi

if test "$cf_have_libUseful5" = "yes" 
then
  echo "installed libUseful-5 found, using that instead of bundled libUseful."
  echo "if you want to use bundled libUseful, force it with --with-libuseful=no"
  AC_SUBST(LIBUSEFUL_BUNDLED,"")
  dnl Default behavior of AC_CHECK_LIB is to add the library to the LIBS variable
  dnl previous AC_CHECK_LIB don't do this, because an action is supplied for them
  AC_CHECK_LIB([Useful-5],GetToken)
  AC_CHECK_LIB([Useful-5],AppendVar)
else
  AC_SUBST(LIBUSEFUL_BUNDLED,"libUseful-bundled/libUseful.a")
  AC_CONFIG_SUBDIRS([libUseful-bundled])
  AC_DEFINE([USE_LIBUSEFUL_BUNDLED])
fi


dnl read Makefile.in and write Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

