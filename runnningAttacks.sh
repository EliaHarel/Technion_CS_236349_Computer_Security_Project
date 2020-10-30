#!/bin/bash

# ./AttackAll attack_num rounds pairs iterations ~/resutls

./AttackAll 1 10 32000000 20 ~/results/Attack1
./AttackAll 1 10 64000000 20 ~/results/Attack1
./AttackAll 1 12 64000000 20 ~/results/Attack1
./AttackAll 1 12 128000000 20 ~/results/Attack1
./AttackAll 1 14 128000000 20 ~/results/Attack1
./AttackAll 1 16 128000000 20 ~/results/Attack1


# ./AttackAll 3 8 200000 20 ~/results/Attack2
./AttackAll 3 8 500000 20 ~/results/Attack2
# ./AttackAll 3 10 1000000 20 ~/results/Attack2
./AttackAll 3 10 2000000 20 ~/results/Attack2
./AttackAll 3 12 4000000 20 ~/results/Attack2
./AttackAll 3 12 8000000 20 ~/results/Attack2
# ./AttackAll 3 14 16000000 20 ~/results/Attack2
./AttackAll 3 16 16000000 20 ~/results/Attack2




attack_num=3
iterations=20
##############################
# arr_6_rounds=(5000 10000 20000 50000 100000)
# arr_8_rounds=(5000 10000 20000 50000 100000)
# arr_10_rounds=(20000 50000 100000 200000)
# arr_12_rounds=(20000 50000 100000 200000)
# arr_14_rounds=(100000 200000 500000)

# for pairs in ${arr_6_rounds[@]}; do ./AttackAll ${attack_num} 6 ${pairs} $iterations ~/results/Attack2; done
# for pairs in ${arr_8_rounds[@]}; do ./AttackAll ${attack_num} 8 ${pairs} $iterations ~/results/Attack2; done
# for pairs in ${arr_10_rounds[@]}; do ./AttackAll ${attack_num} 10 ${pairs} $iterations ~/results/Attack2; done
# for pairs in ${arr_12_rounds[@]}; do ./AttackAll ${attack_num} 12 ${pairs} $iterations ~/results/Attack2; done
# for pairs in ${arr_14_rounds[@]}; do ./AttackAll ${attack_num} 14 ${pairs} $iterations ~/results/Attack2; done
