#!/bin/sh
# Поднимаем учебный стенд: netns wsc и пара veth. TUN создаёт сама программа.
set -e

NS=wsc
ip netns add $NS
# Иначе внутри не работает даже ping localhost.
ip netns exec $NS ip link set lo up
ip link add veth-h type veth peer name veth-c
ip link set veth-c netns $NS
ip addr add 10.200.0.1/24 dev veth-h
ip link set veth-h up
ip netns exec $NS ip addr add 10.200.0.2/24 dev veth-c
ip netns exec $NS ip link set veth-c up
# Подсказка, как запускать клиента.
echo "Стенд поднят. Клиент запускать через: ip netns exec $NS ..."
