#!/bin/sh

# Because I'm too lazy to change the code we use a bit of shell magic to modify the input to do the right thing.
set -eu

exe="$1"
input="$2"

# Run the initial input and set ${result1} to whatever the value of wire 'a' is.
result1="$("${exe}" <"${input}" | grep "^a = " | sed -e "s@a = @@")"

# Now override the setting of wire b to the value of a and re-run.
cat "${input}" | sed -e "s@^.* -> b\$@${result1} -> b@" | "${exe}"
