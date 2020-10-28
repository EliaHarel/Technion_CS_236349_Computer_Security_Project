#!/bin/bash

# ./AttackAll attack_num rounds pairs iterations ~/resutls

./AttackAll 1 10 8000000 20 ./results/Attack1
./AttackAll 1 10 16000000 20 ./results/Attack1
./AttackAll 1 12 8000000 20 ./results/Attack1
./AttackAll 1 12 16000000 20 ./results/Attack1
./AttackAll 1 14 8000000 20 ./results/Attack1
./AttackAll 1 14 16000000 20 ./results/Attack1

attack_num=3
iterations=20
##############################
arr_6_rounds=(5000 10000 20000 50000 100000)
arr_8_rounds=(5000 10000 20000 50000 100000)
arr_10_rounds=(20000 50000 100000 200000)
arr_12_rounds=(20000 50000 100000 200000)
arr_14_rounds=(100000 200000 500000)

for pairs in ${arr_6_rounds[@]}; do; echo ./AttackAll ${attack_num} 6 ${pairs} $iterations ; done
for pairs in ${arr_8_rounds[@]}; do; echo ./AttackAll ${attack_num} 8 ${pairs} $iterations; done
for pairs in ${arr_10_rounds[@]}; do; echo ./AttackAll ${attack_num} 10 ${pairs} $iterations; done
for pairs in ${arr_12_rounds[@]}; do; echo ./AttackAll ${attack_num} 12 ${pairs} $iterations; done
for pairs in ${arr_14_rounds[@]}; do; echo ./AttackAll ${attack_num} 14 ${pairs} $iterations; done
