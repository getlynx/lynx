#!/bin/bash
for i in $(seq 0 100000 999999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 1000000 50000 1999999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 2000000 25000 2499999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 2500000 12500 2724999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 2725000 3000 2871999); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done
for i in $(seq 2882000 100 2899300); do echo "            (  $i, uint256S(\"0x$(lynx-cli getblockhash $i)\"))"; done