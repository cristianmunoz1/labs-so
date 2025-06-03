#!/bin/bash

TRIALS=1000

echo "pages,time_ns" > results.csv

for i in {1..14}; do
  PAGES=$((2**$i))  # 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384
  echo "Testing with $PAGES pages..."
  
  TIME=$(./tlb $PAGES $TRIALS)
  
  echo "$PAGES,$TIME" >> results.csv
done


