#!/bin/bash
for i in $(seq 1000000 1000 3000000); do echo "(  $i, uint256S(\"$(lynx-cli getblockhash $i)\"))"; done
