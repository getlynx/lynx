#!/bin/bash
for i in $(seq 1 1 5); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 500000 500000 2899999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 2907396 1 2907400); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done