# Copyright (c) 2013-2016 The Fleet Credits Core developers
# Copyright (c) 2025-2026 The Fleet Credits Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

FLEETCREDITS_CONFIG_INCLUDES=-I$(builddir)/config
FLEETCREDITS_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

FLEETCREDITS_INCLUDES += -I$(srcdir)/secp256k1/include
FLEETCREDITS_INCLUDES += $(UNIVALUE_CFLAGS)

LIBFLEETCREDITS_SERVER=libfleetcredits_server.a
LIBFLEETCREDITS_COMMON=libfleetcredits_common.a
LIBFLEETCREDITS_CONSENSUS=libfleetcredits_consensus.a
LIBFLEETCREDITS_CLI=libfleetcredits_cli.a
LIBFLEETCREDITS_UTIL=libfleetcredits_util.a
LIBFLEETCREDITS_CRYPTO=crypto/libfleetcredits_crypto.a
LIBFLEETCREDITSQT=qt/libfleetcreditsqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBFLEETCREDITS_ZMQ=libfleetcredits_zmq.a
endif
if BUILD_FLEETCREDITS_LIBS
LIBFLEETCREDITSCONSENSUS=libfleetcreditsconsensus.la
endif
if ENABLE_WALLET
LIBFLEETCREDITS_WALLET=libfleetcredits_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBFLEETCREDITS_CRYPTO) \
  $(LIBFLEETCREDITS_UTIL) \
  $(LIBFLEETCREDITS_COMMON) \
  $(LIBFLEETCREDITS_CONSENSUS) \
  $(LIBFLEETCREDITS_SERVER) \
  $(LIBFLEETCREDITS_CLI) \
  $(LIBFLEETCREDITS_WALLET) \
  $(LIBFLEETCREDITS_ZMQ)

lib_LTLIBRARIES = $(LIBFLEETCREDITSCONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_FLEETCREDITS_DAEMON
  bin_PROGRAMS += fleetcreditsd
endif

if BUILD_FLEETCREDITS_UTILS
  bin_PROGRAMS += fleetcredits-cli fleetcredits-tx
endif

.PHONY: FORCE check-symbols check-security
# fleetcredits core #
FLEETCREDITS_CORE_H = \
  addrdb.h \
  addrman.h \
  attributes.h \
  auxpow.h \
  base58.h \
  bloom.h \
  blockencodings.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  fleetcredits.h \
  fleetcredits-fees.h \
  fs.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netmessagemaker.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  primitives/block.h \
  primitives/pureheader.h \
  primitives/transaction.h \
  primitives/contribution.h \
  primitives/mweb.h \
  primitives/verification.h \
  mweb_mempool.h \
  protocol.h \
  random.h \
  reverselock.h \
  rpc/auxcache.h \
  rpc/blockchain.h \
  rpc/client.h \
  rpc/mining.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/register.h \
  scheduler.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/ismine.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/experimental.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  threadinterrupt.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  txrequest.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmemory.h \
  utilmoneystr.h \
  utiltime.h \
  utilstring.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcutil.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libfleetcredits_util_a-clientversion.$(OBJEXT): obj/build.h
# Legacy alias
libfleetcredits_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between fleetcreditsd and fleetcredits-qt
libfleetcredits_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libfleetcredits_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_server_a_SOURCES = \
  addrman.cpp \
  addrdb.cpp \
  auxpow.cpp \
  bloom.cpp \
  blockencodings.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  net_processing.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  rest.cpp \
  rpc/auxcache.cpp \
  rpc/auxpow.cpp \
  rpc/blockchain.cpp \
  rpc/contribution.cpp \
  rpc/mweb.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  txrequest.cpp \
  ui_interface.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(FLEETCREDITS_CORE_H)

if ENABLE_ZMQ
libfleetcredits_zmq_a_CPPFLAGS = $(FLEETCREDITS_INCLUDES) $(ZMQ_CFLAGS)
libfleetcredits_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between fleetcreditsd and fleetcredits-qt, but only linked
# when wallet enabled
libfleetcredits_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
libfleetcredits_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcutil.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(FLEETCREDITS_CORE_H)

# crypto primitives library
crypto_libfleetcredits_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_CONFIG_INCLUDES)
crypto_libfleetcredits_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libfleetcredits_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/scrypt.cpp \
  crypto/scrypt.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# only include SSE2 scrypt sources if USE_SCRYPT_SSE2 is defined
if USE_SCRYPT_SSE2
crypto_libfleetcredits_crypto_a_SOURCES += crypto/scrypt-sse2.cpp
endif

