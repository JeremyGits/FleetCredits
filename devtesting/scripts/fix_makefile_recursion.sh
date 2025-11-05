#!/bin/bash
# Fix recursive variable definitions in Makefile.am

sed -i '/^# Legacy aliases$/,/^libfleetcredits.*= \$(libfleetcredits.*)$/d' src/Makefile.am
sed -i '/^crypto_libfleetcredits_crypto_a_SOURCES =$/d' src/Makefile.am
sed -i '/^libfleetcredits.*_SOURCES =$/d' src/Makefile.am
sed -i '/^libfleetcredits.*_CPPFLAGS = \$(libfleetcredits.*_CPPFLAGS)$/d' src/Makefile.am
sed -i '/^libfleetcredits.*_CXXFLAGS = \$(libfleetcredits.*_CXXFLAGS)$/d' src/Makefile.am
sed -i '/^libfleetcredits.*_LDFLAGS = \$(libfleetcredits.*_LDFLAGS)$/d' src/Makefile.am
sed -i '/^libfleetcredits.*_LIBADD = \$(libfleetcredits.*_LIBADD)$/d' src/Makefile.am

# Remove duplicate line 353
sed -i '353d' src/Makefile.am

# Remove duplicate scrypt-sse2 line
sed -i '315d' src/Makefile.am

# Remove duplicate nodist line
sed -i '446d' src/Makefile.am

# Remove duplicate consensus lib definitions at end
sed -i '/^libfleetcreditsconsensus_la_SOURCES = \$(libfleetcreditsconsensus_la_SOURCES)$/d' src/Makefile.am
sed -i '/^libfleetcreditsconsensus_la_LDFLAGS = \$(libfleetcreditsconsensus_la_LDFLAGS)$/d' src/Makefile.am
sed -i '/^libfleetcreditsconsensus_la_LIBADD = \$(libfleetcreditsconsensus_la_LIBADD)$/d' src/Makefile.am
sed -i '/^libfleetcreditsconsensus_la_CPPFLAGS = \$(AM_CPPFLAGS)/d' src/Makefile.am
sed -i '/^libfleetcreditsconsensus_la_CXXFLAGS = \$(libfleetcreditsconsensus_la_CXXFLAGS)$/d' src/Makefile.am

echo "Makefile.am cleaned"