# consensus: shared between all executables that validate any consensus rules.
libfleetcredits_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES) -I$(srcdir)/secp256k1/include
libfleetcredits_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/pureheader.cpp \
  primitives/pureheader.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  primitives/contribution.cpp \
  primitives/contribution.h \
  pubkey.cpp \
  pubkey.h \
  script/fleetcreditsconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between fleetcreditsd, and fleetcredits-qt and non-server tools
libfleetcredits_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
libfleetcredits_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chain.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  fleetcredits.cpp \
  fleetcredits-fees.cpp \
  key.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  pow.cpp \
  primitives/block.cpp \
  primitives/pureheader.cpp \
  primitives/transaction.cpp \
  primitives/contribution.cpp \
  primitives/mweb.cpp \
  primitives/verification.cpp \
  mweb_contributions.cpp \
  mweb_mempool.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/sign.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(FLEETCREDITS_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libfleetcredits_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
libfleetcredits_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_util_a_SOURCES = \
  support/lockedpool.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  fs.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(FLEETCREDITS_CORE_H)

if GLIBC_BACK_COMPAT

libfleetcredits_util_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)

endif

# cli: shared between fleetcredits-cli and fleetcredits-qt
libfleetcredits_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
libfleetcredits_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libfleetcredits_cli_a_SOURCES = \
  rpc/client.cpp \
  $(FLEETCREDITS_CORE_H)
nodist_libfleetcredits_util_a_SOURCES = $(srcdir)/obj/build.h
#

# fleetcreditsd binary #
fleetcreditsd_SOURCES = fleetcreditsd.cpp
fleetcreditsd_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
fleetcreditsd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
fleetcreditsd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
fleetcreditsd_SOURCES += fleetcreditsd-res.rc
endif

fleetcreditsd_LDADD = \
  $(LIBFLEETCREDITS_SERVER) \
  $(LIBFLEETCREDITS_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBFLEETCREDITS_UTIL) \
  $(LIBFLEETCREDITS_WALLET) \
  $(LIBFLEETCREDITS_ZMQ) \
  $(LIBFLEETCREDITS_CONSENSUS) \
  $(LIBFLEETCREDITS_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

fleetcreditsd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# fleetcredits-cli binary #
fleetcredits_cli_SOURCES = fleetcredits-cli.cpp
fleetcredits_cli_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES) $(EVENT_CFLAGS)
fleetcredits_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
fleetcredits_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
fleetcredits_cli_SOURCES += fleetcredits-cli-res.rc
endif

fleetcredits_cli_LDADD = \
  $(LIBFLEETCREDITS_CLI) \
  $(LIBUNIVALUE) \
  $(LIBFLEETCREDITS_UTIL) \
  $(LIBFLEETCREDITS_CRYPTO)

fleetcredits_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)

#

# fleetcredits-tx binary #
fleetcredits_tx_SOURCES = fleetcredits-tx.cpp
fleetcredits_tx_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES)
fleetcredits_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
fleetcredits_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
fleetcredits_tx_SOURCES += fleetcredits-tx-res.rc
endif

fleetcredits_tx_LDADD = \
	$(LIBUNIVALUE) \
	$(LIBFLEETCREDITS_COMMON) \
	$(LIBFLEETCREDITS_UTIL) \
	$(LIBFLEETCREDITS_CONSENSUS) \
	$(LIBFLEETCREDITS_CRYPTO) \
	$(LIBSECP256K1)

fleetcredits_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# fleetcreditsconsensus library #
if BUILD_FLEETCREDITS_LIBS
include_HEADERS = script/fleetcreditsconsensus.h
libfleetcreditsconsensus_la_SOURCES = $(crypto_libfleetcredits_crypto_a_SOURCES) $(libfleetcredits_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libfleetcreditsconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libfleetcreditsconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) $(FLEETCREDITS_INCLUDES) -I$(srcdir)/secp256k1/include
libfleetcreditsconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libfleetcreditsconsensus_la_LIBADD = $(LIBSECP256K1)
libfleetcreditsconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)


endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
#CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = $(CTAES_DIST)


config/fleetcredits-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/fleetcredits-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/fleetcredits-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/fleetcredits-config.h.in

# Legacy alias
config/fleetcredits-config.h: config/fleetcredits-config.h
	@test -f config/fleetcredits-config.h || $(MAKE) -C $(top_builddir) $(subdir)/config/fleetcredits-config.h

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
	@echo "Checking glibc back compat..."
	$(AM_V_at) $(PYTHON) $(top_srcdir)/contrib/devtools/symbol-check.py $(bin_PROGRAMS)

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) $(PYTHON) $(top_srcdir)/contrib/devtools/security-check.py $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
